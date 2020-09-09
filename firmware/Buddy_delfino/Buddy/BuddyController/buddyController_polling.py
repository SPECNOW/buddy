import serial
import sys
import pygame
import re
import math
import ast
import time
import thread
import logging
import signal
from math import pi
logging.basicConfig(filename='buddy.log',level=logging.INFO)
logging.getLogger().addHandler(logging.StreamHandler())

NEW_JOYPOS = False
leftMotorTargVel = 0
rightMotorTargVel = 0
output_buff = []
leftMotorVelSet = False
rightMotorVelSet = False
syncError = False


def signal_handler(signal, frame):
    print 'You pressed Ctrl+C!'
    pygame.quit()
    sys.exit(0)


    
def set_motor_thread():
    global NEW_JOYPOS, leftMotorTargVel, rightMotorTargVel, output_buff, syncError, leftMotorVelSet, rightMotorVelSet
    while(True):
        if NEW_JOYPOS:
            rightMotorTargVel = min(rightMotorTargVel, 1.0)
            rightMotorTargVel = max(rightMotorTargVel, -1.0)
            local_rightMotorTargVel = rightMotorTargVel
            leftMotorTargVel = min(leftMotorTargVel, 1.0)
            leftMotorTargVel = max(leftMotorTargVel, -1.0)
            local_leftMotorTargVel = leftMotorTargVel
            
            logging.info( "Sending new Motor Command" )
            print 'L ', int(145 + 48.5*(1+local_leftMotorTargVel))
            output_buff.append('L' + chr(int(145 + 48.5*(1+local_leftMotorTargVel))) )
            motorTimeOut = time.time() + 1
            while not leftMotorVelSet:
                time.sleep(0.001)
                if syncError or time.time() > motorTimeOut:
                    output_buff.append('L' + chr(int(145 + 48.5*(1+local_leftMotorTargVel))) )
                    if syncError:
                        syncError = False    
            print 'R ', int(145 + 48.5*(1+local_rightMotorTargVel))
            output_buff.append('R' + chr(int(145 + 48.5*(1+local_rightMotorTargVel))) )
            motorTimeOut = time.time() + 1
            while not rightMotorVelSet:
                time.sleep(0.001)
                if syncError or time.time() > motorTimeOut:
                    output_buff.append('R' + chr(int(145 + 48.5*(1+local_rightMotorTargVel))) )
                    if syncError:
                        syncError = False
            leftMotorVelSet = False
            rightMotorVelSet = False
            NEW_JOYPOS = False

def readlinesquick(ser):
    global noSerialDebug
    if noSerialDebug:
        return []
    buff = []
    while ser.inWaiting():
        buff.append(ser.readline())    
    return buff

def sync_serial(ser):
    logging.debug("Attempting to synchronize Serial Data...")
    while True:
        if not ser.inWaiting():
            output_buff.append('0')
            time.sleep(5)
        else:
            buff = readlinesquick(ser)
            check_for_buff_error = False
            for line in buff:
                logging.info(line.strip('\r\n'))
            if 'INCORRECT COMMAND'.upper() in buff[-1].upper():
                check_for_buff_error = True
            if check_for_buff_error:
                break
    logging.debug("...Serial Data Re-Synchronized")

def serial_write_thread(ser):
    global output_buff
    while(True):
        if len(output_buff):
            logging.debug("WRITING " + str(output_buff[0] + " To Serial") )
            ser.write( output_buff.pop(0) )
            
def serial_read_thread(ser):
    global leftMotorVelSet, rightMotorVelSet, syncError
    while(True):
        if ser.inWaiting():
            # buff = ser.readlines()
            buff = readlinesquick(ser)
            for line in buff:
                vel_scan = re.search('Velocity: (\d*.\d*)', line)
                lms = re.search('Left Motor command received', line)
                rms = re.search('Right Motor command received', line)
                if vel_scan:
                    current_velocity = float(vel_scan.group(1))
                if lms:
                    leftMotorVelSet = True
                if rms:
                    rightMotorVelSet = True
                logging.info(line.strip('\r\n'))
            if 'incorrect command'.upper() in buff[-1].upper():
                logging.warn( "ERROR: Serial Data not Synchronized" )
                sync_serial(ser)
                syncError = True
        time.sleep(0.001)


def main(useJoystick=False, noSerialDebug=False):
    global leftMotorVelSet, rightMotorVelSet, output_buff, syncError, NEW_JOYPOS, leftMotorTargVel, rightMotorTargVel
    
    if not noSerialDebug: 
        ser = serial.Serial(port='COM8', baudrate=9600, stopbits=1, parity='N', rtscts=0, timeout=0.1, bytesize=8)
    pygame.init()
    pygame.display.set_caption("Joystick")
    xbox_controller = False
    clock = pygame.time.Clock()

    screen = pygame.display.set_mode(size)
    
    try:
        if not noSerialDebug: 
            thread.start_new_thread(serial_write_thread, (ser,))
    except:
        import pdb; pdb.set_trace()
        logging.warn( "Failed to start Serial Write Thread" )
    
    if not noSerialDebug: 
        sync_serial(ser)
    
    try:
        if not noSerialDebug: 
            thread.start_new_thread(serial_read_thread, (ser,))
    except:
        import pdb; pdb.set_trace()
        logging.warn( "Failed to start Serial Read Thread" )

    try:
        thread.start_new_thread(set_motor_thread, ())
    except:
        import pdb; pdb.set_trace()
        logging.warn( "Failed to start Motor Speed Thread" )
        
    if useJoystick: # needs improvement to do polling instead of event interrupts and reading from a buffer the events
        if pygame.joystick.get_count() > 0:
            xbox_controller = pygame.joystick.Joystick(0)
            xbox_controller.init()
        else:
            logging.critical("NO JOYSTICK CONNECTED")
            sys.exit()

        if xbox_controller:
            while(True):
                draw(screen)
                clock.tick(10)
                for event in pygame.event.get():
                    if event.type == pygame.KEYDOWN:
                        if event.key == pygame.K_ESCAPE:
                            pygame.quit()
                            sys.exit()
                    elif event.type == pygame.QUIT:
                        pygame.quit()
                    elif event.type == pygame.JOYAXISMOTION:
                        _axis = ast.literal_eval(str(event)[23:-2])
                        if _axis['axis'] == 1:
                            leftMotorTargVel = -_axis['value']
                        elif _axis['axis'] == 3:
                            rightMotorTargVel = -_axis['value']
                        NEW_JOYPOS = True            
                    elif event.type == pygame.JOYBUTTONDOWN:
                        if xbox_controller.get_button(0):    # A Button
                            leftMotorTargVel = 0
                            rightMotorTargVel = 0
                            NEW_JOYPOS = True
        else:
            logging.warn( "ERROR: No Joystick Found" )
    else: # use keyboard, assume rightMotorTargVel and leftMotorTargVel is 0-256
        pygame.key.set_repeat(500, 100) #delay 500 ms after sending the first pygame.KEYDOWN, then every 100ms send a pygame.KEYDOWN 
        pygame.event.clear()
        pygame.event.set_allowed((pygame.KEYDOWN, pygame.QUIT,))
        while(True):
            draw(screen)
            clock.tick(10)
            for events in pygame.event.get():
                if events.type == pygame.KEYDOWN:
                    if events.key == pygame.K_ESCAPE:
                        pygame.quit()
                        sys.exit()
                    elif events.key == pygame.K_0:
                        rightMotorTargVel = 0
                        leftMotorTargVel = 0
                        output_buff.append('L0')
                        output_buff.append('R0')
                        NEW_JOYPOS = True #to send cmd to change the motor speed
                    elif events.key == pygame.K_UP:
                        rightMotorTargVel = leftMotorTargVel = (rightMotorTargVel + leftMotorTargVel)/2 + .1
                        NEW_JOYPOS = True #to send cmd to change the motor speed
                    elif events.key == pygame.K_DOWN:
                        leftMotorTargVel = rightMotorTargVel = (rightMotorTargVel + leftMotorTargVel)/2 - .1
                        NEW_JOYPOS = True #to send cmd to change the motor speed
                    elif events.key == pygame.K_LEFT:
                        rightMotorTargVel += .05
                        leftMotorTargVel -= .05
                        NEW_JOYPOS = True #to send cmd to change the motor speed
                    elif events.key == pygame.K_RIGHT:
                        rightMotorTargVel -= .05
                        leftMotorTargVel += .05
                        NEW_JOYPOS = True #to send cmd to change the motor speed 
    return
red = (255, 0, 0)
green = (0,255,0)
blue = (0,0,255)
black = (0,0,0)
white = (255, 255, 255)
grey = (128, 128, 128)


image_width = 140+140/4
image_height = 180+180/4
size = width, height = image_width*3, image_height*4
bimg = pygame.image.load(r'buddy.png')
bimg = pygame.transform.scale(bimg, (image_width, image_height))    
status_timer = time.time()
box_size = 300
color = (255, 0, 0)
start = 0
end = pi/2
arcwidth = 5
vel_bar_pad = 5

circle_pos=\
    {
        0 : {   'x':0,              'y':0,             },
        1 : {   'x':0,              'y':image_height,  },
        2 : {   'x':0,              'y':2*image_height,},
        3 : {   'x':2*image_width,  'y':2*image_height,},
        4 : {   'x':2*image_width,  'y':image_height,  },
        5 : {   'x':2*image_width,  'y':0,             }
    }                                                  
    
def draw(screen):
    global image_width, image_height, leftMotorTargVel, rightMotorTargVel, width, height, status_timer, box_size, color, start, end, arcwidth, circle_pos
    #start prep the next frame
    screen.fill( white )
    
    ### Left Motor Info
    pygame.draw.rect(
                        screen, 
                        black, 
                        (   
                            image_width/4, 
                            image_height*3+image_height/4, 
                            image_width/2, 
                            image_height/2
                        ), 
                        0
                    )
    if leftMotorTargVel >= 0:
        temp_left_color = green
    else:
        temp_left_color = red
    pygame.draw.rect(   
                        screen, 
                        temp_left_color, 
                        (   
                            image_width/4 + vel_bar_pad, 
                            image_height*3+image_height/2,
                            image_width/2-2*vel_bar_pad, 
                            (image_height-2*vel_bar_pad)/2*(-leftMotorTargVel)
                        ),
                        0
                    )
    ### Right Motor Info
    pygame.draw.rect(   
                        screen, 
                        black, 
                        (   
                            2*image_width + image_width/4, 
                            image_height*3 + image_height/4, 
                            image_width/2, 
                            image_height/2
                        ), 
                        0
                    )
                    
                    
    if rightMotorTargVel >= 0:
        temp_right_color = green
    else:
        temp_right_color = red

    pygame.draw.rect(
                        screen, 
                        temp_right_color, 
                        (
                            2*image_width + image_width/4 + vel_bar_pad, 
                            image_height*3 + image_height/2, 
                            image_width/2-2*vel_bar_pad, 
                            (image_height-2*vel_bar_pad)/2*(-rightMotorTargVel)
                        ), 
                        0
                    )
    
    arcSurface = pygame.Surface((image_width, image_height))
    
    
    for i in range(6):
        arcSurface.fill( white )
        # pygame.draw.rect(arcSurface, white, (0,0,image_width, image_height))
        arcSurface = draw_circle(arcSurface, signal_strength = i)
        if i == 0:
            arcSurface = arcSurface
        elif i == 1:
            # _tempSurf = pygame.Surface((image_width, image_height))
            # _tempSurf.fill( white )
            arcSurface = pygame.transform.flip(pygame.transform.smoothscale(pygame.transform.rotozoom(arcSurface, 45, 1), (image_width,image_height)), False, False)
            for x in range(arcSurface.get_width()):
                for y in range(arcSurface.get_height()):
                    (r,g,b,a) = arcSurface.get_at((x,y))
                    if r <= 120 and  g <= 120 and  b <= 120:
                        arcSurface.set_at((x,y), (255,255,255,255))
            #arcSurface = pygame.transform.smoothscale(pygame.transform.rotozoom(arcSurface, 45, 1), (image_width,image_height))
            #print pygame.transform.threshold(arcSurface, arcSurface, black, (64,64,64,255), (255,255,255,255), 2, arcSurface, False)
            # _tempSurf.blit(arcSurface, (0,0))
            # arcSurface = _tempSurf
        elif i == 2:
            arcSurface = pygame.transform.flip(arcSurface, False, True)
        elif i == 3:
            arcSurface = pygame.transform.flip(arcSurface, True, True)
        elif i == 4:
            arcSurface = pygame.transform.flip(pygame.transform.smoothscale(pygame.transform.rotozoom(arcSurface, 45, 1), (image_width,image_height)), True, False)
            for x in range(arcSurface.get_width()):
                for y in range(arcSurface.get_height()):
                    (r,g,b,a) = arcSurface.get_at((x,y))
                    if r <= 120 and  g <= 120 and  b <= 120:
                        arcSurface.set_at((x,y), (255,255,255,255))
        else:
            arcSurface = pygame.transform.flip(arcSurface, True, False)
        screen.blit(arcSurface, (circle_pos[i]['x'],circle_pos[i]['y']))
        
    screen.blit(bimg, (image_width,image_height))
    ### Draw to Screen
    pygame.display.flip()
    if time.time() - status_timer >= 1:
        logging.info( "Getting Status" )
        output_buff.append('sr')
        output_buff.append('av')
        status_timer = time.time()
    time.sleep(0.001)
    return
 
#holds x and y and radius
circle_dict=\
    {
        0 : {   'x':0,              'y':0,              'r':5,  'sa':pi/2,  'ea':pi },
        1 : {   'x':0,              'y':0,              'r':5,  'sa':3*pi/4,'ea':5*pi/4},
        2 : {   'x':0,              'y':2*image_height, 'r':5,  'sa':pi,    'ea':3*pi/2},
        3 : {   'x':2*image_width,  'y':2*image_height, 'r':5,  'sa':3*pi/2,'ea':2*pi},
        4 : {   'x':2*image_width,  'y':image_height,   'r':5,  'sa':7*pi/4,'ea':pi/4},
        5 : {   'x':2*image_width,  'y':0,              'r':5,  'sa':0,     'ea':pi/2}
    }
        
def draw_circle(screen=None,signal_strength = 1, sensor_loc = 0):
    """
    pass in the location 0-5, then signal strength of 0-5
    it will draw stuff
    """
    global circle_dict
    signal_strength_mod=100-signal_strength*10
    
    for i in range(5): # draw the blank 5 times
            pygame.draw.arc(screen, 
                        grey, 
                        (circle_dict[sensor_loc]['x']+20*i, circle_dict[sensor_loc]['y']+20*i, 200-20*i,  200-20*i), 
                        circle_dict[sensor_loc]['sa'], 
                        circle_dict[sensor_loc]['ea'], 
                        circle_dict[sensor_loc]['r'])
    
    for i in range(4, 4-signal_strength, -1): # draw the blue 5 times
        pygame.draw.arc(
                            screen, 
                            blue, 
                            (circle_dict[sensor_loc]['x']+20*i, circle_dict[sensor_loc]['y']+20*i, 200-20*i,  200-20*i), 
                            circle_dict[sensor_loc]['sa'], 
                            circle_dict[sensor_loc]['ea'], 
                            circle_dict[sensor_loc]['r']
                        )
    #   Antialiasing Code to fill in holes in teh arcs. This takes way too much CPU and causes it to lag.   
    # for x in range(1,screen.get_width()-1):
    #    for y in range(1, screen.get_height()-1):
    #        (r,g,b,a) = screen.get_at((x,y))
    #        (rl,gl,bl,al) = screen.get_at((x-1,y))
    #        (rr,gr,br,ar) = screen.get_at((x+1,y))
    #        (ru,gu,bu,au) = screen.get_at((x,y-1))
    #        (rd,gd,bd,ad) = screen.get_at((x,y+1))
    #        if (r == 255 and b == 255 and g == 255) and (rl == rr == ru == rd) and (gl == gr == gu == gd) and (bl == br == bu == bd):
    #            screen.set_at((x,y), (rl,gl,bl,al))

    return screen

if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    while True:
        noSerialDebug = raw_input("Is serial mode enabled? (y/n):")
        if noSerialDebug == 'y':
            noSerialDebug = bool(0)
            break
        elif noSerialDebug == 'n':
            noSerialDebug = bool(1)
            break
    while True:
        useJoystick = raw_input("Do you have a joystick? (y/n):")
        if useJoystick == 'y':
            main(useJoystick=True, noSerialDebug=noSerialDebug)
            break
        elif useJoystick == 'n':
            main(useJoystick=False, noSerialDebug=noSerialDebug)
            break
    
pygame.display.quit()
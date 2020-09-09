import ast
import logging
import math
import os
import re
import sys
import _thread
import time

logging.basicConfig(filename='buddy.log',level=logging.DEBUG)
logging.getLogger().addHandler(logging.StreamHandler())

sys.path.insert(1, os.path.abspath(os.path.join("tests")))
import BuddyPacket as BuddyPacket
import serial
import pygame

NEW_JOYPOS = False
leftMotorTargVel = 127
rightMotorTargVel = 127
output_buff = BuddyPacket.GET_PACKET

def set_motor_thread():
    """
    Set Motor Speed to be sent to Buddy
    """
    global NEW_JOYPOS, leftMotorTargVel, rightMotorTargVel, output_buff
    while(True):
        if NEW_JOYPOS:
            logging.info( "Sending new Motor Command" )
            logging.info('Left Motor: {}'.format(leftMotorTargVel))
            logging.info('Right Motor: {}'.format(rightMotorTargVel))
            output_buff = output_buff + [
                BuddyPacket.LEFT_MOTOR, leftMotorTargVel,
                BuddyPacket.RIGHT_MOTOR, rightMotorTargVel
            ]
            NEW_JOYPOS = False

def readlinesquick(ser):
    """
    Read from Serial Buffer
    """
    buff = []
    while ser.in_waiting:
        buff.append(ser.readline())    
    return buff

def sync_serial(ser):
    """
    Sync Serial Buffer by sending GET_PACKET[0] (0xFF) until Buddy responds
    """
    global output_buff
    logging.debug("Attempting to synchronize Serial Data...")
    while True:
        if ser.in_waiting < 18:
            output_buff = output_buff + [BuddyPacket.GET_PACKET[0]]
            time.sleep(0.1)
        else:
            buff = readlinesquick(ser)
            packet = BuddyPacket.BuddyPacket(buff[0][0:18])
            logging.info(packet)
            break
    logging.debug("...Serial Data Re-Synchronized")

def serial_write_thread(ser):
    """
    Monitor Output Buffer and write to Serial
    """
    global output_buff
    while(True):
        if len(output_buff):
            if output_buff == BuddyPacket.GET_PACKET:
                logging.debug("Sending GET_DATA to Delfino")
            elif len(output_buff) == 2:
                if output_buff[0] == BuddyPacket.LEFT_MOTOR:
                    logging.debug("Sending Left Motor {} to Delfino".format(output_buff[1]))
                elif output_buff[0] == BuddyPacket.RIGHT_MOTOR:
                    logging.debug("Sending Right Motor {} to Delfino".format(output_buff[1]))
            else:
                logging.debug("Sending {} to Delfino".format(output_buff))
            ser.write( output_buff )
            output_buff = []
            
def serial_read_thread(ser):
    """
    Read Serial resonses from Buddy
    """
    while(True):
        if ser.in_waiting >= 18:
            buff = readlinesquick(ser)
            packet = BuddyPacket.BuddyPacket(buff[0][0:18])
            logging.info(packet)
        time.sleep(0.001)

def joy_to_vel(joy_pos):
    """
    Convert Joystick Position (-1,1) to Speed (0,255)
    """
    vel = min(joy_pos, 1.0)
    vel = max(vel, -1.0)
    return int(vel*127 + 127)

def main():
    """
    Main Function
    Create threads for handling Serial and Setting Motor Speed, handle Joystick Stuff and Drawing GUI in main thread
    """
    global output_buff, NEW_JOYPOS, leftMotorTargVel, rightMotorTargVel
    
    ser = serial.Serial(port='COM4', baudrate=115200, stopbits=1, parity='N', rtscts=0, timeout=0.1, bytesize=8)
    pygame.init()
    
    status_timer = time.time()
    pygame.display.set_caption("Joystick")
    xbox_controller = False
    size = width, height = 640, 480    
    clock = pygame.time.Clock()

    screen = pygame.display.set_mode(size)
    bimg = pygame.image.load(os.path.join("BuddyController", 'buddy.png'))
    bimg = pygame.transform.scale(bimg, (280, 360))    
    
    if pygame.joystick.get_count() > 0:
        xbox_controller = pygame.joystick.Joystick(0)
        xbox_controller.init()
    else:
        logging.critical("NO JOYSTICK CONNECTED")
        sys.exit()

    try:
        _thread.start_new_thread(serial_write_thread, (ser,))
    except:
        logging.warn( "Failed to start Serial Write _thread" )
        sys.exit()
    
    sync_serial(ser)
    
    try:
        _thread.start_new_thread(serial_read_thread, (ser,))
    except:
        logging.warn( "Failed to start Serial Read _thread" )
        sys.exit()
    
    try:
        _thread.start_new_thread(set_motor_thread, ())
    except:
        logging.warn( "Failed to start Motor Speed _thread" )
        sys.exit()
            
    if xbox_controller:
        while(True):
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
                        leftMotorTargVel = joy_to_vel(-_axis['value'])
                    elif _axis['axis'] == 3:
                        rightMotorTargVel = joy_to_vel(-_axis['value'])
                    NEW_JOYPOS = True            
                elif event.type == pygame.JOYBUTTONDOWN:
                    if xbox_controller.get_button(0):    # A Button
                        leftMotorTargVel = 127
                        rightMotorTargVel = 127
                        NEW_JOYPOS = True
                        
            screen.fill( (255, 255, 255) )
            ### Left Motor Info
            pygame.draw.rect(screen, (0, 0, 0),
                (50, 50, 100, height-100),
            0)
            if leftMotorTargVel >= 127:
                pygame.draw.rect(screen, 
                    (0, 255, 0),
                        (55, int((height)/2),
                        100-10, int((height-100-5)/2*-(leftMotorTargVel-127)/127) ),
                    0)
            else:
                pygame.draw.rect(screen,
                    (255, 0, 0),
                        (55, int((height)/2),
                        100-10, int( (height-100-5)/2*-(leftMotorTargVel-127)/127) ),
                    0)
            ### Right Motor Info
            pygame.draw.rect(screen, (0, 0, 0), 
                (width - 150, 50, 100, height-100),
            0)
            if rightMotorTargVel >= 127:
                pygame.draw.rect(screen, 
                    (0, 255, 0), 
                        (width-145, int((height)/2),
                        100-10, int((height-100-5)/2*-(rightMotorTargVel-127)/127) ), 
                    0)
            else:
                pygame.draw.rect(screen, 
                    (255, 0, 0), 
                        (width-145, int((height)/2),
                        100-10, int((height-100-5)/2*-(rightMotorTargVel-127)/127) ),
                    0)
            ### Draw to Screen
            screen.blit(bimg, (175,60))
            pygame.display.flip()
            if time.time() - status_timer >= 1:
                logging.info( "Getting Status" )
                output_buff = output_buff + BuddyPacket.GET_PACKET
                status_timer = time.time()
            time.sleep(0.001)
    else:
        logging.warn( "ERROR: No Joystick Found" )
        
if __name__ == '__main__':
    main()
    
pygame.display.quit()
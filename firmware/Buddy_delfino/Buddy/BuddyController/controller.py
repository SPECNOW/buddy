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

from test_base import Delfino

leftMotorTargVel = 127
rightMotorTargVel = 127
output_buff = []

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
    # Stop once synched
    output_buff = [BuddyPacket.LEFT_MOTOR, 127, BuddyPacket.RIGHT_MOTOR, 127]

def serial_write_thread(ser):
    """
    Monitor Output Buffer and write to Serial
    """
    global output_buff
    while(True):
        write_buff = []
        while len(output_buff):
            if output_buff[0:2] == BuddyPacket.GET_PACKET:
                logging.debug("Sending GET_DATA to Delfino")
            elif output_buff[0] == BuddyPacket.LEFT_MOTOR:
                logging.debug("Sending Left Motor  {} to Delfino".format(output_buff[1]))
            elif output_buff[0] == BuddyPacket.RIGHT_MOTOR:
                logging.debug("Sending Right Motor {} to Delfino".format(output_buff[1]))
            else:
                logging.debug("Sending {} to Delfino".format(output_buff[0:2]))
            write_buff = write_buff + [output_buff.pop(0) for packet in output_buff[0:2]]
        ser.write(write_buff)

def serial_read_thread(ser):
    """
    Read Serial resonses from Buddy
    """
    while(True):
        if ser.in_waiting >= 18:
            buff = readlinesquick(ser)
            packet = BuddyPacket.BuddyPacket(buff[0][0:18])
            logging.info(packet)

def joy_to_vel(joy_pos):
    """
    Convert Joystick Position (-1,1) to Speed (0,255)
    """
    vel = min(joy_pos, 1.0)
    vel = max(vel, -1.0)
    return int(vel*127 + 127)

def draw(screen, bimg, width, height):
    """
    Draw to Screen
    """
    global leftMotorTargVel, rightMotorTargVel
    screen.fill( pygame.Color('white') )
    ### Left Motor Info
    pygame.draw.rect(screen, pygame.Color('black'),
        (50, 50, 100, height-100),
    0)
    if leftMotorTargVel >= 127:
        pygame.draw.rect(screen,
            pygame.Color('green'),
                (55, int((height)/2),
                100-10, int((height-100-5)/2*-(leftMotorTargVel-127)/127) ),
            0)
    else:
        pygame.draw.rect(screen,
            pygame.Color('red'),
                (55, int((height)/2),
                100-10, int( (height-100-5)/2*-(leftMotorTargVel-127)/127) ),
            0)
    ### Right Motor Info
    pygame.draw.rect(screen, pygame.Color('black'),
        (width - 150, 50, 100, height-100),
    0)
    if rightMotorTargVel >= 127:
        pygame.draw.rect(screen,
            pygame.Color('green'),
                (width-145, int((height)/2),
                100-10, int((height-100-5)/2*-(rightMotorTargVel-127)/127) ),
            0)
    else:
        pygame.draw.rect(screen,
            pygame.Color('red'),
                (width-145, int((height)/2),
                100-10, int((height-100-5)/2*-(rightMotorTargVel-127)/127) ),
            0)
    ### Draw to Screen
    screen.blit(bimg, (175,60))
    pygame.display.flip()

def read_controller(xbox_controller):
    """
    Read Controller events
    """
    global output_buff, leftMotorTargVel, rightMotorTargVel
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
                logging.info("Sending new Motor Command")
                logging.info('Left Motor: {}'.format(leftMotorTargVel))
                output_buff = output_buff + [BuddyPacket.LEFT_MOTOR, leftMotorTargVel]
            elif _axis['axis'] == (3 if sys.platform != 'linux' else 4):
                rightMotorTargVel = joy_to_vel(-_axis['value'])
                logging.info("Sending new Motor Command")
                logging.info('Right Motor: {}'.format(rightMotorTargVel))
                output_buff = output_buff + [BuddyPacket.RIGHT_MOTOR, rightMotorTargVel]
        elif event.type == pygame.JOYBUTTONDOWN:
            if xbox_controller.get_button(0):    # A Button/ Stop
                leftMotorTargVel = 127
                rightMotorTargVel = 127
                logging.info("Sending new Motor Command")
                output_buff = output_buff + [BuddyPacket.LEFT_MOTOR, leftMotorTargVel, BuddyPacket.RIGHT_MOTOR, rightMotorTargVel]

def main():
    """
    Main Function
    Create threads for handling Serial and Setting Motor Speed, handle Joystick Stuff and Drawing GUI in main thread
    """
    global output_buff, leftMotorTargVel, rightMotorTargVel
    delfino = Delfino()

    ser = serial.Serial(
        port=os.environ.get('DELFINO_COM_PORT', 'COM4' if sys.platform != 'linux' else '/dev/ttyUSB0'),
        baudrate=115200,
        stopbits=1,
        parity='N',
        rtscts=0,
        timeout=0.1,
        bytesize=8
    )
    pygame.init()

    status_timer = time.time()
    pygame.display.set_caption("Buddy")
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

    _thread.start_new_thread(serial_write_thread, (ser,))
    sync_serial(ser)
    _thread.start_new_thread(serial_read_thread, (ser,))

    if xbox_controller:
        while(True):
            clock.tick(30)
            read_controller(xbox_controller)
            draw(screen, bimg, width, height)
            # Periodically get status
            if time.time() - status_timer >= 5:
                logging.info( "Getting Status" )
                output_buff = output_buff + BuddyPacket.GET_PACKET
                status_timer = time.time()
    else:
        logging.warn( "ERROR: No Joystick Found" )

if __name__ == '__main__':
    main()

pygame.display.quit()
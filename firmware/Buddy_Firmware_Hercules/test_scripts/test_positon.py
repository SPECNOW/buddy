import serial
import re
import time
import random

last_val = 0
position = 0
isForward = True

ser = serial.Serial()
ser.baurdrate = 9600
ser.port = "com8"
ser.timeout=0
ser.open()

while 1:
    ser.write("sr")
    buff = ser.read(1000).replace("\r", "").split("\n")[0]
    m = re.match(r"INFO: Position Count: ([\d]*)", buff)
    if "INFO: Position Count:" in buff:
        position = int(buff[21:])
        if not (((position >= last_val) and isForward) or ((position <= last_val) and not isForward)):
            isForward = not isForward
            print("direction changed")
        last_val = position
    print("\t\t" + str(position))
    time.sleep(random.random())
import serial
import struct

def main():
  ser = initserial()
  ser.write("sr")
  ser.read()
  serialprocess(ser.read(18))

def initserial(port = '/dev/ttyUSB0'):
  ser = serial.Serial()
  ser.baurdrate = 9600
  ser.port = port
  ser.open()
  return ser
  
def serialprocess(serialbuffer):
  Packetheader = ord(serialbuffer[0])
  ValidData = ord(serialbuffer[1])
  UltraF = ord(serialbuffer[2])
  UltraB = ord(serialbuffer[3])
  # serialbuffer[2:3] are 0x00,0x00 for some reason...
  EncL = stringtofloat(serialbuffer[4:8])
  EncR = stringtofloat(serialbuffer[8:12])
  infra = [ord(serialbuffer[12]), 
	   ord(serialbuffer[13]), 
	   ord(serialbuffer[14]), 
	   ord(serialbuffer[15]), 
	   ord(serialbuffer[16]), 
	   ord(serialbuffer[17])]
  return Packetheader, ValidData, EncL, EncR, UltraF, UltraB, infra
 
def stringtofloat(string):
  return struct.unpack('>f', string[::-1])[0]
  
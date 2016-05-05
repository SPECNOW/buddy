import serial
def main():
  ser = initserial()
  ser.open()
  ser.write("sr")
  ser.read()
  ser.write("sr")
  ser.open()
  ser.open()
  ser.port = '/dev/ttyUSB1'
  ser.open()
  ser.write("sr")
  ser.read()

def initserial():
  ser = serial.Serial()
  ser.baurdrate = 9600
  ser.port = '/dev/ttyUSB0'
  ser.open()
  return ser
  
def serialprocess(serialbuffer):
  Packetheader = ord(serialbuffer[0])
  ValiData = ord(serialbuffer[1])
  EncL = stringtofloat(serialbuffer[2:5])
  EncR = stringtofloat(serialbuffer[6:9])
  UltraF = ord(serialbuffer[10])
  UltraB = ord(serialbuffer[11])
  infra = [ord(serialbuffer[12]), 
	   ord(serialbuffer[13]), 
	   ord(serialbuffer[14]), 
	   ord(serialbuffer[15]), 
	   ord(serialbuffer[16]), 
	   ord(serialbuffer[17])]
 
def stringtofloat(string):
  return float(ord(string[0])<< 3 + ord(string[1]) << 2 + ord(string[2]) << 1 + ord(string[3]))
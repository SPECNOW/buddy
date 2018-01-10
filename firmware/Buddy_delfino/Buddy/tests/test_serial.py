import serial
import time
import pytest
import os
import subprocess

delfinoPort = 'COM10'
arduinoPort = 'COM9'

# TODO: Need to somehow get Arduino and Delfino to automatically 
#        flash correct test programs before running this

class Arduino:
    def __init__(self,  arduino_ino, arduino_path=r"C:\Program Files (x86)\Arduino", arduino_port=arduinoPort):
        print("Compiling " + arduino_ino)
        p = subprocess.Popen(
            [
                "arduino_debug.exe", 
                "--upload",
                "--board", "arduino:avr:mega",
                "--port", arduino_port,
                "--verbose",
                arduino_ino
            ],
            cwd=arduino_path
        )
        return p

class TestSerial(object):
    arduino_ino = None
    arduino = None
    def setup_class(cls):
        cls.serialDelfino = serial.Serial()
        cls.serialDelfino.baurdrate = 9600
        cls.serialDelfino.port = delfinoPort
        cls.serialDelfino.timeout = 1
        cls.serialDelfino.open()
        # Clear the Buffer
        cls.serialDelfino.read()        
        print("Delfino Serial Initialized")

        print("Compiling and uploading Arduino")
        cls.arduino = Arduino(arduino_ino=cls.arduino_ino)
        
        cls.serialArduino = serial.Serial()
        cls.serialArduino.baurdrate = 9600
        cls.serialArduino.port = arduinoPort
        cls.serialArduino.timeout = 1
        cls.serialArduino.open()
        # Clear the Buffer
        cls.serialArduino.read()
        print("Arduino Serial Initialized")
        return
        
class TestSerialRx(TestSerial):
    arduino_ino = os.path.abspath(os.getcwd()+"\..") + r"\ArduinoTest\ArduinoSerialRecieve\ArduinoSerialRecieve\ArduinoSerialRecieve.ino"
    def test_delfino_rx(self):
        for tx in "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz":
            self.serialArduino.write(tx)
            rx = self.serialDelfino.read()
            print "TX: {}, RX: {}".format(tx, rx)
            assert tx == rx
        return

class TestSerialTx(TestSerial):
    arduino_ino = os.path.abspath(os.getcwd()+"\..") + r"\ArduinoTest\171114Serialtest\171114Serialtest.ino"
    def test_delfino_tx(self):
        for tx in "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz":
            self.serialDelfino.write(tx)
            rx = self.serialArduino.read()
            print "TX: {}, RX: {}".format(tx, rx)
            assert tx == rx
        return
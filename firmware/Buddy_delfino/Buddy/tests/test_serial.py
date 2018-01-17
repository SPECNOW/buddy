import serial
import time
import pytest
import os
import sys
import subprocess

delfinoPort = 'COM10'
arduinoPort = 'COM9'

# TODO: Need to somehow get Arduino and Delfino to automatically 
#        flash correct test programs before running this

class Arduino:
    def __init__(self,  arduino_ino, arduino_path='C:\\Program Files (x86)\\Arduino', arduino_port=arduinoPort):
        print("Compiling " + arduino_ino)
        curDir = os.path.abspath(os.curdir)
        os.chdir(arduino_path)
        proc = subprocess.Popen(
            [
                "arduino_debug.exe",
                "--upload",
                "--board", "arduino:avr:mega",
                "--port", arduino_port,
                arduino_ino
            ],
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE
        )
        proc.wait()
        print "ERR: {}".format(''.join(proc.stderr.readlines()))
        print "OUT: {}".format(''.join(proc.stdout.readlines()))
        os.chdir(arduino_path)
        if (proc.returncode):
            print "Failed to compile"
            sys.exit()

class TestBase(object):
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
        
class TestDelfinoSerialRx(TestBase):
    arduino_ino = os.path.abspath(os.getcwd()) + r"\ArduinoTest\TestDelfinoSerialRx\TestDelfinoSerialRx.ino"
    def test_delfino_rx(self):
        testPass = True
        for tx in "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz":
            self.serialArduino.write(tx)
            rx = self.serialDelfino.read()
            print "Sent To Delfino: {}, Recieved By Delfino: {}".format(tx, rx)
            testPass = testPass and (tx == rx)
        assert testPass
        return

class TestDelfinoSerialTx(TestBase):
    arduino_ino = os.path.abspath(os.getcwd()) + r"\ArduinoTest\TestDelfinoSerialTx\TestDelfinoSerialTx.ino"
    def test_delfino_tx(self):
        testPass = True
        for tx in "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz":
            self.serialDelfino.write(tx)
            rx = self.serialArduino.read()
            print "Sent By Delfino: {}, Received By Arduino: {}".format(tx, rx)
            testPass = testPass and (tx == rx)
        assert testPass
        return
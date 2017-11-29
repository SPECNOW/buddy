import serial
import time
import pytest

delfinoPort = 'COM10'
arduinoPort = 'COM9'

# TODO: Need to somehow get Arduino and Delfino to automatically 
#        flash correct test programs before running this

class TestSerial(object):
    def setup_class(cls):
        cls.serialDelfino = serial.Serial()
        cls.serialDelfino.baurdrate = 9600
        cls.serialDelfino.port = delfinoPort
        cls.serialDelfino.timeout = 1
        cls.serialDelfino.open()
        # Clear the Buffer
        cls.serialDelfino.read()
        print("Delfino Serial Initialized")

        cls.serialArduino = serial.Serial()
        cls.serialArduino.baurdrate = 9600
        cls.serialArduino.port = arduinoPort
        cls.serialArduino.timeout = 1
        cls.serialArduino.open()
        # Clear the Buffer
        cls.serialArduino.read()
        print("Arduino Serial Initialized")

    def test_delfino_rx(self):
        for tx in "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz":
            self.serialArduino.write(tx)
            rx = self.serialDelfino.read()
            print "TX: {}, RX: {}".format(tx, rx)
            assert tx == rx
        return
        
    def test_delfino_tx(self):
        assert False
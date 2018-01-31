from tests.test_base import TestBase

import os
import pytest
     
class TestDelfinoSerialRx(TestBase):
    arduino_ino = os.path.abspath(os.getcwd()) + r"\ArduinoTest\TestDelfinoSerialRx\TestDelfinoSerialRx.ino"
    delfino_project = 'Buddy'
    
    def test_delfino_rx(self):
        testPass = True
        for tx in "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz":
            self.serialArduino.write(tx)
            rx = self.serialDelfino.read()
            print "Sent To Delfino: {}, Recieved By Delfino: {}".format(tx, rx)
            testPass = testPass and (tx == rx)
        assert testPass
        return
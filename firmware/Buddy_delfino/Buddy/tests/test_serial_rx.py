from tests.test_base import TestBase

import os
import pytest
     
@pytest.mark.skip(reason="Redundant")
class TestDelfinoSerialRx(TestBase):
    arduino_ino = os.path.abspath(os.getcwd()) + r"\ArduinoTest\TestDelfinoSerialRx\TestDelfinoSerialRx.ino"
    delfino_project = 'Buddy'
    
    def test_delfino_rx(self):
        testPass = True
        for tx in "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz":
            self.serialArduino.write([ord(tx)])
            rx = self.serialDelfino.read()
            print("Sent From Arduino: {}, Recieved By Delfino: {}".format(tx, rx))
            testPass = testPass and (tx == rx)
        assert testPass
        return
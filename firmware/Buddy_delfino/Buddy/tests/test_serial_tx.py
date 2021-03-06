from tests.test_base import TestBase

import os
import pytest

@pytest.mark.skip(reason="Redundant")
class TestDelfinoSerialTx(TestBase):
    arduino_ino = os.path.join(os.path.abspath(os.getcwd()), "ArduinoTest", "TestDelfinoSerialTx", "TestDelfinoSerialTx.ino")
    delfino_project = 'Buddy'
    
    def test_delfino_tx(self):
        testPass = True
        for tx in "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz":
            self.serialDelfino.write([ord(tx)])
            rx = self.serialArduino.read()
            print("Sent By Delfino: {}, Received By Arduino: {}".format(tx, rx))
            testPass = testPass and (tx == rx)
        assert testPass
        return
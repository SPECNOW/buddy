from tests.test_base import TestBase
from tests.BuddyPacket import LEFT_MOTOR
from tests.BuddyPacket import RIGHT_MOTOR

import os
import pytest

class TestSabertooth(TestBase):
    arduino_ino = os.path.join(os.path.abspath(os.getcwd()), "ArduinoTest", "TestSabertooth", "TestSabertooth.ino")
    delfino_project = 'Buddy'

    def test_brake(self):
        left_brake = [LEFT_MOTOR, 127]
        right_brake = [RIGHT_MOTOR, 127]

        self.serialDelfino.write(left_brake)
        rx = self.serialArduino.read()
        assert ord(rx) == 192
        print("Left Brake good")

        self.serialDelfino.write(right_brake)
        rx = self.serialArduino.read()
        assert ord(rx) == 64
        print("Right Brake good")

    def test_forward(self):
        left_brake = [LEFT_MOTOR, 255]
        right_brake = [RIGHT_MOTOR, 255]

        self.serialDelfino.write(left_brake)
        rx = self.serialArduino.read()
        assert ord(rx) == 255
        print("Left Throttle good")

        self.serialDelfino.write(right_brake)
        rx = self.serialArduino.read()
        assert ord(rx) == 127
        print("Right Throttle good")

    def test_reverse(self):
        left_brake = [LEFT_MOTOR, 0]
        right_brake = [RIGHT_MOTOR, 0]

        self.serialDelfino.write(left_brake)
        rx = self.serialArduino.read()
        assert ord(rx) == 128
        print("Left Reverse good")

        self.serialDelfino.write(right_brake)
        rx = self.serialArduino.read()
        assert ord(rx) == 1
        print("Right Reverse good")

    
    def test_left_motor(self):
        testPass = True
        last_speed = 0
        for speed in range(256):
            payload = [LEFT_MOTOR, speed]
            self.serialDelfino.write(payload)
            rx = self.serialArduino.read()
            print("Sent To Delfino: {}, Received By Arduino: {}".format(payload, ord(rx)))
            # Speed ramps up but must stay in motor's range
            testPass = testPass and ord(rx) >= last_speed and ord(rx) > 127
            last_speed = ord(rx)
        assert testPass
        return

    def test_right_motor(self):
        testPass = True
        last_speed = 0
        for speed in range(256):
            payload = [RIGHT_MOTOR, speed]
            self.serialDelfino.write(payload)
            rx = self.serialArduino.read()
            print("Sent To Delfino: {}, Received By Arduino: {}".format(payload, ord(rx)))
            # Speed ramps up but must stay in motor's range
            testPass = testPass and ord(rx) >= last_speed and ord(rx) < 128 
        assert testPass
        return
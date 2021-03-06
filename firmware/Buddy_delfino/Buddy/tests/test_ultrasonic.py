from tests.test_base import TestBase
from tests import BuddyPacket

import os
import time
import pytest

NUM_SAMPLES = 20
SAMPLE_PERIOD = 0.06

@pytest.mark.skip(reason="Under Develoment")
class TestUltrasonic(TestBase):
    arduino_ino = os.path.join(os.path.abspath(os.getcwd()), "ArduinoTest", "TestUltraSonic", "TestUltraSonic.ino")
    arduino_template = os.path.join(os.path.abspath(os.getcwd()), "ArduinoTest", "TestUltraSonic", "TestUltraSonic_base")
    delfino_project = 'Buddy'

    def test_max_a(self):
        samples = self.run_test('\xff', isTrigA=True)
        print(samples)
        return

    def test_min_a(self):
        samples = self.run_test('\x01', isTrigA=True)
        print(samples)
        return

    def test_medium_a(self):
        samples = self.run_test('\x3f', isTrigA=True)
        print(samples)
        return

    def test_max_b(self):
        samples = self.run_test('\xff', isTrigA=False)
        print(samples)
        return

    def test_min_b(self):
        samples = self.run_test('\x01', isTrigA=False)
        print(samples)
        return

    def test_medium_b(self):
        samples = self.run_test('\x3f', isTrigA=False)
        print(samples)
        return

    def run_test(self, pwm, isTrigA=True):
        self.serialDelfino.flushOutput()
        self.serialDelfino.flushInput()
        self.set_response(pwm, isTrigA)
        return self.get_samples()

    def set_response(self, pwm, isTrigA=True):
        self.serialArduino.flushOutput()
        self.serialArduino.flushInput()
        self.serialArduino.close()
        self.generate_ino(isTrigA)
        self.arduino.compile()
        self.serialArduino.open()
        self.serialArduino.read(2000)
        self.serialArduino.write([ord(pwm)])

    def generate_ino(self, isTrigA=True):
        with open(self.arduino_template, 'r') as f:
            ino = f.readlines()
        ino = "bool A = {};\n".format(isTrigA.__str__().lower()) + ''.join(ino)
        with open(self.arduino_ino, 'w') as f:
            f.write(ino)
    
    def get_samples(self):
        front = []
        back = []
        for _ in range(NUM_SAMPLES):
            time.sleep(SAMPLE_PERIOD)
            packet = BuddyPacket.get_packet(self.serialDelfino)
            front.append(packet.ultraSonicFront)
            back.append(packet.ultraSonicBack)
        return [front, back]

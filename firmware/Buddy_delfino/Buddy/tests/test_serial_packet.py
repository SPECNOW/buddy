from tests.test_base import TestBase
from tests import BuddyPacket

import os
import time
import pytest

RATE=200.0 # Hz

class TestSerialPacket(TestBase):
    arduino_ino = None
    delfino_project = 'Buddy'

    def test_single_packet_rx(self):
        self.get_multi_packet(1)

    def test_three_packet_rx(self):
        self.get_multi_packet(3)

    def test_ten_packet_rx(self):
        self.get_multi_packet(10)

    def test_one_hundred_packet_rx(self):
        self.get_multi_packet(100)

    def test_1second_packet_rx(self):
        self.get_multi_packet(RATE)

    @pytest.mark.skip()
    def test_5second_packet_rx(self):
        self.get_multi_packet(5*RATE)

    def get_multi_packet(self, num_packets):
        num_packets = int(num_packets)
        self.serialDelfino.flushOutput()
        self.serialDelfino.flushInput()
        for i in range(num_packets):
            self.serialDelfino.write([ord(char) for char in BuddyPacket.GET_PACKET])
            time.sleep(1.0/RATE)
        time.sleep(3)
        data = self.serialDelfino.read(1000*num_packets)
        assert data
        assert len(data) == 18*num_packets
        for i in range(num_packets):
            packet = BuddyPacket.BuddyPacket(data[i*18:(i+1)*18])
            assert packet
            assert packet.header == 255
            print(packet.encoderLeft)
            print(packet.encoderRight)
        return

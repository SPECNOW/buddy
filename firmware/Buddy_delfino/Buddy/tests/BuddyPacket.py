import struct

GET_PACKET = '\xff\xff'

def get_packet(serial):
    serial.flushOutput()
    serial.flushInput()
    serial.write(GET_PACKET)
    return BuddyPacket(serial.read(1000))

class BuddyPacket:
    header=\
    validData=\
    encoderLeft=\
    encoderRight=\
    ultraSonicFront=\
    ultraSonicBack = None
    infraRed=[]

    def __init__(self, data):
        assert len(data) == 18
        self.header = ord(data[0])
        self.validData = ord(data[1])
        self.ultraSonicFront = ord(data[2])
        self.ultraSonicBack = ord(data[3])
        self.encoderLeft = struct.unpack('!f', data[4:8])[0]
        self.encoderRight = struct.unpack('!f', data[8:12])[0]
        self.infra = [
            ord(data[12]),
            ord(data[13]),
            ord(data[14]),
            ord(data[15]),
            ord(data[16]),
            ord(data[17])
        ]

    def __str__(self):
        return """
Header:         {}
ValidData:      {}
UltraFront:     {}
UltraBack:      {}
encoderLeft:    {}
encoderRight:   {}
InfraRed:
    {}
    {}
    {}
    {}
    {}
    {}""".format(
        self.header,
        self.validData,
        self.ultraSonicFront,
        self.ultraSonicBack,
        self.encoderLeft,
        self.encoderRight,
        self.infra[0],
        self.infra[1],
        self.infra[2],
        self.infra[3],
        self.infra[4],
        self.infra[5])

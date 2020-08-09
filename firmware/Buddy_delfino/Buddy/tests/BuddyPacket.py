import struct

GET_PACKET = '\xff\xff'

def get_packet(serial):
    serial.flushOutput()
    serial.flushInput()
    serial.write([ord(char) for char in GET_PACKET])
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
        self.header = data[0]
        self.validData = data[1]
        self.ultraSonicFront = data[2]
        self.ultraSonicBack = data[3]
        self.encoderLeft = struct.unpack('!f', data[4:8])[0]
        self.encoderRight = struct.unpack('!f', data[8:12])[0]
        self.infra = [
            data[12],
            data[13],
            data[14],
            data[15],
            data[16],
            data[17]
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

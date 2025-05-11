import struct

class BMS:
    def __init__(self, ser, id):
        self.ser = ser
        self.id = id
    
    def cell_voltage(self, cell):
        self.ser.write((bytes([self.id, cell - 1])))
        return struct.unpack("<h", self.ser.read(2))[0]
    
    def stack_voltage(self):
        self.ser.write((bytes([self.id, 0x06])))
        return struct.unpack("<h", self.ser.read(2))[0]
    
    def current(self):
        self.ser.write((bytes([self.id, 0x07])))
        return struct.unpack("<h", self.ser.read(2))[0]
    
    def temp(self):
        self.ser.write((bytes([self.id, 0x08])))
        return round(struct.unpack("<f", self.ser.read(4))[0], 2)
    
    def output(self):
        self.ser.write((bytes([self.id, 0x09])))
        return self.ser.read(1)[0]
    
    def estop(self):
        self.ser.write((bytes([self.id, 0x0A])))
        return self.ser.read(1)[0]
    
class main_power:
    def __init__(self, ser):
        self.ser = ser
        self.id = id

    def wait(self):
        """
        Helper function, waits for EE system to respond before sending another command
        """
        self.ser.read()

    def current_3v3(self):
        self.ser.write((bytes([self.id, 0x00])))
        return struct.unpack("<f", self.ser.read(4))[0]
    
    def current_5v(self):
        self.ser.write((bytes([self.id, 0x01])))
        return struct.unpack("<f", self.ser.read(4))[0]

    def current_12v(self):
        self.ser.write((bytes([self.id, 0x02])))
        return struct.unpack("<f", self.ser.read(4))[0]

    def current_19v(self):
        self.ser.write((bytes([self.id, 0x03])))
        return struct.unpack("<f", self.ser.read(4))[0]

    def estop(self):
        self.ser.write((bytes([self.id, 0x04])))
        return self.ser.read(1)[0]

    def fan1(self, value):
        self.ser.write((bytes([self.id, 0x05, value])))
        self.ser.wait()

    def fan2(self, value):
        self.ser.write((bytes([self.id, 0x06, value])))
        self.ser.wait()
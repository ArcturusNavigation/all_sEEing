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
        return struct.unpack("<f", self.ser.read(4))[0]
    
    def output(self):
        self.ser.write((bytes([self.id, 0x09])))
        return self.ser.read(1)[0]
    
    def estop(self):
        self.ser.write((bytes([self.id, 0x0A])))
        return self.ser.read(1)[0]
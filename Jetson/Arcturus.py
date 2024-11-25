import struct

class LED:
    def __init__(self, ser):
        self.adr = 0x11
        self.ser = ser
    
    def wait(self):
        self.ser.read()
    
    def setMPL(self, led, value):
        self.ser.write(bytes([self.adr << 1 + 1, led, value]))
        self.wait()
    
    def fillStrip(self, r, g, b):
        self.ser.write(bytes([self.adr << 1 + 1, 0x04, r, g, b]))
        self.wait()
    
    def idvClr(self, px, r, g, b):
        self.ser.write(bytes([self.adr << 1 + 1, 0x05, px - 1, r, g, b]))
        self.wait()
    
    def clrStrip(self):
        self.ser.write(bytes([self.adr << 1 + 1, 0x06]))
        self.wait()

class BMS:
    def __init__(self, ser, adr):
        self.ser = ser
        self.adr = adr
    
    def wait(self):
        self.ser.read()
    
    def output(self, value):
        self.ser.write(bytes([self.adr << 1 + 1, 0x01, value]))
        self.wait()
    
    def voltage(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x02])))
        return struct.unpack("<f", self.ser.read(4))
    
    def current(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x03])))
        return struct.unpack("<f", self.ser.read(4))

    def cell1(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x04])))
        return struct.unpack("<f", self.ser.read(4))
    
    def cell2(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x05])))
        return struct.unpack("<f", self.ser.read(4))
    
    def cell3(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x06])))
        return struct.unpack("<f", self.ser.read(4))
    
    def cell4(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x07])))
        return struct.unpack("<f", self.ser.read(4))
    
    def cell5(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x08])))
        return struct.unpack("<f", self.ser.read(4))

    def cell6(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x09])))
        return struct.unpack("<f", self.ser.read(4))
    
    def temp(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x0A])))
        return struct.unpack("<f", self.ser.read(4))
    
class ESTOP:
    def __init__(self, ser):
        self.ser = ser
        self.adr = 0x12

    def wait(self):
        self.ser.read()

    def reset(self):
        self.ser.write(bytes([self.adr << 1 + 1, 0x08]))
        self.wait()

    def estop(self):
        self.ser.write((bytes([self.adr << 1, 1, 0x01])))
        return self.ser.read(1)[0]

    def mode(self):
        self.ser.write((bytes([self.adr << 1, 1, 0x02])))
        return self.ser.read(1)[0]

    def drive_x(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x03])))
        return struct.unpack("<f", self.ser.read(4))

    def drive_y(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x04])))
        return struct.unpack("<f", self.ser.read(4))

    def conencted(self):
        self.ser.write((bytes([self.adr << 1, 1, 0x05])))
        return self.ser.read(1)[0]

    def rssi(self):
        self.ser.write((bytes([self.adr << 1, 2, 0x04])))
        return struct.unpack("<i", self.ser.read(2))
    
    def snr(self):
        self.ser.write((bytes([self.adr << 1, 2, 0x04])))
        return struct.unpack("<i", self.ser.read(2))

class Buck:
    def __init__(self, ser):
        self.ser = ser
        self.adr = 0x13

    def wait(self):
        self.ser.read()

    def adj1_en(self, value):
        self.ser.write(bytes([self.adr << 1 + 1, 0x06, value]))
        self.wait()
        
    def adj2_en(self, value):
        self.ser.write(bytes([self.adr << 1 + 1, 0x07, value]))
        self.wait()

    def fan1(self, value):
        self.ser.write(bytes([self.adr << 1 + 1, 0x0A, value]))
        self.wait()
    
    def fan2(self, value):
        self.ser.write(bytes([self.adr << 1 + 1, 0x0B, value]))
        self.wait()

    def adj1_voltage(self, value):
        self.ser.write(bytes([self.adr << 1 + 1, 0x08]))
        self.ser.write(struct.pack("<f", value))
        self.wait()

    def adj2_voltage(self, value):
        self.ser.write(bytes([self.adr << 1 + 1, 0x09]))
        self.ser.write(struct.pack("<f", value))
        self.wait()

    def current_5v(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x01])))
        return struct.unpack("<f", self.ser.read(4))

    def current_12v(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x02])))
        return struct.unpack("<f", self.ser.read(4))

    def current_19v(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x03])))
        return struct.unpack("<f", self.ser.read(4))

    def current_adj1(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x04])))
        return struct.unpack("<f", self.ser.read(4))
    
    def current_adj2(self):
        self.ser.write((bytes([self.adr << 1, 4, 0x05])))
        return struct.unpack("<f", self.ser.read(4))

class Mechanisms:
    def __init__(self,ser):
        self.ser = ser
    
    def wait(self):
        self.ser.read()

    def output(self, pin, value):
        self.ser.write(bytes([0x00, 0x01, pin - 1, value]))
        self.wait()

    def angle(self, pin, value):
        self.ser.write(bytes([0x00, 0x02, pin - 1, value]))
        self.wait()

    def direction(self, pin, value):
        self.ser.write(bytes([0x00, 0x03, pin - 1, value]))
        self.wait()       

    def output_h(self, pin, value):
        self.ser.write(bytes([0x00, 0x04, pin - 1, value]))
        self.wait()

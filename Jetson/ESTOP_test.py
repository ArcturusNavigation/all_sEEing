import ArcturusEE
import serial

ser = serial.Serial("COM21", 115200, timeout = 1)

estop = ArcturusEE.ESTOP(ser)

print("Reading Values")
if estop.remote_estop():
    print("REMOTE ESTOP: ON")
else:
    print("REMOTE ESTOP: OFF")
if estop.manual():
    print("DRIVE MODE: MANUAL")
    print("DRIVE X1: %s" % estop.drive_x1)
    print("DRIVE Y1: %s" % estop.drive_y1())
    print("DRIVE X2: %s" % estop.drive_x2())
    print("DRIVE Y2: %s" % estop.drive_y2())
else:
    print("DRIVE MODE: AUTO")
if estop.connected():
    print("CONNECTED: YES")
else:
    print("CONNECTED: NO")
import Arcturus
import serial
import time

ser = serial.Serial("COM1", 115200, timeout = 1)

estop = Arcturus.ESTOP(ser)

print("Resetting ESTOP board")
print("Warning: May trigger ESTOP/UNESTOP")
estop.reset()
time.sleep(5)

print("Reading Values")
if estop.estop():
    print("ESTOP: ON")
else:
    print("ESTOP: OFF")
if estop.mode():
    print("DRIVE MODE: MANUAL")
    print("DRIVE X: %s" % estop.drive_x())
    print("DRIVE Y: %s" % estop.drive_y())
else:
    print("DRIVE MODE: AUTO")
if estop.connected():
    print("CONNECTED: YES")
else:
    print("CONNECTED: NO")
print("RSSI: %s" % estop.rssi())
print("SNR: %s" % estop.snr())

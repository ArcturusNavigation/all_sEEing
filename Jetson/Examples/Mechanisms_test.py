import Arcturus
import serial
import time

ser = serial.Serial("COM16", 115200, timeout = 1)

mech = Arcturus.Mechanisms(ser)

print("Resetting balls launched count")
mech.reset_launched()

print("Setting 12V full power")
mech.set12V(255)
time.sleep(5)

print("Setting 12V half power")
mech.set12V(128)
time.sleep(5)

print("Turning off 12V")
mech.set12V(0)

print("Setting 20V full power")
mech.set20V(255)
time.sleep(5)

print("Setting 20V half power")
mech.set20V(128)
time.sleep(5)

print("Turning off 20V")
mech.set12V(0)

print("Setting Servo 1 45 degrees / half power reverse on a continuous servo")
mech.servo1_angle(45)
time.sleep(5)

print("Turning off Servo 1")
mech.stop_servo1()

print("Setting Servo 2 45 degrees / half power reverse on a continuous servo")
mech.servo2_angle(45)
time.sleep(5)

print("Turning off Servo 2")
mech.stop_servo2()

print("Balls launched: %s" % mech.launched())

if mech.switch1():
    print("Switch 1 open")
else:
    print("Switch 1 closed")

if mech.switch2():
    print("Switch 2 open")
else:
    print("Switch 2 closed")

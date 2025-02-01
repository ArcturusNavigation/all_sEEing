import Arcturus
import serial
import time

ser = serial.Serial("COM16", 115200, timeout = 1)

mech = Arcturus.Mechanisms(ser)
buck = Arcturus.Buck(ser)

print("Resetting balls launched count")
mech.reset_launched()

print("Turning on 12V")
buck.adj1_en(1)
time.sleep(5)

print("Turning off 12V")
buck.adj_en(0)

print("Turning on 20V")
buck.adj2_en(1)
time.sleep(5)

print("Turning off 20V")
buck.ad2_en(0)

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

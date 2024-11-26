import Arcturus
import serial
import time

header = 1 #1 for 5VA, 2 for 5VB, 3 for 12V, 4 for Flex A/ADJ2, 5 for Flex B/ADJ1

mode = 1 #1 for ON/OFF, 2 for Servo, 3 for H-Bridge

ser = serial.Serial("COM16", 115200, timeout = 1)

mech = Arcturus.Mechanisms(ser)

if mode == 1:
    print("ON/OFF Mode")
    print("OUTPUT A ON")
    mech.output(header, 1, 1)
    time.sleep(3)
    print("OUTPUT A OFF\nOUTPUT B ON")
    mech.output(header, 1, 0)
    mech.output(header, 2, 1)
    time.sleep(3)
    print("OUTPUT B OFF")
    mech.output(header, 2, 0)
elif mode == 2:
    print("SERVO Mode")
    mech.setServo(header, 1)
    mech.setServo(header, 2)
    print("SERVO A 75 degrees")
    mech.angle(header, 1, 75)
    print("SERVO B 75 degrees")
    mech.angle(header, 2, 75)
    time.sleep(3)
    print("SERVO A 35 degrees")
    mech.angle(header, 1, 35)
    print("SERVO B 35 degrees")
    mech.angle(header, 2, 35)
else:
    print("H-Bridge Mode")
    print("Forward - ON")
    mech.direction(header, 1)
    mech.output_h(header, 1)
    time.sleep(3)
    print("Forward - OFF")
    mech.output_h(header, 0)
    time.sleep(3)
    print("Reverse - ON")
    mech.direction(header, 0)
    mech.output_h(header, 1)
    time.sleep(3)
    print("Reverse - OFF")
    mech.output_h(header, 0)

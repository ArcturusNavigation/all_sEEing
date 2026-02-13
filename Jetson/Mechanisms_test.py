import ArcturusEE
import serial
import time

ser = serial.Serial("COM4", 115200, timeout = 1)

mechanisms = ArcturusEE.mechanisms(ser)

mechanisms.motor_enable(1)
#mechanisms.pump_enable(0)
time.sleep(3)
mechanisms.motor_enable(0)
#mechanisms.pump_enable(0)
#mechanisms.servo1(90)
#mechanisms.servo2(90)
#time.sleep(3)
#mechanisms.servo2(90)
print(mechanisms.switch1())
print(mechanisms.switch2())
"""
initial = time.time()
while time.time() - initial < 3:
    print("Motor Current: %s A" % mechanisms.motor_current())
    print("Pump Current: %s A" % mechanisms.pump_current())
    time.sleep(.1)
mechanisms.motor_enable(0)
mechanisms.pump_enable(0)
"""
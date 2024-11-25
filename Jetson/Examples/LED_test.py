import Arcturus
import serial
import time

ser = serial.Serial("COM1", 115200, timeout = 1)

led_board = Arcturus.LED(ser)

print("Setting Mulitpurpose LEDs")
for i in range(1,4):
    print("LED %s on" % i)
    led_board.setMPL(i, 1)
    time.sleep(1)
    print("LED %s off" % i)
    led_board.setMPL(i, 0)

print("filling strip with rgb color (255,0,255)")
led_board.fillStrip(255, 0, 255)
time.sleep(2)

print("setting pixel #2 to color (0, 255, 0)")
led_board.idvClr(2, 0, 255, 0)
time.sleep(2)

print("clearing strip")
led_board.clrStrip()

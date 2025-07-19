#!/usr/bin/python

import ArcturusEE
import serial
import time

ser = serial.Serial("/dev/ttyACM0", 115200, timeout = 1)

main_pwr = ArcturusEE.main_power(ser)

def main():
    print("Fans on")
    main_pwr.fan1(1)
    main_pwr.fan2(1)
    
    if main_pwr.estop():
        print("ESTOP ON")
    else:
        print("ESTOP OFF")
    print("Reading currents")
    print("3V3 output: %s A" % main_pwr.current_3v3())
    print("5V output: %s A" % main_pwr.current_5v())
    print("12V output: %s A" % main_pwr.current_12v())
    print("19V output: %s A" % main_pwr.current_19v())
    
    time.sleep(3)
    print("Fans off")
    main_pwr.fan1(0)
    main_pwr.fan2(0)


if __name__ == "__main__":
    main()

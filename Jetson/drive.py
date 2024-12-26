import Arcturus
import serial
import time

ser = serial.Serial("COM16", 115200, timeout = 1) #need to determine port

estop = Arcturus.ESTOP(ser)

def drive(x, y):
    """
    Drive the boat

    Args:
        x (float): between -1 and 1, -1 means joystick all the way left, 0 center, etc.
        y (float): between -1 and 1, -1 means joystick all the way down, 0 center, etc.
    """
    pass #add code to drive the boat

while True:
    if estop.mode(): #boat is in manual mode
        drive(estop.drive_x(), estop.drive_y())
    else: #boat is in automatic mode
        pass #set to automatic mode here
    time.sleep(1)

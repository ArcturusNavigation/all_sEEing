import Arcturus
import serial
import time

ser = serial.Serial("COM1", 115200, timeout = 1)

t1bms = Arcturus.BMS(ser, 0x08) #Thuster battery 1 BMS
t2bms = Arcturus.BMS(ser, 0x09) #Thruster battery 2 BMS
eebms = Arcturus.BMS(ser, 0x0A) #EE battery BMS

print("Turning off BMS outputs")
print("Warning: Using this on EE bms will permanently turn off all electronics on boat")
t1bms.output(0)
t2bms.output(0)
eebms.output(0)
time.sleep(5)

print("Turning on BMS outputs")
print("Warning: This command will override E-STOP on thruster batteries")
t1bms.output(1)
t2bms.output(1)
eebms.output(1)
time.sleep(5)

print("Reading thruster battery #1 values")
print("Battery Voltage: %s V" % t1bms.voltage())
print("Current: %s A" % t1bms.current())
print("Cell 1 Voltage: %s V" % t1bms.cell1)
print("Cell 2 Voltage: %s V" % t1bms.cell2)
print("Cell 3 Voltage: %s V" % t1bms.cell3)
print("Cell 4 Voltage: %s V" % t1bms.cell4)
print("Temperature: %s °F" % t1bms.temp)

print("Reading thruster battery #2 values")
print("Battery Voltage: %s V" % t2bms.voltage())
print("Current: %s A" % t2bms.current())
print("Cell 1 Voltage: %s V" % t2bms.cell1)
print("Cell 2 Voltage: %s V" % t2bms.cell2)
print("Cell 3 Voltage: %s V" % t2bms.cell3)
print("Cell 4 Voltage: %s V" % t2bms.cell4)
print("Temperature: %s °F" % t2bms.temp)

print("Reading EE battery values")
print("Battery Voltage: %s V" % eebms.voltage())
print("Current: %s A" % eebms.current())
print("Cell 1 Voltage: %s V" % eebms.cell1)
print("Cell 2 Voltage: %s V" % eebms.cell2)
print("Cell 3 Voltage: %s V" % eebms.cell3)
print("Cell 4 Voltage: %s V" % eebms.cell4)
print("Cell 5 Voltage: %s V" % eebms.cell5)
print("Cell 6 Voltage: %s V" % eebms.cell6)

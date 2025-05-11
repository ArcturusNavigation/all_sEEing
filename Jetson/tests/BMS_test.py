import ArcturusEE
import serial

ser = serial.Serial("COM16", 115200, timeout = 1)

eebms = ArcturusEE.BMS(ser, 0x02)

print("Reading EE battery values")
if eebms.estop():
    print("ESTOP ON")
else:
    print("ESTOP OFF")
if eebms.output():
    print("OUTPUT ON")
else:
    print("OUTPUT OFF")
print("Battery Voltage: %s cV" % eebms.stack_voltage())
print("Current: %s mA" % eebms.current())
print("Temperature: %s °C" % eebms.temp())
print("Cell 1 Voltage: %s mV" % eebms.cell_voltage(1))
print("Cell 2 Voltage: %s mV" % eebms.cell_voltage(2))
print("Cell 3 Voltage: %s mV" % eebms.cell_voltage(3))
print("Cell 4 Voltage: %s mV" % eebms.cell_voltage(4))
print("Cell 5 Voltage: %s mV" % eebms.cell_voltage(5))
print("Cell 6 Voltage: %s mV" % eebms.cell_voltage(6))
import ArcturusEE
import serial

ser = serial.Serial("COM21", 115200, timeout = 1)

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


thr_a = ArcturusEE.BMS(ser, 0x03)
print("Reading THR A battery values")
if thr_a.estop():
    print("ESTOP ON")
else:
    print("ESTOP OFF")
if thr_a.output():
    print("OUTPUT ON")
else:
    print("OUTPUT OFF")
print("Battery Voltage: %s cV" % thr_a.stack_voltage())
print("Current: %s mA" % thr_a.current())
print("Temperature: %s °C" % thr_a.temp())
print("Cell 1 Voltage: %s mV" % thr_a.cell_voltage(1))
print("Cell 2 Voltage: %s mV" % thr_a.cell_voltage(2))
print("Cell 3 Voltage: %s mV" % thr_a.cell_voltage(3))
print("Cell 4 Voltage: %s mV" % thr_a.cell_voltage(4))

thr_b = ArcturusEE.BMS(ser, 0x04)
print("Reading THR B battery values")
if thr_b.estop():
    print("ESTOP ON")
else:
    print("ESTOP OFF")
if thr_b.output():
    print("OUTPUT ON")
else:
    print("OUTPUT OFF")
print("Battery Voltage: %s cV" % thr_b.stack_voltage())
print("Current: %s mA" % thr_b.current())
print("Temperature: %s °C" % thr_b.temp())
print("Cell 1 Voltage: %s mV" % thr_b.cell_voltage(1))
print("Cell 2 Voltage: %s mV" % thr_b.cell_voltage(2))
print("Cell 3 Voltage: %s mV" % thr_b.cell_voltage(3))
print("Cell 4 Voltage: %s mV" % thr_b.cell_voltage(4))
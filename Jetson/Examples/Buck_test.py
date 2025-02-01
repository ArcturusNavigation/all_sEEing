import Arcturus
import serial
import time

v1 = 13.0
v2 = 20.0

ser = serial.Serial("COM16", 115200, timeout = 1)

buck = Arcturus.Buck(ser)

print("Setting voltage on adjustable converters")
buck.adj1_voltage(v1)
buck.adj2_voltage(v2)

print("Enabling fans/outputs")
buck.adj1_en(1)
buck.adj2_en(1)
buck.fan1(1)
buck.fan2(1)

print("Reading currents")
print("5V output: %s A" % buck.current_5v())
print("12V output: %s A" % buck.current_12v())
print("19V output: %s A" % buck.current_19v())
print("ADJ1 output: %s A" % buck.current_adj1())
print("ADJ2 output: %s A" % buck.current_adj2())

time.sleep(5)
print("Disabling fans/outputs")
buck.adj1_en(0)
buck.adj2_en(0)
buck.fan1(0)
buck.fan2(0)

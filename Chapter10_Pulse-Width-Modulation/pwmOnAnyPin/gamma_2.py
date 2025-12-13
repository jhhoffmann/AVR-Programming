#!/bin/python3

print("uint8_t gamma_2[256] = {")
first = True
for i in range(0, 256):
    space = "  " if first else ""
    first = False
    value = int(i*i/255)
    print(f"{space:s}{value:3d},", end="")
    if i % 16 == 15:
        print("")
        first = True
print("};")

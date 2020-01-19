#!/usr/bin/env python3

import sys
import usb.core

# pip3 install PyUsb


# find USB devices
dev = usb.core.find(find_all=True)

# loop through devices, printing vendor and product ids in decimal and hex
for cfg in dev:
    sys.stdout.write('Decimal VendorID=' + str(cfg.idVendor) + ' & ProductID=' + str(cfg.idProduct) + '\n')
    sys.stdout.write('Hexadecimal VendorID=' + hex(cfg.idVendor) + ' & ProductID=' + hex(cfg.idProduct) + '\n\n')



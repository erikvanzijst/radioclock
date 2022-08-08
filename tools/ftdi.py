#!/usr/bin/env -S python3 -u

import argparse
import sys

from serial import Serial
from serial.tools import list_ports


if __name__ == '__main__':
    parser = argparse.ArgumentParser(sys.argv[0], description='USB Serial client for USB FTDI UART dongle.')
    parser.add_argument('-p', '--port', required=False,
                        help='the serial port (e.g. /dev/cu.usbserial-A10JX2C7); auto-detected when omitted')
    parser.add_argument('--nolog', action='store_true', help='do not write uart.log file')
    args = parser.parse_args()

    dev = args.port
    logfile = None if args.nolog else open('uart.log', 'ab')

    def log(line):
        if type(line) == str:
            line = line.encode("utf-8")
        sys.stdout.buffer.write(line)
        if logfile:
            logfile.write(line)
            logfile.flush()

    if not args.port:
        try:
            dev = next(
                filter(lambda p: p.product == 'FT232R USB UART' or p.manufacturer == 'FTDI',
                       list_ports.comports())).device
            log(f"UART found at {dev}\n")
        except StopIteration:
            log('Cannot find FTDI UART. If it is connected, specify the port manually.\n')
            exit(1)

    board = Serial(port=dev, baudrate=115200)

    while True:
        data = board.read(1)
        if len(data) == 0:
            break
        else:
            log(data)

#!/usr/bin/env python3
#
# Connects to the radioclock PCB over USB-serial UART and reads the output of the PCB's DCF_DATA
# line as output by the firmware as a 0 or 1 per line. Then decodes the time signal.

import argparse
import datetime
import sys
import time

from serial import Serial
from serial.tools import list_ports


def decode(bits):
    if len(bits) != 59 or any(map(lambda e: e is None, bits)):
        raise ValueError('Incomplete bit sequence')
    if bits[0]:
        raise ValueError('Start of minute not 0')
    if sum(bits[21:28]) % 2 != bits[28]:
        raise ValueError('Parity error on minutes')
    if sum(bits[29:35]) % 2 != bits[35]:
        raise ValueError('Parity error on hours')
    if sum(bits[36:58]) % 2 != bits[58]:
        raise ValueError('Parity error on date')

    mins    = 1 * bits[21] + 2 * bits[22] + 4 * bits[23] + 8 * bits[24] + 10 * bits[25] + 20 * bits[26] + 40 * bits[27]
    hours   = 1 * bits[29] + 2 * bits[30] + 4 * bits[31] + 8 * bits[32] + 10 * bits[33] + 20 * bits[34]
    day     = 1 * bits[36] + 2 * bits[37] + 4 * bits[38] + 8 * bits[39] + 10 * bits[40] + 20 * bits[41]
    dow     = 1 * bits[42] + 2 * bits[43] + 4 * bits[44]
    month   = 1 * bits[45] + 2 * bits[46] + 4 * bits[47] + 8 * bits[48] + 10 * bits[49]
    year    = 1 * bits[50] + 2 * bits[51] + 4 * bits[52] + 8 * bits[53] + 10 * bits[54] + 20 * bits[55] + 40 * bits[56] + 80 * bits[57]

    return '20%02d-%02d-%02d %02d:%02d:00' % (year, month, day, hours, mins)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(sys.argv[0], description='DCF77 radio clock test client')
    parser.add_argument('-p', '--port', required=False, help='the serial port connected to the board')
    parser.add_argument('--nolog', action=argparse.BooleanOptionalAction, help='do not write log file')
    args = parser.parse_args()

    dev = args.port
    if not args.port:
        try:
            dev = next(
                filter(lambda p: p.product == 'FT232R USB UART' or p.manufacturer == 'FTDI',
                       list_ports.comports())).device
            print(f"UART found at {dev}")
        except StopIteration:
            print('Cannot find FTDI UART. If it is connected, specify the port manually.')
            exit(1)

    logfile = None if args.nolog else open('dcf.log', 'a')
    board = Serial(port=dev, baudrate=115200, timeout=30)

    def log(line):
        print(line)
        if logfile:
            logfile.write(line)
            logfile.write('\n')

    prev_up, up, down = 0, 0, 0
    i, prev_val, val = 0, 0, 0
    start = 0

    bits = []

    while True:
        try:
            val = bool(int(board.readline().decode('utf-8').strip()))
            now = time.time()
            ts = datetime.datetime.now().strftime('%H:%M:%S.%f')[:-3]

            if not prev_val and val:     # rising edge
                up = now
                period = up - prev_up
                pulse = down - prev_up

                bit = 0 if abs(0.100 - pulse) < 0.04 else 1 if abs(0.200 - pulse) < 0.04 else None
                bits.append(bit)

                err = ''
                if not (abs(1 - period) < 0.1 or abs(2 - period) < 0.1):
                    err = 'period length mismatch'
                elif bit is None:
                    err = 'pulse length mismatch'

                bar = ' #' + '#' * int(pulse * 100) + '.' * int((min(period, 2) - pulse) * 100)
                log(ts + ('%-120s' % bar)[:120] + ' %3d / %4d ms (%s)' % (pulse * 1000, period * 1000, err if err else bit))

                som = abs(period - 2) < 0.1
                if som:
                    try:
                        log(ts + ' Time sync: ' + decode(bits))
                    except ValueError as e:
                        log(str(e))
                    bits.clear()

                prev_up = up

            if prev_val and not val:    # falling edge
                down = now

                # if not (i % 20):
                #     log('0  %9d %9d %9d %9d %9d %9d %9d %9d %9d %9dms' %
                #           (100, 200, 300, 400, 500, 600, 700, 800, 900, 1000))
                #     log('#', '---------#' * 10, sep='')
                #
                # log('#', '#' * int(pulse * 100), ' %dms' % (pulse * 1000), sep='')
                i += 1
            prev_val = val
        except ValueError:
            pass

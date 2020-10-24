# Run with python3 osc-serial.py /dev/ttyACM0 | ./osc-sdl
import os
import select
import serial
import struct
import sys
import threading
import time
import tty
import termios
import fcntl

if len(sys.argv) != 2:
    print("invalid argument")
    exit(1)

tty_path = sys.argv[1]

baudrate = 230400

try:
    while True:
        try:
            with serial.Serial(tty_path, baudrate) as ser:
                while True:
                    b = ser.read(1)[0]
                    if b == 0xff:
                        value = ser.read(1)[0]
                        t = ser.read(1)[0]
                        print("%d %d" % (t, value))

                    #  _, value, t = struct.unpack('BBH', data)
                    #  print(t, value)
        except serial.serialutil.SerialException as e:
            print("error: " + str(e))
            print("restarting serial port")
            time.sleep(5)
finally:
    pass
    # termios.tcsetattr(fd, termios.TCSADRAIN, old_settings

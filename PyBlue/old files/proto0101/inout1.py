import serial
import time
import threading
import signal
from pyo import *

# CONSTANTS

Serialname = '/dev/cu.usbmodem1d1121'
BaudRate = 9600

outputFS = 44100
outputT = 0.00001
inputT = 0.00001
inputFS = 8000

GAIN = 0.001023 #normalise input to range 0-1

loop = 1
run = 0
runend = 5000

# VARIABLES

inbuff = [0]*10

# START

stream = Server(sr=inputFS, nchnls=2).boot()
stream.start()

ser = serial.Serial(Serialname, BaudRate)
print 'Connected to Serial'

while(loop):
    
    #Read Data
    input = ser.readline()
    try:
        inbuff.append(float(int(input))*GAIN)
    except ValueError:
        pass
    
    #Write Data
    if len(inbuff) >= 1:
        out = Sig(inbuff[0]).out()
        #print out.value #debugging
        inbuff = inbuff[1:]

    #Manage While Loop
    run=run+1
    if run>=runend:
        loop = 0

#stream.gui(locals())
#time.sleep(10)

stream.stop()

print 'End Program'

import serial
import time
import threading
import signal
import wx
from pyo import *

# CONSTANTS

Serialname = '/dev/cu.usbmodem1d1121'
BaudRate = 9600

outputFS = 44100
outputT = 0.00001
inputT = 0.00001
inputFS = 8000

GAIN = 0.001023 #normalise input to range 0-1

drumsamp = "/Users/kartikgohil/Documents/Imperial/Year4/Project/Test Audio/afrimid.wav"

loop = 1
run = 0
runend = 1000

# VARIABLES

inbuff = []

# INITIALISE

stream = Server(sr=outputFS, nchnls=2).boot()
stream.start()

ser = serial.Serial(Serialname, BaudRate)
print 'Connected to Arduino [Serial]'

# FUNCTIONS

def play_drum():
    return SfPlayer(drumsamp)

# START

#sf = play_drum().out()
#sf = SfPlayer(drumsamp).out()
#print 'Play'

while(loop):
    
    #Read Data
    input = ser.readline()
    #print input #debugging
    try:
        inbuff.append(Sig(float(int(input))*GAIN))
    except ValueError:
        pass
    
    #Write Data
    if len(inbuff)>=20:
        trig = AttackDetector(inbuff,deltime=inputT,cutoff = 500, maxthresh=1, minthresh=0)
        print 'Scope'
        scope = Scope(inbuff)
        #print TrigVal(trig,value=0)
        #sf = TrigFunc(trig,play_drum).out()
        inbuff = []

    #Manage While Loop
    run=run+1
    if run>=runend:
        loop = 0

#stream.gui(locals())
#time.sleep(10)

stream.stop()

print 'End Program'

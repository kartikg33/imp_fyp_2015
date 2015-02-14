import serial
import time
import threading
import signal
import wx
from pyo import *

# CONSTANTS

Serialname = '/dev/cu.usbmodem1d1121'
BaudRate = 115200

outputFS = 44100
outputT = 0.00001
inputT = 0.00001
inputFS = 8000

GAIN = 0.001023 #normalise input to range 0-1

drumsamp = "/Users/kartikgohil/Documents/Imperial/Year4/Project/Test Audio/afrimid.wav"

loop = 1
run = 0
runend = 3000

# VARIABLES

inbuff = []


# INITIALISE

stream = Server(sr=outputFS, nchnls=2).boot()
stream.start()

ser = serial.Serial(Serialname, BaudRate)
print 'Connected to Arduino [Serial]'

# FUNCTIONS



# START

#sf = play_drum().out()
#sf = SfPlayer(drumsamp).out()
#print 'Play'

readytoplay=1

while(loop):
    
    for value in range(0,10):
    #Read Data
        input = ser.readline()
    #print input #debugging
        try:
            inbuff.append(Sig(float(int(input))*GAIN))
        except ValueError:
            pass
        
    
    #Write Data
    trig = [1 for value in inbuff if value.value >= 0.65]
    # if trig is all ones and readytoplay, play and make not readytoplay
    if 1 in trig and readytoplay:
        readytoplay = 0
        sf = SfPlayer(drumsamp).out()
    # if trig empty and not readtoplay, make readytoplay
    if not trig and not readytoplay:
        readytoplay = 1

    #Reset Buffer
    inbuff = []

    #Manage While Loop
    run=run+1
    if run>=runend:
        loop = 0

#stream.gui(locals())
#time.sleep(10)

stream.stop()

print 'End Program'

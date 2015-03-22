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
inbuffmax = 0


# INITIALISE

stream = Server(sr=outputFS, nchnls=2).boot()
stream.start()

ser = serial.Serial(Serialname, BaudRate)
print 'Connected to Arduino [Serial]'

# FUNCTIONS

def play_drum():
    print 'Play'
    return SfPlayer(drumsamp)

# START

#sf = play_drum().out()
#sf = SfPlayer(drumsamp).out()
#print 'Play'
readytoplay = 1
while(loop):
    
    #Read Data
    input = ser.readline()
    #print input #debugging
    try:
        temp = Sig(float(int(input)))*GAIN
        inbuff.append(temp)
        if temp>inbuffmax:
            inbuffmax=temp
        if temp<=0.3 and not readytoplay:
            inbuffmax = 0
            readytoplay = 1
    except ValueError:
        pass
    
    #Write Data
    if inbuffmax>=0.7 and readytoplay:
        #play_drum()
        sf = play_drum().out()
        readytoplay = 0


    if len(inbuff)>=10:
        #        print [value.value for value in inbuff if value.value >= 0.7]
        #trig = Thresh(inbuff,threshold=0.7,dir=0)
        #trig2 = Thresh(trig,threshold=0.5,dir=0)
        #print [value for value in trig if value >= 0.5]
        #sf = TrigFunc(trig,play_drum)
        #sf.out()
        inbuff = inbuff[1:]

    #Manage While Loop
    run=run+1
    if run>=runend:
        loop = 0

#stream.gui(locals())
#time.sleep(10)

stream.stop()

print 'End Program'

import serial
import time
import threading
import signal
from pyo import *

outputFS = 44100
outputT = 0.00001
inputT = 0.00001
inputFS = 8000

inbuff = [0]*10

##################################################
def read_data():
    global tread, inbuff
    input = ser.readline()
    try:
        inbuff.append(int(input))
    except ValueError:
        PyErr_SetString(PyExc_ValueError,'NULL')
        inbuff.append(0)
        #PyErr_Clear()
        #pass

    tread = threading.Timer(inputT, read_data)
    tread.start()

##################################################

##################################################
def play_data():
    global tplay, inbuff
    if len(inbuff) >= 1:
        out = Sig(inbuff[0],mul=1).out()
        print out.value
    else:
        out = Sig(0,mul=1).out()
        print out.value
    inbuff = inbuff[1:]
    tplay = threading.Timer(outputT, play_data)
    tplay.start()

##################################################

stream = Server(sr = inputFS).boot()
stream.start()

ser = serial.Serial('/dev/cu.usbmodem1d1121', 9600)
print 'Connected to Serial'

#tread = threading.Timer(inputT, read_data)
#tread.start()
#tplay = threading.Timer(outputT, play_data)
#tplay.start()

loop = 1
run = 0
runend = 10000
while(loop):
    run=run+1
    if run>=runend:
        loop = 0

stream.gui(locals())
#time.sleep(10)


tread.cancel()
tplay.cancel()
stream.stop()



print 'End Program'

import serial
import time
import pyaudio
import wave
import threading
import signal




wf = wave.open("/Users/kartikgohil/Documents/Imperial/Year4/Project/Test Audio/afrimid.wav", 'rb')



# define callback (2)
def callback(in_data, frame_count, time_info, status):
    data = wf.readframes(frame_count)
    return (data, pyaudio.paContinue)

def callback2(in_data, frame_count, time_info, status):
    global inbuff
    outbuff = inbuff[0:int(outputT*inputFS)]
    inbuff = inbuff[int(outputT*inputFS):]
    #print outbuff
    #data = wf.readframes(frame_count)
    return (str(outbuff).strip('[]'), pyaudio.paContinue)

#stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
#channels=wf.getnchannels(),
#               rate=wf.getframerate(),
#               output=True,
#               stream_callback=callback)

inputFS = 8000
inputT = 0.000125
outputFS = 44100
outputT = 0.05
playsnare = 0.5
programrun = 10

##################################################
def play_drum():
    global tdrum
    # instantiate PyAudio (1)
    p = pyaudio.PyAudio()
    # open stream using callback (3)
    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True,
                    stream_callback=callback)
    # start the stream (4)
    stream.start_stream()
    print 'Play'
    wf.rewind()

##################################################


##################################################
def read_data():
    global tread
    input = ser.readline()
    inbuff.append(input)
    #print input
    tread = threading.Timer(inputT, read_data)
    tread.start()

##################################################


##################################################
def play_data():
    global tout
    # instantiate PyAudio (1)
    p = pyaudio.PyAudio()
    # open stream using callback (3)
    stream = p.open(format=pyaudio.paInt16,
                    channels=2,
                    rate=inputFS,
                    output=True,
                    #frames_per_buffer = 8000,
                    stream_callback=callback2)
    # start the stream (4)
    stream.start_stream()
    #print 'Play'
    #wf.rewind()
    tout = threading.Timer(outputT,play_data)
    tout.start()
##################################################


ser = serial.Serial('/dev/cu.usbmodem1d1121', 9600)
print 'Connected to Serial'

inbuff = []


tread = threading.Timer(inputT, read_data)
tread.start()
tout = threading.Timer(outputT,play_data)
tout.start()

loop = 1
run = 0
runend = 1000
    #while(loop):
    #input = ser.readline()
    # inbuff.append(ser.readline())
    
    #  run=run+1
    #if run>=runend:
#    loop = 0

time.sleep(programrun)

tread.cancel()
tout.cancel()

# stop stream (6)
#stream.stop_stream()
#stream.close()

# close waveform and PyAudio (7)
wf.close()
#p.terminate()

print 'End Program'



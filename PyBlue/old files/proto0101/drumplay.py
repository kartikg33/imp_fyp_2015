import serial
import time
import pyaudio
import wave
import threading




wf = wave.open("/Users/kartikgohil/Documents/Imperial/Year4/Project/Test Audio/afrimid.wav", 'rb')



# define callback (2)
def callback(in_data, frame_count, time_info, status):
    data = wf.readframes(frame_count)
    return (data, pyaudio.paContinue)

#stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
#channels=wf.getnchannels(),
#               rate=wf.getframerate(),
#               output=True,
#               stream_callback=callback)

inputFS = 8000
inputT = 0.000125
outputFS = 44100
outputT = 0.00002268
playsnare = 0.5
programrun = 10


##################################################
def read_data():
    global inbuff
    inbuff.append(ser.readline())
    #print ser.readline()
    tread = threading.Timer(inputT, read_data)
    tread.start()

##################################################

##################################################
def play_drum():
    global tdrum, wf
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
    tdrum = threading.Timer(playsnare, play_drum)
    tdrum.start()
##################################################



#ser = serial.Serial('/dev/tty.usbmodem1a1221', 9600)
print 'Connected to Serial'

inbuff = []

#tread = threading.Timer(inputT, read_data)
#tread.start()
tdrum = threading.Timer(playsnare, play_drum)
tdrum.start()


time.sleep(programrun)

tdrum.cancel()

# stop stream (6)
#stream.stop_stream()
#stream.close()

# close waveform and PyAudio (7)
wf.close()
#p.terminate()

print 'End Program'



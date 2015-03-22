import wx
import serial
import glob
import time
import threading
import multiprocessing as mp
import signal
from pyo import *


# CONSTANTS

Serialname = '/dev/tty.usbmodem1a1221'
BaudRate = 115200


FSard = 32000
FSdrum = 44100

GAIN = 0.0009775 #normalise input to range -1:1
drumsamp = "/Users/kartikgohil/Documents/Imperial/Year4/Project/Test Audio/afrimid.wav"

mainframex = 400
mainframey = 200

# VARIABLES

inbuff = []
ser = []
VoiceFl = []
SampleFl = []
workers = []

################################################################################

#SERIAL PORTS
#http://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
def listserial():
    ports = glob.glob('/dev/tty.*')
    result = ['Select Serial Port']
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    result.append('NULL')
    return result

############### END SERIAL PORTS ###############

#VOICE FUNCTION
def voicefunc(bus):
    global inbuff
    print 'Voice Function %s' %bus
    #print bus
    delay = 0
    inbuff = []
    #t = threading.Thread(target=voiceout,args=(bus,delay,))
    #t.daemon = True
    #t.start()
    while(VoiceFl[bus] and ser[bus]!='NULL'):
        try:
            input = ser[bus].readline()
            try:
                #inbuff.append(Sig((int(input)-512)*GAIN))
                a=(Sig((int(input)-512)*GAIN)).out()
                #a.out()
                #print a.value
            except ValueError:
                pass
        except serial.SerialException:
            pass
    print 'End Voice Function %s' %bus

def voiceout(bus,delay):
    global inbuff
    while(VoiceFl[bus]):
        N = len(inbuff)
        for i in range(0,N):
            inbuff[i].out()
            #print inbuff[i].value
            #print 'finished printing'
        inbuff = inbuff[N:]

        time.sleep(delay)
    print 'End VoiceOut %s' %bus
############### END VOICE FUNCTION ###############

#SAMPLE FUNCTION
def samplefunc(bus):
    global inbuff
    N = 1000
    inbuff = []
    #print inbuff
    readytoplay=1
    print 'Sample Function %s' %bus
    t = threading.Thread(target=auIn,args=(bus,))
    t.daemon = True
    t.start()
    while(SampleFl[bus] and ser[bus]!='NULL'):
        #Write Data
        #print [(i.value) for i in inbuff]
        trig = [1 for index in inbuff[0:N] if index.value >= 0.2]
        # if trig is all ones and readytoplay, play and make not readytoplay
        if 1 in trig and readytoplay:
            sf = SfPlayer(drumsamp).out()
            readytoplay = 0
        # if trig empty and not readtoplay, make readytoplay
        if not trig and not readytoplay:
            readytoplay = 1
        #pop used samples
        inbuff = inbuff[N:]
    print 'End Sample Function %s' %bus

def auIn(bus):
    global inbuff
    while(SampleFl[bus]):
        try:
        #Read Data
            input = ser[bus].readline()
        #print input #debugging
            try:
                inbuff.append(Sig((int(input)-512)*GAIN))
            except ValueError:
                pass
        except serial.SerialException:
            pass
    print 'End AudioIn %s' %bus

############### END SAMPLE FUNCTION ###############
#BUS FUNCTION

def OpenBus(bus):
    global ser, VoiceFl, SampleFl, workers
    if len(ser)-1 < bus:
        ser.append('NULL')
        VoiceFl.append(0)
        SampleFl.append(0)
        workers.append('NULL')
    else:
        ser[bus] = 'NULL'
        VoiceFl[bus] = 0
        SampleFl[bus] = 0
        workers[bus] = 'NULL'
    print 'opened %s' %bus
#   print ser
#   print VoiceFl
#   print SampleFl

def CloseBus(bus):
    global ser, VoiceFl, SampleFl,workers
    if ser[bus] != 'NULL':
        ser[bus].close()
    ser[bus] = 'NULL'
    VoiceFl[bus] = 0
    SampleFl[bus] = 0
    print 'closed %s' %bus
#print ser
#   print VoiceFl
#   print SampleFl

############### END BUS FUNCTION ###############

#MAINFRAME CLASS
class Mainframe(wx.Frame):
    
    def __init__(self, *args, **kw):
        super(Mainframe, self).__init__(*args, **kw)
        panel = MyPanel(self)
        
        self.Bind(wx.EVT_CLOSE, self.OnQuit)
        self.Bind(wx.EVT_MENU, self.OnQuit, id=wx.ID_EXIT)
        self.Bind(wx.EVT_MENU, self.OnAbout, id=wx.ID_ABOUT)
        
        self.menu_bar  = wx.MenuBar()
        self.file_menu = wx.Menu()
        self.help_menu = wx.Menu()
        
        self.file_menu.Append(wx.ID_EXIT,"&Quit")
        self.help_menu.Append(wx.ID_ABOUT, "&About PAWS")
        
        self.menu_bar.Append(self.file_menu, "&File")
        self.menu_bar.Append(self.help_menu, "&Help")
        self.SetMenuBar(self.menu_bar)
        
        self.fSizer = wx.BoxSizer(wx.VERTICAL)
        self.fSizer.Add(panel, 1, wx.EXPAND)
        self.SetSizer(self.fSizer)
        self.Centre()
        #self.Fit()
        self.SetMinSize((mainframex,mainframey))
        self.SetTitle('PAWS')
        self.Show()
        #stream.gui()

    def OnQuit(self,e):
        e.Skip()
        print len(ser)
        for bus in range(0,len(ser)):
            CloseBus(bus)
        #time.sleep(0.01)
        self.Destroy()
    
    def OnAbout(self,e):
        print 'About'

class MyPanel(wx.Panel):
    
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.numBoards = 0
        self.frame = parent
        
        self.mainSizer = wx.BoxSizer(wx.VERTICAL)
        self.widgetSizer = wx.BoxSizer(wx.VERTICAL)
        self.boardSizer = []
        topSizer = wx.BoxSizer(wx.HORIZONTAL)
        
        addbtn = wx.Button(self, label='Add PAWS Board')
        addbtn.Bind(wx.EVT_BUTTON, self.AddBoard)
        topSizer.Add(addbtn, 0, wx.CENTER|wx.ALL, 5)
        
        rembtn = wx.Button(self, label='Remove PAWS Board')
        rembtn.Bind(wx.EVT_BUTTON, self.RemBoard)
        topSizer.Add(rembtn, 0, wx.CENTER|wx.ALL, 5)
        
        self.mainSizer.Add(topSizer, 0, wx.CENTER)
        self.mainSizer.Add(self.widgetSizer, 0, wx.CENTER)
        self.SetSizer(self.mainSizer)
    
    
    def RemBoard(self,e):
        if self.widgetSizer.GetChildren():
            bus=self.numBoards-1
            CloseBus(bus)
            self.widgetSizer.Hide(bus)
            self.widgetSizer.Remove(bus)
            self.boardSizer = self.boardSizer[0:bus]
            self.numBoards -= 1
            self.frame.fSizer.Layout()
            self.frame.Fit()

    def AddBoard(self,e):
        bus = int(self.numBoards)
        self.numBoards += 1
        OpenBus(bus)
        self.boardSizer.append(wx.BoxSizer(wx.VERTICAL))

        name = "serial%s" % bus
        serialselect = wx.ComboBox(self,name=name, choices=listserial(),style=wx.CB_READONLY)
        serialselect.Bind(wx.EVT_COMBOBOX, self.SelectBus)
        self.boardSizer[bus].Add(serialselect, 0, wx.ALL, 5)

        self.widgetSizer.Add(self.boardSizer[bus], 0, wx.CENTER)
        self.frame.fSizer.Layout()
        self.frame.Fit()
    
    def SelVoice(self,e):
        global VoiceFl, SampleFl
        select=e.GetEventObject()
        name = select.GetName()
        bus = int(name[-1])
        print name #debugging
        VoiceFl[bus] = 1
        SampleFl[bus] = 0
        print 'Voice'
        t = threading.Thread(target=voicefunc,args=(bus,))
        t.daemon = True
        t.start()

    
    def SelSample(self,e):
        global VoiceFl, SampleFl
        select=e.GetEventObject()
        name = select.GetName()
        bus = int(name[-1])
        print name #debugging
        VoiceFl[bus] = 0
        SampleFl[bus] = 1
        print 'Sample'
        t = threading.Thread(target=samplefunc,args=(bus,))
        t.daemon = True
        t.start()

    
    def SelFunc(self, e):
        global VoiceFl, SampleFl, workers
        select=e.GetEventObject()
        name = select.GetName()
        bus = int(name[-1])
        print name #debugging
        if name[0] == 'v':
            VoiceFl[bus] = 1
            SampleFl[bus] = 0
            print 'Voice'
            workers[bus] = threading.Thread(target=voicefunc,args=(bus,))
            workers[bus].daemon = True
        elif name[0] == 's':
            VoiceFl[bus] = 0
            SampleFl[bus] = 1
            print 'Sample'
            workers[bus] = threading.Thread(target=samplefunc,args=(bus,))
            workers[bus].daemon = True
        else:
            VoiceFl[bus] = 0
            SampleFl[bus] = 0
            print 'Voice By Default'
            workers[bus] = threading.Thread(target=voicefunc(bus))
            workers[bus].daemon = True
        workers[bus].start()

    
    def SelectBus(self, e):
        global ser, VoiceFl, SampleFl, workers
        select=e.GetEventObject()
        name=select.GetName()
        bus = int(name[-1])
        Serialname = e.GetString()
        if ser[bus] != 'NULL':
            ser[bus].close()
        try:
            ser[bus] = serial.Serial(Serialname, BaudRate)
            print 'Connected to Arduino [Serial Bus %s]' %bus
            busSizer = wx.BoxSizer(wx.HORIZONTAL)
        
            name = "voice%s" % bus
            voiceselect = wx.RadioButton(self,name=name, label='Voice', style=wx.RB_GROUP)
            voiceselect.Bind(wx.EVT_RADIOBUTTON, self.SelFunc)
        
            name = "sample%s" % bus
            sampleselect = wx.RadioButton(self,name=name, label='Sample')
            sampleselect.Bind(wx.EVT_RADIOBUTTON, self.SelFunc)
        
            busSizer.Add(voiceselect, 0, wx.ALL, 5)
            busSizer.Add(sampleselect, 0, wx.ALL, 5)
        
            self.boardSizer[bus].Add(busSizer, 0, wx.CENTER)
            self.frame.fSizer.Layout()
            self.frame.Fit()
        except OSError:
            ser[bus] = 'NULL'
            VoiceFl[bus] = 0
            SampleFl[bus] = 0
            workers[bus] = 'NULL'
            print 'Failed to connect to Arduino [Serial Bus %s]' %bus
            pass
        


############### END MAINFRAME CLASS ###############


# START
stream = Server(sr=FSard, nchnls=2,buffersize=3200).boot()
stream.start()

app = wx.App()
frame = Mainframe(None)
app.MainLoop()

stream.stop()
print 'End Program'

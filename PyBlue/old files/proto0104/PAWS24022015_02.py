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

GAIN = 0.001023 #normalise input to range 0-1
drumsamp = "/Users/kartikgohil/Documents/Imperial/Year4/Project/Test Audio/afrimid.wav"

mainframex = 400
mainframey = 200

# VARIABLES

inbuff = []
ser = []
VoiceFl = []
SampleFl = []

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


#VOICE FUNCTION
def voicefunc(bus):
    print 'Voice Function %s' %bus
    #print bus
    while(VoiceFl[bus] and ser[bus]!='NULL'):
        #print 'Inside While'
        try:
            input = ser[bus].readline()
            try:
                a=Sig(int(input)*GAIN)
                a.out()
                print a.value
            except ValueError:
                pass
        except serial.SerialException:
            pass
    print 'End Voice Function %s' %bus

def voicethread2(bus):
    print 'Voice Function %s' %bus
    #print bus
    N = 512
    buff = mp.Array('d',[0 for x in range(N)])
    while(VoiceFl[bus] and ser[bus]!='NULL'):
        #print 'Inside While'
        p = mp.Process(target = voicefunc2,args=(bus,buff,))
        p.start()
        p.join()
        #Write Data
        for i in range(0,len(buff)):
            #buff2.append(Sig(buff[i]))
            Sig(buff[i]).out()
            print buff[i] #debugging
        #buff2.play()
    print 'End Voice Function %s' %bus


def voicefunc2(bus,inbuff):
    #print 'Voice Read %s' %bus
    for i in range(0,len(inbuff)):
        #print i
        #Read Data
        try:
            input = ser[bus].readline()
            try:
                inbuff[i]=(int(input)*GAIN)
                #inbuff[i]=(int(input))
            except ValueError:
                pass
        except serial.SerialException:
            pass
#print 'End Voice Read %s' %bus


def voicefunc(bus):
    #global inbuff
    inbuff = []
    print 'Voice Function %s' %bus
    while(VoiceFl[bus] and ser[bus]!='NULL'):
        #Read Data
        try:
            input = ser[bus].readline()
            try:
                inbuff.append(int(input)*GAIN)
            except ValueError:
                pass
        except serial.SerialException:
            pass
        #Write Data
        if len(inbuff) >= 1:
            out = Sig(inbuff[0]).out()
            #print out.value #debugging
            inbuff = inbuff[1:]
    print 'End Voice Function %s' %bus

############### END VOICE FUNCTION ###############

#SAMPLE FUNCTION
def samplefunc(bus):
    #global inbuff
    inbuff = []
    readytoplay=1
    print 'Sample Function %s' %bus
    
    while(SampleFl[bus] and ser[bus]!='NULL'):
        for value in range(0,10):
            try:
                #Read Data
                input = ser[bus].readline()
                #print input #debugging
                try:
                    inbuff.append(Sig(int(input)*GAIN))
                except ValueError:
                    pass
            except serial.SerialException:
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

    print 'End Sample Function %s' %bus

############### END SAMPLE FUNCTION ###############
#BUS FUNCTION

def OpenBus(bus):
    global ser, VoiceFl, SampleFl
    if len(ser)-1 < bus:
        ser.append('NULL')
        VoiceFl.append(0)
        SampleFl.append(0)
    else:
        ser[bus] = 'NULL'
        VoiceFl[bus] = 0;
        SampleFl[bus] = 0;
    print 'opened %s' %bus
#   print ser
#   print VoiceFl
#   print SampleFl

def CloseBus(bus):
    global ser, VoiceFl, SampleFl
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
        self.SetSize((mainframex,mainframey))
        self.SetTitle('PAWS')
        self.Show()


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
            self.numBoards -= 1
            self.frame.fSizer.Layout()
            self.frame.Fit()

    def AddBoard(self,e):
        self.numBoards += 1
        bus=int(self.numBoards-1)
        OpenBus(bus)
        boardSizer = wx.BoxSizer(wx.HORIZONTAL)
        
        name = "serial%s" % bus
        serialselect = wx.ComboBox(self,name=name, choices=listserial(),style=wx.CB_READONLY)
        serialselect.Bind(wx.EVT_COMBOBOX, self.SelectBus)
        
        name = "voice%s" % bus
        voiceselect = wx.RadioButton(self,name=name, label='Voice', style=wx.RB_GROUP)
        voiceselect.Bind(wx.EVT_RADIOBUTTON, self.SelVoice)
        
        name = "sample%s" % bus
        sampleselect = wx.RadioButton(self,name=name, label='Sample')
        sampleselect.Bind(wx.EVT_RADIOBUTTON, self.SelSample)
        
        boardSizer.Add(serialselect, 0, wx.ALL, 5)
        boardSizer.Add(voiceselect, 0, wx.ALL, 5)
        boardSizer.Add(sampleselect, 0, wx.ALL, 5)

        self.widgetSizer.Add(boardSizer, 0, wx.CENTER)
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
        global VoiceFl, SampleFl
        select=e.GetEventObject()
        name = select.GetName()
        bus = int(name[-1])
        print name #debugging
        if name[0] == 'v':
            VoiceFl[bus] = 1
            SampleFl[bus] = 0
            print 'Voice'
            t = threading.Thread(target=voicefunc(bus))
            t.daemon = True
        elif name[0] == 's':
            VoiceFl[bus] = 0
            SampleFl[bus] = 1
            print 'Sample'
            t = threading.Thread(target=samplefunc(bus))
            t.daemon = True
        else:
            VoiceFl[bus] = 0
            SampleFl[bus] = 0
            print 'Voice By Default'
            t = threading.Thread(target=voicefunc(bus))
            t.daemon = True
        t.start()
    
    def SelectBus(self, e):
        global ser, VoiceFl, SampleFl
        select=e.GetEventObject()
        name=select.GetName()
        bus = int(name[-1])
        Serialname = e.GetString()
        if ser[bus] != 'NULL':
            ser[bus].close()
        try:
            ser[bus] = serial.Serial(Serialname, BaudRate)
            print 'Connected to Arduino [Serial Bus %s]' %bus
        except OSError:
            ser[bus] = 'NULL'
            VoiceFl[bus] = 0
            SampleFl[bus] = 0
            print 'Failed to connect to Arduino [Serial Bus %s]' %bus
            pass

############### END MAINFRAME CLASS ###############


# START
stream = Server(sr=FSard, nchnls=2).boot()
stream.start()

app = wx.App()
frame = Mainframe(None)
app.MainLoop()

stream.stop()
print 'End Program'

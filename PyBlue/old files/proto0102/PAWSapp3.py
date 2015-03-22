import wx
import serial
import time
import threading
import signal
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

mainframex = 400
mainframey = 200

# VARIABLES

inbuff = []
ser = 'NULL'
loop = 1
VoiceFl = 0
SampleFl = 0

################################################################################


#VOICE FUNCTION
def voicefunc():
    global inbuff
    if ser == 'NULL':
        return
    print 'Voice Function'

    while(VoiceFl):
        #Read Data
        input = ser.readline()
        try:
            inbuff.append(int(input)*GAIN)
        except ValueError:
            pass
        
        #Write Data
        if len(inbuff) >= 1:
            out = Sig(inbuff[0]).out()
            #print out.value #debugging
            inbuff = inbuff[1:]

############### END VOICE FUNCTION ###############

#SAMPLE FUNCTION
def samplefunc():
    global inbuff
    if ser == 'NULL':
        return
    readytoplay=1
    print 'Sample Function'
    
    while(SampleFl):
        for value in range(0,10):
            #Read Data
            input = ser.readline()
            #print input #debugging
            try:
                inbuff.append(Sig(int(input)*GAIN))
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

############### END SAMPLE FUNCTION ###############

#MAINFRAME CLASS
class Mainframe(wx.Frame):
    
    def __init__(self, *args, **kw):
        super(Mainframe, self).__init__(*args, **kw)
        self.InitUI()
    
    def InitUI(self):
        
        pnl = wx.Panel(self)
        self.Bind(wx.EVT_CLOSE, self.OnClose)
        self.Bind(wx.EVT_MENU, self.OnAbout, id=wx.ID_ABOUT)
        self.Bind(wx.EVT_MENU, self.OnClose, id=wx.ID_EXIT)
        
        self.menu_bar  = wx.MenuBar()
        self.file_menu = wx.Menu()
        self.help_menu = wx.Menu()

        self.file_menu.Append(wx.ID_EXIT,"&Quit")
        self.help_menu.Append(wx.ID_ABOUT, "&About PAWS")

        self.menu_bar.Append(self.file_menu, "&File")
        self.menu_bar.Append(self.help_menu, "&Help")
        self.SetMenuBar(self.menu_bar)
        
        cbtn = wx.Button(pnl, label='Quit', pos=(mainframex-100, mainframey-50))
        cbtn.Bind(wx.EVT_BUTTON, self.OnClose)
        
        self.rbv = wx.RadioButton(pnl, label='Voice', pos=(10, 30),style=wx.RB_GROUP)
        self.rbv.Bind(wx.EVT_RADIOBUTTON, self.SelFunc)
        
        self.rbs = wx.RadioButton(pnl, label='Sample', pos=(10, 50))
        self.rbs.Bind(wx.EVT_RADIOBUTTON, self.SelFunc)
        
        serialdisp = wx.StaticText(pnl, label=Serialname, style=wx.ALIGN_CENTRE)

        
        self.SetSize((mainframex, mainframey))
        self.SetTitle('PAWS')
        self.Centre()
        self.Show(True)
    
    def SelFunc(self, e):
        global VoiceFl, SampleFl
        if self.rbv.GetValue():
            VoiceFl = 1
            SampleFl = 0
            print 'Voice'
            t = threading.Thread(target=voicefunc)
            t.daemon = True
        elif self.rbs.GetValue():
            VoiceFl = 0
            SampleFl = 1
            print 'Sample'
            t = threading.Thread(target=samplefunc)
            t.daemon = True
        else:
            VoiceFl = 1
            SampleFl = 0
            print 'Voice by default'
            t = threading.Thread(target=voicefunc)
            t.daemon = True
        t.start()
    
    def OnClose(self, e):
        global loop
        e.Skip()
        loop = 0
        time.sleep(0.01)
        self.Destroy()

    def OnAbout(self,e):
        print 'About'

############### END MAINFRAME CLASS ###############

#RETURN SERIAL
def returnbus():
    ser = 'NULL'
    try:
        ser = serial.Serial(Serialname, BaudRate)
    except OSError:
        VoiceFl = 0
        SampleFl = 0
        print 'Failed to connect to Arduino [Serial]'
        pass
    return ser

############### END RETURN SERIAL ###############



#MAIN FUNCTION
def main():
    global ser
    stream = Server(sr=inputFS, nchnls=2).boot()
    stream.start()
    
    ser = returnbus()
    if ser != 'NULL':
        print 'Connected to Arduino [Serial]'
    
    while(loop):
        stuff=1

    if ser != 'NULL':
        ser.close()
    stream.stop()
    print 'End Program'

############### END MAIN FUNCTION ###############

# START
t = threading.Thread(target=main)
t.daemon = True
frame = wx.App()
Mainframe(None)
t.start()
frame.MainLoop()

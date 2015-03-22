import wx
import serial
import glob
import time
import threading
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
ser = 'NULL'
VoiceFl = 0
SampleFl = 0

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
    return result

#VOICE FUNCTION
def voicefunc():
    global inbuff
    if ser == 'NULL':
        return
    print 'Voice Function'

    while(VoiceFl):
        #Read Data
        try:
            input = ser.readline()
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
    print 'End Voice Function'

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
            try:
                #Read Data
                input = ser.readline()
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

    print 'End Sample Function'

############### END SAMPLE FUNCTION ###############

#MAINFRAME CLASS
class Mainframe(wx.Frame):
    
    def __init__(self, *args, **kw):
        super(Mainframe, self).__init__(*args, **kw)
        self.InitUI()
    
    def InitUI(self):
        
        pnl = wx.Panel(self)
        
        
        self.Bind(wx.EVT_CLOSE, self.OnQuit)
        self.Bind(wx.EVT_MENU, self.OnQuit, id=wx.ID_EXIT)
        self.Bind(wx.EVT_MENU, self.OnAbout, id=wx.ID_ABOUT)
        
        cb = wx.ComboBox(pnl, pos=(50, 30), choices=listserial(),style=wx.CB_READONLY)
        cb.Bind(wx.EVT_COMBOBOX, self.SelectBus)
        
        self.menu_bar  = wx.MenuBar()
        self.file_menu = wx.Menu()
        self.help_menu = wx.Menu()

        self.file_menu.Append(wx.ID_EXIT,"&Quit")
        self.help_menu.Append(wx.ID_ABOUT, "&About PAWS")

        self.menu_bar.Append(self.file_menu, "&File")
        self.menu_bar.Append(self.help_menu, "&Help")
        self.SetMenuBar(self.menu_bar)
        
        cbtn = wx.Button(pnl, label='Add PAWS Board', pos=(mainframex-100, mainframey-50))
        cbtn.Bind(wx.EVT_BUTTON, self.OnQuit)
        
        self.rbv = wx.RadioButton(pnl, label='Voice', pos=(10, 30),style=wx.RB_GROUP)
        self.rbv.Bind(wx.EVT_RADIOBUTTON, self.SelFunc)
        
        self.rbs = wx.RadioButton(pnl, label='Sample', pos=(10, 50))
        self.rbs.Bind(wx.EVT_RADIOBUTTON, self.SelFunc)
        
        self.serialdisp = wx.StaticText(pnl, label=Serialname, style=wx.ALIGN_CENTRE)

        
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
    
    def SelectBus(self, e):
        global ser, VoiceFl, SampleFl
        Serialname = e.GetString()
        self.serialdisp.SetLabel(Serialname)
        ser = 'NULL'
        try:
            ser = serial.Serial(Serialname, BaudRate)
        except OSError:
            VoiceFl = 0
            SampleFl = 0
            print 'Failed to connect to Arduino [Serial]'
            pass
        if ser != 'NULL':
            print 'Connected to Arduino [Serial]'
    
    def OnQuit(self, e):
        e.Skip()
        time.sleep(0.01)
        self.Destroy()

    def OnAbout(self,e):
        print 'About'



############### END MAINFRAME CLASS ###############

#RETURN SERIAL
def returnbus():
    global ser
    ser = 'NULL'
    try:
        ser = serial.Serial(Serialname, BaudRate)
    except OSError:
        VoiceFl = 0
        SampleFl = 0
        print 'Failed to connect to Arduino [Serial]'
        pass
    if ser != 'NULL':
        print 'Connected to Arduino [Serial]'

############### END RETURN SERIAL ###############

# START
stream = Server(sr=FSard, nchnls=2).boot()
stream.start()


frame = wx.App()
Mainframe(None)
frame.MainLoop()

if ser != 'NULL':
    ser.close()
stream.stop()
print 'End Program'

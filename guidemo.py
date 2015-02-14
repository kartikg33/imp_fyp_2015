#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      ck2211
#
# Created:     04/06/2014
# Copyright:   (c) ck2211 2014
# Licence:     <your licence>
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      Christos Karpis
#
# Created:     03/06/2014
# Copyright:   (c) Christos Karpis 2014
# Licence:     <your licence>
#-------------------------------------------------------------------------------
import Tkinter
import bluetooth
import time
import random
import math
from Tkinter import *
from Tkinter import PhotoImage

# global variables: teacher_button, student_button, prototype_button, sparx_header, home_button

def main():
##    FrameCreator()
    print "Performing inquiry.."
    target_address = "00:12:02:28:73:47"
    target_name = "jyl2"
    port = 1
    nearby_devices = bluetooth.discover_devices(lookup_names=True)

    print "found %d devices" % len(nearby_devices)
    for name, addr in nearby_devices:
        print " %s - %s" % (addr,name)

    #bluetooth address of the bluetooth module is 00:12:02:28:73:47
    global client_socket, window
    client_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    client_socket.connect((target_address, port)) #client connects to the server on port 1
    print "connected to server"

    window =Tk()
    window.geometry("291x300")
    window.title("Dienes Blocks Application")
    window.configure(background='black')
    window.tk_setPalette(background='black', foreground='white', activeBackground='black', activeForeground='white')
    window.iconbitmap(default='favicon.ico')
##    w=Tkinter.Label(window,image=photo)
##    w.grid(row=0, column=1)
    app = HomeScreen(window)
    window.protocol("WM_DELETE_WINDOW", handler)
    window.mainloop()


def handler():
    print "Closing socket...\n",
    client_socket.close()
    print "done."
    window.destroy()

def generateQuestion(question_difficulty, sublevel):
    if question_difficulty == "easy":
        if sublevel == 1:
            question = random.randint(1,10)
        elif sublevel == 2:
            question = random.randint(11,20)
        elif sublevel == 3:
            question = random.randint(21,100)
    elif question_difficulty == "medium":
        if sublevel == 1:
            question = random.randint(101,110)
        elif sublevel == 2:
            question = random.randint(111,200)
        elif sublevel == 3:
            question = random.randint(201,999)
    elif question_difficulty =="hard":
        if sublevel == 1:
            question = random.randint(1000,1100)
        elif sublevel == 2:
            question = random.randint(1100,1200)
        elif sublevel == 3:
            question = random.randint(1200,2999)
    print question_difficulty, question
    return question

def bt_comm(question):
    confirmation_timeout = 2
    resend_timeout = 5
    answer_timeout = 300 #make very large to allow child to submit an answer
    C = 0
    A = 0
    escape = 0
    Question_String = 'Q'

    try:
             try:
                 client_socket.send("Q")
                 client_socket.send(str(question))
                 print str(question)
                 print "question sent"

                 start = time.time()

                 while (C == 0) : #if there Is data OR we have not yet read any data from a Question packet #(len(client_socket.recv(65536)) > 0)  ||
                    print "confirmation loop started"
                    data  = client_socket.recv(1024) #waits until there is data to be received
                    if len(data) != 0:
                        print "received [%s]" % data
                    print "confirmation data printed\n"

                    if(data == 'C') :
                        C = 1

                    if(((time.time() - start) > confirmation_timeout) and (C == 0)):
                        client_socket.send("Q")
                        client_socket.send(str(question))
                        print "message re-sent"

                       #count number of resends (if not using timeout3)
                       #if timeout2 >t2/number of resends exceeded without confirmation, display warning on child's screen
                    if(((time.time() - start) > resend_timeout) and (C == 0)):
                             #while(1):
                                print "!!!!!!!!!!!!!!!BOARD NOT RECEIVING! (resend_timeout) !!!!!!!!!!!!!!!!!" #/call function to display warning on screen and halt program
                                escape = 1
                                break
                    time.sleep(1)

             except bluetooth.BluetoothError, b:
                 print "Bluetooth Error: ", b
             else:
                start = time.time()

                while (A == 0):
                    answer_confirmation  = client_socket.recv(1024) # note that the A character is received separately from the answer and requires 2 recv.() calls
                    print "answer_confirmation [%s]"% (answer_confirmation)
                    if(answer_confirmation == 'A'):
                        answer_data = client_socket.recv(1024)
                        print "answer_data [%s]" % answer_data

                        A = 1
                    if((time.time() - start)> answer_timeout):
                            print "!!!!!!!!!!!!!!!NOT RECEIVING ANSWER! (answer_timeout) !!!!!!!!!!!!!!!!!" #/call function to display warning on screen and halt program
                            escape = 1
                            break

                client_socket.send("C")
                time.sleep(1)
                C=0
                A=0


    except KeyboardInterrupt:
         print "Closing socket...\n",
         client_socket.close()
         print "done."
    return answer_data


##def FrameCreator():
##    window =Tk()
##    window.geometry("300x300")
##    window.title("Dienes Blocks Application")
##    window.iconbitmap(default='favicon.ico')

class HomeScreen(Frame):
    def __init__(self, master):
        Frame.__init__(self,master)
        self.sparx_head()
        self.create_buttons()

    def sparx_head(self):
        self.grid()
        photo = PhotoImage(file="logosmall.gif")
        global sparx_header
        sparx_header = Label(self, image=photo, bg ="black")
        sparx_header.image = photo # keep a reference!
        sparx_header.grid(column=0, row=0, sticky='NSEW', columnspan=3)
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(0,weight=1)
##        self.resizable(True, True)

    def create_buttons(self):
        self.grid()
        global teacher_button, student_button, prototype_button
        #teacher button
        teacher_button = Tkinter.Button(self, text="Teacher")
##        teacher_button.place(y=90, height=20)
        teacher_button.grid(column=0, row=1, sticky='EW')
##        self.resizable(True, True)
##        teacher_button.pack()
        # student button
        student_button = Tkinter.Button(self, text="Student")
##        student_button.place(y=110, height=20)
        student_button.grid(column=2, row=1, sticky='EW')
##        self.resizable(True, True)
##        student_button.pack()
        # prototype button
        prototype_button = Tkinter.Button(self, text="Prototype", command=self.OnPrototypeButtonClick)
##        prototype_button.place(y=130, height=20)
        prototype_button.grid(column=1, row=1, sticky='EW')
##        self.resizable(True, True)
##        prototype_button.pack()

    def OnPrototypeButtonClick(self):
        sparx_header.grid_forget()
        teacher_button.grid_forget()
        prototype_button.grid_forget()
        student_button.grid_forget()
        app = Prototype(self)

class Prototype(Frame):
    def __init__(self,master):
        Frame.__init__(self,master)
        self.createLayout()
        window.geometry("600x300")
##        self.bluetoothRead()

    def createLayout(self):
        self.grid()
        global home_button, thousands, thousands_label, hundreds, hundreds_label, tens, tens_label, units, units_label

        photo = PhotoImage(file="logosmall.gif")
        global sparx_header
        sparx_header = Label(self, image=photo, bg ="black", anchor=W, justify=LEFT)
        sparx_header.image = photo # keep a reference!
        sparx_header.grid(column=0, row=0, sticky='NSEW', columnspan=3)
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(0,weight=1)

        #add the difficulty text entry
        global difficulty_input
        difficulty_input = Entry(self, bg="black", fg="white", justify=LEFT)
        difficulty_input.grid(row=2, column=0, columnspan=3)

        #add a difficulty submit button
        difficulty_button = Tkinter.Button(self, text="Confirm Difficulty", command=self.OnSubmitDifficulty)
        difficulty_button.grid(row=2, column=3, columnspan=1)

        self.current_question = Tkinter.StringVar()
        self.current_question.set ("No current question answered")

        #add the current question answered label
        current_question_label = Label(self, textvariable= self.current_question, width=60, anchor=W, justify=LEFT)
        current_question_label.grid(row=3, column=0, columnspan =4)

        #add the feedback label and its variable
        self.feedback = Tkinter.StringVar()

        #add label to say number of blocks on the board
        answer_label = Label(self, textvariable=self.feedback, width=60, anchor=W, justify=LEFT)
        answer_label.grid(row=4, column=0, columnspan=4)

        #add the four zones
        thousands = Label(self, text="Thousands", bg="red3", fg="black", relief="sunken", height=2, width=15)
        thousands.grid(column=0, row=5, columnspan=1, rowspan=2)
        hundreds = Label(self, text="Hundreds", bg="RoyalBlue1", fg="black", relief="sunken", height=2, width=15)
        hundreds.grid(column=1, row=5, columnspan=1, rowspan=2)
        tens = Label(self, text="Tens", bg="green3", fg="black", relief="sunken",height=2, width=15)
        tens.grid(column=2, row=5, columnspan=1, rowspan=2)
        units = Label(self, text="Units", bg="gold2", fg="black", relief="sunken",height=2,width=15)
        units.grid(column=3, row=5, columnspan=1, rowspan=2)

        #create variables to hold the data
        self.thousands_data = Tkinter.StringVar()

        self.hundreds_data = Tkinter.StringVar()
        self.tens_data = Tkinter.StringVar()
        self.units_data = Tkinter.StringVar()

        #create labels to display the data
        thousands_label = Label(self, textvariable=self.thousands_data, bg="white", fg="black", relief="raised", height=3, width=15)
        thousands_label.grid(column=0, row=7)
        hundreds_label = Label(self, textvariable=self.hundreds_data, bg="white", fg="black", relief="raised", height=3, width=15)
        hundreds_label.grid(column=1, row=7)
        tens_label = Label(self, textvariable=self.tens_data, bg="white", fg="black", relief="raised", height=3, width=15)
        tens_label.grid(column=2, row=7)
        units_label = Label(self, textvariable=self.units_data, bg="white", fg="black", relief="raised", height=3, width=15)
        units_label.grid(column=3, row=7)



        #add the home button
        home_button = Tkinter.Button(self, text="Home", command=self.HomeButtonClick)
        home_button.grid(row=11, column=0, sticky='EW')

        #add the question set button
        question_set_button = Tkinter.Button(self, text="Question Set")
        question_set_button.grid(row=11, column=2, sticky='EW')

        #add the question level button
        start_button = Tkinter.Button(self, text="START!", command=self.OnStartPress)
        start_button.grid(row=11, column=3, sticky='EW')


    def HomeButtonClick(self):
        home_button.grid_forget()
        thousands.grid_forget()
        hundreds.grid_forget()
        tens.grid_forget()
        units.grid_forget()
        thousands_label.grid_forget()
        hundreds_label.grid_forget()
        tens_label.grid_forget()
        units_label.grid_forget()
        app = HomeScreen(self)

    def OnSubmitDifficulty(self):
        global question_difficulty
        question_difficulty = difficulty_input.get()
        print question_difficulty

    def OnStartPress(self):
        sublevel = 1
        current_question = generateQuestion(question_difficulty, sublevel)
        self.UpdateLabel(current_question)
##        self.current_question.set(str(current_question) +" is the current question attempted")
        attempt_number = 1
        correct = 0
        number_wrong = 0
##        while True:
        answer = bt_comm(current_question)
        number = int(answer)
        number_thousands = math.floor(number/1000)
        self.thousands_data.set(int(number_thousands))
        thousands_modulo = number%1000
        number_hundreds = math.floor(thousands_modulo/100)
        self.hundreds_data.set(int(number_hundreds))
        hundreds_modulo = thousands_modulo%100
        number_tens = math.floor(hundreds_modulo/10)
        self.tens_data.set(int(number_tens))
        tens_modulo = hundreds_modulo%10
        number_ones = math.floor(tens_modulo)
        self.units_data.set(int(number_ones))
        if number == current_question:
                self.feedback.set("The answer given is correct! :)")
                # check whether 3 correct ones have been answered in a row
                correct += 1
                if attempt_number == 2:
                    correct = 0
                # keep track of the previous question
                previous_question = current_question

                #generate a new question
                if correct == 3: # if 3 correct ones in a row
                    sublevel += 1 # go to the next level
                    if sublevel > 3:
                        sublevel = random.randint(1,3)
                    current_question = generateQuestion(question_difficulty, sublevel)
                    correct = 0
                    attempt_number = 1
                else:
                    # ensure question is not the same as the previous one
                    while current_question == previous_question:
                        current_question = generateQuestion(question_difficulty, sublevel)
                    attempt_number = 1
            # if the answer is wrong once
        elif number != current_question and attempt_number == 1:
                self.feedback.set("The answer given is wrong :/")
                correct = 0
                attempt_number = 2
            # if the answer is wrong after retrying
        elif number != current_question and attempt_number == 2:
                self.feedback.set("The answer given is wrong twice in a row :(")
                current_question = generateQuestion(question_difficulty, sublevel)
                number_wrong += 1
                if number_wrong == 2:
                    if sublevel != 1:
                        sublevel -= 1
                    current_question = generateQuestion(question_difficulty, sublevel)
                    number_wrong = 0

    def UpdateLabel(self, current_question):
        self.current_question.set(str(current_question) +" is the current question attempted")


if __name__ == "__main__":
    main()

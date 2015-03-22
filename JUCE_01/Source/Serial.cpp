/*
  ==============================================================================

    Serial.cpp
    Created: 10 Mar 2015 4:31:13pm
    Author:  Kartik Gohil

  ==============================================================================
*/

//#include "Serial.h"
#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <CoreFoundation/CoreFoundation.h>
//#include <CoreServices/CoreServices.h>

#include <IOBluetooth/IOBluetoothUserLib.h>
#include <IOBluetooth/IOBluetoothUtilities.h>
#include <IOBluetoothUI/IOBluetoothUI.h>
#include <IOBluetoothUI/IOBluetoothUIUserLib.h>
/*
#include <IOKit/IOTypes.h>
#include <IOKit/IOReturn.h>
#include <IOKit/hid/IOHIDLib.h>

*/



#define ard "/dev/tty.usbmodem1d1121"
#define blu "/dev/tty.HC-06-DevB"


#include <pthread.h>

struct thread_args{
    int  serial;
    float *buffptr;
    int bufflen;
};


int serialConnect(char*);
float serialInput(int);
void * readser(void *);
void serialClose(int);
void blueConnect(void);

int serialConnect(char* ser){
    
    struct termios options;
    
    memset(&options,0,sizeof(options));

    //std::cout<<"Do Nothing";
    const char *device = ser;
    int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1) {
        std::cout<<"Failed to connect to Arduino\n";
    } else {
        std::cout<<"Connected to Arduino\n";
        tcgetattr(fd, &options);   // read serial port options
        cfsetospeed(&options,B115200);            // 115200 baud
        cfsetispeed(&options,B115200);            // 115200 baud
        tcsetattr(fd,TCSANOW, &options);
        fcntl(fd, F_SETFL, 0);    // clear all flags on descriptor, enable direct I/O
        //tcgetattr(fd, &options);   // read serial port options

    }
    return fd;

}

float serialInput(int fd){
    
    //std::cout<<"Do Nothing";
    //int len = 30;
    char ascii_int[30] = {0};
    char c = NULL;
    int i = 0;
    
    read(fd, &c, 1);
    while ((c != '\n')&&(i<29))
    //while (c != ' ')
    {
        ascii_int[i++] = c;
        //std::cout<<i<<newLine;
        read(fd, &c, 1);
    }
    //std::cout<<ascii_int<<newLine;
    return atof(ascii_int);


}

void * readser(void * input){
    std::cout<<"In Thread\n";
    struct thread_args * serarg;
    serarg = (struct thread_args *) input;
    float * buff = serarg->buffptr;
    int ptr = 0;
    std::cout<<"Create While In Thread\n";
    while(1){
        char ascii_flo[5] = {0};
        char c = NULL;
        int i = 0;
    
//        read(serarg->serial, &c, 1)
        std::cout<<"Read Ser In While In Thread\n";
        while ((c != '\n')&&(i<4)) {
            ascii_flo[i++] = c;
            //std::cout<<i<<newLine;
            read(serarg->serial, &c, 1);
        }
        //std::cout<<ascii_int<<newLine;
        try {
            buff[ptr] = atof(ascii_flo);
        }
        catch(std::exception& e) { //Takes a reference to an 'exception' object
            std::cout << "Error allocating memory: " << e.what() << std::endl;
        }
        
        ptr++;
        if(ptr>=serarg->bufflen){
            ptr = 0;
        }
    }
    std::cout<<"Exit Thread\n";
    pthread_exit(NULL);
}

void serialClose(int fd){
    if(fd!=-1){
    close(fd);
    std::cout<<"Closing connection to Arduino\n";
    }
}



void blueConnect(){
    
    //IOBluetoothDeviceRef *blue;
    //IOBluetoothDeviceSelectorControllerRef sel = IOBluetoothGetDeviceSelectorController();
    //long sel = IOBluetoothNumberOfAvailableHIDDevices();
    
    //std::cout << sel;
    /*
     CFUUIDRef   uuid;
     CFStringRef string;
     uuid = CFUUIDCreate( NULL );
     string = CFUUIDCreateString( NULL, uuid );
     CFShow( string );
     */
    
    std::string z1 = "30-a8-db-45-54-61"; // hardcoded BT address
    /*
    IOBluetoothServiceBrowserControllerRef controller;
    IOBluetoothServiceBrowserControllerOptions inOptions;
    controller = IOBluetoothServiceBrowserControllerCreate(inOptions);
    std::cout<<controller;
*/
}




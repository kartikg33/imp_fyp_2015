//
//  Bluetooth.cpp
//  PAWS_audio_01
//
//  Created by Kartik Gohil on 18/03/2015.
//
//

#include "Bluetooth.h"
#include "JuceHeader.h"
#include <iostream>
#include <IOBluetooth/IOBluetooth.h>
#include <IOBluetooth/IOBluetoothTypes.h>
#include <IOBluetooth/IOBluetoothUserLib.h>
#include <IOBluetooth/IOBluetoothUtilities.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/usb/IOUSBLib.h>


void blue2(){
    CFMutableDictionaryRef matchingDict;
    io_iterator_t iter;
    kern_return_t kr;
    io_service_t device;
    long devices = IOBluetoothNumberOfAvailableHIDDevices();
    std::cout<<devices<<newLine;
    //matchingDict = IOServiceMatching();
    IOBluetoothDeviceSearchTypesBits bits = kIOBluetoothDeviceSearchClassic;
    std::cout<<bits<<newLine;
    //IOServiceMatching();
}

void blue(){
    //IOBluetoothDeviceRef *blu = new IOBluetoothDeviceRef;
    //IOBluetoothRFCOMMChannelRef blu;
    //std::cout<< blu <<newLine;
    CFMutableDictionaryRef matchingDict;
    io_iterator_t iter;
    kern_return_t kr;
    io_service_t device;
    /* set up a matching dictionary for the class */
    matchingDict = IOServiceMatching("/dev/tty.");
    if (matchingDict == NULL)
    {
        std::cout<<-1<<newLine; // fail
    }
    //std::cout<<matchingDict<<newLine;
    
    /* Now we have a dictionary, get an iterator.*/
    kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iter);
    if (kr != KERN_SUCCESS)
    {
        std::cout<<-1<<newLine;
    }
    //std::cout<<matchingDict<<newLine;
    
    /* iterate */
    while ((device = IOIteratorNext(iter)))
    {
        /* do something with device, eg. check properties */
        /* ... */
    
        std::cout<<String(device)<<newLine;
        /* And free the reference taken before continuing to the next item */
        IOObjectRelease(device);
    }

    
    /* Done, release the iterator */
    IOObjectRelease(iter);
    std::cout<<0<<newLine;
    
}

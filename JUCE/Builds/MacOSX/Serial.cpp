//
//  Serial.cpp
//  PAWS_audio_juce
//
//  Created by Kartik Gohil on 09/03/2015.
//
//

#include "Serial.h"

int Serial::ReadData(char *buffer, unsigned int nbChar)
{
    //Number of bytes we'll have read
    unsigned int bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;
    
    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->errors, &this->status);
    
    //Check if there is something to read
    if(this->status.cbInQue>0)
    {
        //If there is we check if there is enough data to read the required number
        //of characters, if not we'll read only the available characters to prevent
        //locking of the application.
        if(this->status.cbInQue>nbChar)
        {
            toRead = nbChar;
        }
        else
        {
            toRead = this->status.cbInQue;
        }
        
        //Try to read the require number of chars, and return the number of read bytes on success
        if(ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0)
        {
            return bytesRead;
        }
        
    }
    
    //If nothing has been read, or that an error was detected return -1
    return -1;
    
}
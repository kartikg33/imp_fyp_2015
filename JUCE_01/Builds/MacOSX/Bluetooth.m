//
//  Bluetooth.cpp
//  PAWS_audio_01
//
//  Created by Kartik Gohil on 10/03/2015.
//
//

#include <CoreFoundation/CoreFoundation.h>

int main()
{
    CFUUIDRef   uuid;
    CFStringRef string;
    
    uuid = CFUUIDCreate( NULL );
    string = CFUUIDCreateString( NULL, uuid );
    
    CFShow( string );
}

- (BOOL)publishService
{
    NSString            *dictionaryPath = nil;
    NSString            *serviceName = nil;
    NSMutableDictionary *sdpEntries = nil;
    
    // Create a string with the new service name.
    serviceName = [NSString stringWithFormat:@"%@ My New Service", [self
                                                                    localDeviceName]];
    
    // Get the path for the dictionary we wish to publish.
    dictionaryPath = [[NSBundle mainBundle]
                      pathForResource:@"MyServiceDictionary" ofType:@"plist"];
    
    if ( ( dictionaryPath != nil ) && ( serviceName != nil ) )
    {
        // Initialize sdpEntries with the dictionary from the path.
        sdpEntries = [NSMutableDictionary
                      dictionaryWithContentsOfFile:dictionaryPath];
        
        if ( sdpEntries != nil )
        {
            IOBluetoothSDPServiceRecordRef  serviceRecordRef;
            
            [sdpEntries setObject:serviceName forKey:@"0100 - ServiceName*"];
            
            // Create a new IOBluetoothSDPServiceRecord that includes both
            // the attributes in the dictionary and the attributes the
            // system assigns. Add this service record to the SDP database.
            if (IOBluetoothAddServiceDict( (CFDictionaryRef) sdpEntries,
                                          &serviceRecordRef ) == kIOReturnSuccess)
            {
                IOBluetoothSDPServiceRecord *serviceRecord;
                
                serviceRecord = [IOBluetoothSDPServiceRecord
                                 withSDPServiceRecordRef:serviceRecordRef];
                
                // Preserve the RFCOMM channel assigned to this service.
                // A header file contains the following declaration:
                // IOBluetoothRFCOMMChannelID mServerChannelID;
                [serviceRecord getRFCOMMChannelID:&mServerChannelID];
                
                // Preserve the service-record handle assigned to this
                // service.
                // A header file contains the following declaration:
                // IOBluetoothSDPServiceRecordHandle mServerHandle;
                [serviceRecord getServiceRecordHandle:&mServerHandle];
                
                // Now that we have an IOBluetoothSDPServiceRecord object,
                // we no longer need the IOBluetoothSDPServiceRecordRef.
                IOBluetoothObjectRelease( serviceRecordRef );
                
            }
        }
    }
}


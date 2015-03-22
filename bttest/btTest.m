//
//  btTest.m
//  bttest
//
//  Created by Kartik Gohil on 10/03/2015.
//
//

#import <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>

@implementation BTObj

int main()
{
    CFUUIDRef   uuid;
    CFStringRef string;
    
    uuid = CFUUIDCreate( NULL );
    string = CFUUIDCreateString( NULL, uuid );
    
    CFShow( string );
    
    // Register for a notification so we get notified when a client opens
    // the channel assigned to our new service.
    // A header file contains the following declaration:
    // IOBluetoothUserNotification *mIncomingChannelNotification;
    
    mIncomingChannelNotification = [IOBluetoothRFCOMMChannel
                                    registerForChannelOpenNotifications:self
                                    selector:@selector(newRFCOMMChannelOpened:channel:)
                                    withChannelID:mServerChannelID
                                    direction:kIOBluetoothUserNotificationChannelDirectionIncoming];
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

- (void)stopProvidingService
{
    if ( mServerHandle != 0 )
    {
        // Remove the service.
        IOBluetoothRemoveServiceWithRecordHandle( mServerHandle );
    }
    
    // Unregister the notification.
    if ( mIncomingChannelNotification != nil )
    {
        [mIncomingChannelNotification unregister];
        mIncomingChannelNotification = nil;
    }
    
    mServerChannelID = 0;
}



@end

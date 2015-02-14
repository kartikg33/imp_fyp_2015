clear all;
port = '/dev/tty.HC-06-DevB';
usb = '/dev/tty.usbmodem1421';
s = serial(usb);
%get(s,{'Name','Port','Type'})
%set(s,'BaudRate',9600);
fopen(s);
%fprintf(s,'*IDN?')
%readasync(s)
out = fscanf(s)

fclose(s)
delete(s)
clear s

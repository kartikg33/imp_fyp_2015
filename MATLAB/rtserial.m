function rtserial

global buff s tkill;

buff = [];
FSout = 44100;

port = '/dev/tty.HC-06-DevB';
usb = '/dev/tty.usbmodem1411';
usb2 = '/dev/tty.usbmodem1a1221';

s = serial(usb,'BaudRate',9600);
set(s,'InputBufferSize',512);
fopen(s);

%Data Input Timer
TIN_PERIOD = 0.005;
tin = timer('TimerFcn',@(x,y)datain(),'Period',TIN_PERIOD);
set(tin,'ExecutionMode','fixedRate');


%Data Input Timer
TOUT_PERIOD = 0.01;
tout = timer('TimerFcn',@(x,y)dataout(),'Period',TOUT_PERIOD);
set(tout,'ExecutionMode','fixedRate');

%Kill Timer
TKILL_PERIOD = 10;
tkill = timer('TimerFcn',@(x,y)endprogram(tin,tout),'StartDelay',TKILL_PERIOD);

start(tin);
start(tout);
start(tkill);



end

function datain()
    global buff s;
    buff = [buff, fscanf(s,'%f')];
end

function dataout()
    global buff;
    thresh = 500;
    disp((buff))
    %disp((buff>thresh)')
    %out = buff>500
    buff = [];
end

function endprogram(tin,tout)
    global buff s tkill;
    
    fclose(s);
    delete(s);
    stop(tin);
    stop(tout);
    stop(tkill);
    delete(tin);
    delete(tout);
    delete(tkill);
    
    clear all;
    disp('End Program')
end





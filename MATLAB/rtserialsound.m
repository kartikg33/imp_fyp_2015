function rtserial

global buff buffFS s playsize tkill serialbuffsize;

buffFS = 8000;
buffT = 1.25e-04; % 0.000125 seconds
buff = zeros(1,8000);

TIN_PERIOD = 0.005;
TOUT_PERIOD = 1;
TKILL_PERIOD = 3;

playsize = buffFS*TOUT_PERIOD;

port = '/dev/tty.HC-06-DevB';
usb = '/dev/tty.usbmodem1411';
usb2 = '/dev/cu.usbmodem1a1221';

s = serial(usb2,'BaudRate',9600);
serialbuffsize = buffFS*TIN_PERIOD;
set(s,'InputBufferSize',serialbuffsize*2); %size of input buffer in bytes 
set(s,'ByteOrder','littleEndian');
set(s,'ReadAsyncMode','continuous');
fopen(s);

%Data Input Timer
tin = timer('TimerFcn',@(x,y)datain(),'Period',TIN_PERIOD);
set(tin,'ExecutionMode','fixedRate');

%Data Input Timer
tout = timer('TimerFcn',@(x,y)dataout(),'Period',TOUT_PERIOD);
set(tout,'ExecutionMode','fixedRate');

%Kill Timer
tkill = timer('TimerFcn',@(x,y)endprogram(tin,tout),'StartDelay',TKILL_PERIOD);

start(tin);
delay = 1;
while delay<10000000
    delay = delay+1;
end
start(tout);
start(tkill);



end

function datain()
    global buff s serialbuffsize;
    %buff = [buff,fread(s,serialbuffsize,'uint16')'];
    %for i=1:serialbuffsize
        buff = [buff,fscanf(s,'%u')];
    %end
end

function dataout()
    global buff buffFS playsize;
    %sound(buff(1:playsize),buffFS);
    disp(buff(1:playsize))
    buff = buff(playsize+1:end);
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
    clear buff;
    disp('End Program')
end





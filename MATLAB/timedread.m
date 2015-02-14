function timedread

clear all;

global buff newbuff TIMER_PERIOD FSout ptr

TIMER_PERIOD = 10;
FSout = 44100;
ptr = 0;

filename = 'cole.mp3';
in = audioread(filename);

buff = 0;
newbuff = 0;

t = timer('TimerFcn',@(x,y)writeData(),'Period',TIMER_PERIOD);
set(t,'ExecutionMode','fixedRate');
start(t)

while(1) 
    %Simulate new data coming in from outside
    newbuff = in(ptr+1:ptr+(FSout*TIMER_PERIOD),:);
    % buff gets processed here
    disp('Other Shit Happnin')
end

end

function writeData()
    
    global buff newbuff TIMER_PERIOD FSout ptr;
    %Output current buffer data
    sound(buff,FSout)
    %Get new buffer data
    buff = newbuff;
    ptr = ptr+(FSout*TIMER_PERIOD);    
    
end
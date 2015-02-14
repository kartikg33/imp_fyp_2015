function chunkreadwrite
clear all;

global FSout
FSout = 44100;

filename = 'cole.mp3';
in = audioread(filename);

ptr = 0;
len = FSout*3;

while(1)
    
    wav = in(ptr+1:ptr+len,:);
    playsound(wav);
    ptr = ptr+len;

    if ptr>FSout*12
        break
    end
    pause(3);
    
end

end

function playsound(wav)
    global FSout;
    sound(wav,FSout);
end

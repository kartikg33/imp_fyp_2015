clear all;
%%

[wavlo, flo] = audioread('/Test Audio/micslap8k.wav');
[wavhi, fhi] = audioread('/Test Audio/micslap44p1k.wav');
[waveng, feng] = audioread('/Test Audio/micslapengland.wav');
[drum, fdrum] = audioread('/Test Audio/afrihi.wav');
wavorig = waveng;
fo = feng;
%%

wavfilt = wavorig;
freq = fft(wavfilt);
freq((length(wavfilt)*0.01):length(wavfilt)) = 0;
wavfilt = abs(ifft(freq));

%%
wav = wavfilt;
buffSize = 1000;
buff = zeros(buffSize,1);
buffptr = 1;
wavtrans = [];
range = 500;
change = 0.005;
i = 1;

while 1
    
    buff(buffptr) = wav(i);
    diff = buffptr - range;
    if (diff > 0)
        if (buff(buffptr) - buff(diff) > change)
            wavtrans(i) = 1;
        end
    elseif (diff < 0)  
        if (buff(buffptr) - buff(buffSize + diff) > change)
            wavtrans(i) = 1;
        end
    else
        wavtrans(i) = 0;
    end
    
    buffptr = buffptr + 1;
    if buffptr > buffSize
        buffptr = 1;
    end
    
    i = i + 1;
    if i > length(wav)
        break
    end
end

%%
i = 1;
play = 0;
prev = 1;
druml = length(drum);
wavl = length(wavtrans)+druml;

wavfinal = zeros(wavl,2);

while 1
    buff = zeros(wavl,2);
    if wavtrans(i) == 1 && prev == 0
        play = 1;
    end
    prev = wavtrans(i);
    
    if play == 1
        for j = i:i+druml-1
            buff(j,1) = drum(j-i+1,1);
            buff(j,2) = drum(j-i+1,2);
        end
        wavfinal = wavfinal + buff;
    end
    play = 0;
    i = i + 1;
    if i > length(wavtrans)
        break
    end
end

%%
h = figure;
subplot(4,1,1)
plot(wavorig);
axis([1 length(wavorig) min(min(wavorig)) max(max(wavorig))]);
title('Recording (44.1kHz)');
subplot(4,1,2)
plot(wavfilt);
axis([1 length(wavorig) min(min(wavfilt)) max(max(wavfilt))]);
title('Low Pass Filtered');
subplot(4,1,3)
plot(wavtrans);
axis([1 length(wavorig) min(min(wavtrans)) max(max(wavtrans))]);
title('Transient Detection');
subplot(4,1,4)
plot(wavfinal);
axis([1 length(wavorig) min(min(wavfinal)) max(max(wavfinal))]);
title('Drum Output');
print(h,'-djpeg','slaptodrum');
%%
%sound(wavorig,fo)
%sound(wavfinal,fo)
%audiowrite('micslapengDRUM.wav',wavfinal,fo);
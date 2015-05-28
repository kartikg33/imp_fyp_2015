A = csvread('qsampcsv_25052015.csv');
f = fft(A);
oldrate = 44100/7;
newrate = 44100;

%% MIC BUMP
B = (A(30100:31700)-512)/512;
C = [B(1)];
for x=2:length(B)  
    inter = (B(x)-B(x-1))/7;
    for i=1:6
        C = [C, B(x-1)+(inter*i)]; 
    end
    C = [C, B(x)];
end

figure;
subplot(2,2,1)
f = fft(C);
semilogy([0:newrate/length(f):(newrate/2)-(newrate/length(f))]',abs(f(1:length(f)/2)))
ylim([0 1000])
ylabel('Amplitude (dB)')
xlabel('Frequency (Hz)')
title('Interpolated Signal @ 44100Hz')

subplot(2,2,3)
plot([0:1/newrate:(length(C)/newrate)-(1/newrate)],C)
xlim([0, (length(C)/newrate)-(1/newrate)]);
ylabel('Amplitude (floating point value)')
xlabel('Time (s)')

rp = 3;           % Passband ripple
rs = 40;          % Stopband ripple
fs = newrate;        % Sampling frequency
f = [1000 oldrate/2];    % Cutoff frequencies
a = [1 0];        % Desired amplitudes
dev = [(10^(rp/20)-1)/(10^(rp/20)+1)  10^(-rs/20)];
[n,fo,ao,w] = firpmord(f,a,dev,fs);
b = firpm(n,fo,ao,w);
D = filter(b,1,C);

subplot(2,2,2)
f = fft(D);
semilogy([0:newrate/length(f):(newrate/2)-(newrate/length(f))]',abs(f(1:length(f)/2)))
ylim([0 1000])
ylabel('Amplitude (dB)')
xlabel('Frequency (Hz)')
title('Filtered Signal @ 44100Hz')

subplot(2,2,4)
plot([0:1/newrate:(length(D)/newrate)-(1/newrate)],D)
xlim([0, (length(D)/newrate)-(1/newrate)]);
ylabel('Amplitude (floating point value)')
xlabel('Time (s)')

%% 'TESTING'

B = (A(125800:127350)-512)/512;
C = [B(1)];
for x=2:length(B)  
    inter = (B(x)-B(x-1))/7;
    for i=1:6
        C = [C, B(x-1)+(inter*i)]; 
    end
    C = [C, B(x)];
end

figure;
subplot(2,2,1)
f = fft(C);
semilogy([0:newrate/length(f):(newrate/2)-(newrate/length(f))]',abs(f(1:length(f)/2)))
ylim([0 1000])
ylabel('Amplitude (dB)')
xlabel('Frequency (Hz)')
title('Interpolated Signal @ 44100Hz')

subplot(2,2,3)
plot([0:1/newrate:(length(C)/newrate)-(1/newrate)],C)
xlim([0, (length(C)/newrate)-(1/newrate)]);
ylabel('Amplitude (floating point value)')
xlabel('Time (s)')

rp = 3;           % Passband ripple
rs = 40;          % Stopband ripple
fs = newrate;        % Sampling frequency
f = [1000 oldrate/2];    % Cutoff frequencies
a = [1 0];        % Desired amplitudes
dev = [(10^(rp/20)-1)/(10^(rp/20)+1)  10^(-rs/20)];
[n,fo,ao,w] = firpmord(f,a,dev,fs);
b = firpm(n,fo,ao,w);
D = filter(b,1,C);

subplot(2,2,2)
f = fft(D);
semilogy([0:newrate/length(f):(newrate/2)-(newrate/length(f))]',abs(f(1:length(f)/2)))
ylim([0 1000])
ylabel('Amplitude (dB)')
xlabel('Frequency (Hz)')
title('Filtered Signal @ 44100Hz')

subplot(2,2,4)
plot([0:1/newrate:(length(D)/newrate)-(1/newrate)],D)
xlim([0, (length(D)/newrate)-(1/newrate)]);
ylabel('Amplitude (floating point value)')
xlabel('Time (s)')

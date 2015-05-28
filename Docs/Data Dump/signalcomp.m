A = csvread('qsampcsv_25052015.csv');
f = fft(A);
oldrate = 44100/7;

%% MIC BUMP
figure;
scope = imread('scope_1.png');
imshow(scope,[], 'XData', [0 10], 'YData', [0 10])
figure;
subplot(2,1,1)
B = (A(30100:31700)-512)/512;
f = fft(B);
semilogy([0:oldrate/length(f):(oldrate/2)-(oldrate/length(f))]',abs(f(1:length(f)/2)))
ylim([0 1000])
ylabel('Amplitude (dB)')
xlabel('Frequency (Hz)')

subplot(2,1,2)
plot([0:1/oldrate:(length(B)/oldrate)-(1/oldrate)],B)
xlim([0, (length(B)/oldrate)-(1/oldrate)]);
ylabel('Amplitude (floating point value)')
xlabel('Time (s)')

%% 'TESTING'
figure;
scope = imread('scope_3.png');
imshow(scope,[], 'XData', [0 10], 'YData', [0 10])
figure;
subplot(2,1,1)
B = (A(125800:127350)-512)/512;
f = fft(B);
semilogy([0:oldrate/length(f):(oldrate/2)-(oldrate/length(f))]',abs(f(1:length(f)/2)))
ylim([0 1000])
ylabel('Amplitude (dB)')
xlabel('Frequency (Hz)')

subplot(2,1,2)
plot([0:1/oldrate:(length(B)/oldrate)-(1/oldrate)],B)
xlim([0, (length(B)/oldrate)-(1/oldrate)]);
ylabel('Amplitude (floating point value)')
xlabel('Time (s)')


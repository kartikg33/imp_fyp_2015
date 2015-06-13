%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('scope_5.csv');
%A = B(800:900,:);%idle
%A = B(350:500,:);%ringtap
A = B; %everything
numsamp = size(A,1);
T = (A(end,1)-A(1,1))/numsamp;
fs = 1/T;
subplot(2,1,1)
%plot([1:1:numsamp],A(:,2))
plot(A(:,1),A(:,2))
xlabel('Time (seconds)');
ylabel('Amplitude (Volts)');
freqlim = [1:fs/numsamp:fs/2];
freqax = round(freqlim*numsamp/fs);
I = find(freqax==0);
freqax(I) = 1;
A_freq = abs(fft(A(:,2)));
subplot(2,1,2)
loglog(freqlim,A_freq(freqax));
ylim([0,5]);
xlabel('Frequency (Hz)');
ylabel('Amplitude (dB)');
%% RAIL NOISE 1
%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('scope_5.csv');
A = B; %everything
numsamp = size(A,1);
T = (A(end,1)-A(1,1))/numsamp;
fs = 1/T;

subplot(2,1,1)
plot(A(:,1),A(:,2))
xlim([A(1,1),A(end,1)]);
ylim([4.85 4.95]);
grid minor;
xlabel('Time (seconds)');
ylabel('Amplitude (Volts)');

subplot(2,1,2)
freqlim = [50:fs/numsamp:fs/2];
freqax = round(freqlim*numsamp/fs);
I = find(freqax==0);
freqax(I) = 1;
A_freq = abs(fft(A(:,2)));
loglog(freqlim,A_freq(freqax));
hold on;
temp = zeros(1,length(freqlim));
temp(round(490*numsamp/fs)) = 100;
stem(freqlim,temp,'LineStyle','--','Marker', 'none');
str1 = '490Hz';
text(350,0.1,str1,'Color','r')
hold off;
%ylim([0,5]);
xlim([freqlim(1), freqlim(end)]);
ylim([1e-2 1e2]);
grid minor;
xlabel('Frequency (Hz)');
ylabel('Amplitude (dB)');
%% RAIL NOISE 2
%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('scope_10.csv');
A = B(100:900,:); %everything
numsamp = size(A,1);
T = (A(end,1)-A(1,1))/numsamp;
fs = 1/T;

plot(A(:,1),A(:,2))
xlim([A(1,1),A(end,1)]);
ylim([4.75 4.8]);
grid minor;
xlabel('Time (seconds)');
ylabel('Amplitude (Volts)');


%% SIGNAL NOISE 1
%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('scope_7.csv');
A = B; %everything
numsamp = size(A,1);
T = (A(end,1)-A(1,1))/numsamp;
fs = 1/T;

subplot(2,1,1)
plot(A(:,1),A(:,2))
xlim([A(1,1),A(end,1)]);
ylim([2 3]);
grid minor;
xlabel('Time (seconds)');
ylabel('Amplitude (Volts)');

subplot(2,1,2)
freqlim = [100:fs/numsamp:fs/2];
freqax = round(freqlim*numsamp/fs);
I = find(freqax==0);
freqax(I) = 1;
A_freq = abs(fft(A(:,2)));
loglog(freqlim,A_freq(freqax));
hold on;
temp = zeros(1,length(freqlim));
temp(round(490*numsamp/fs)) = 100;
stem(freqlim,temp,'LineStyle','--','Marker', 'none');
str1 = '490Hz';
text(350,0.1,str1,'Color','r')
hold off;
%ylim([0,5]);
xlim([freqlim(1), freqlim(end)]);
grid minor;
xlabel('Frequency (Hz)');
ylabel('Amplitude (dB)');
%% SIGNAL NOISE 2
%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('scope_11.csv');
%A = B(800:900,:);%idle
A = B(350:500,:);%ringtap
numsamp = size(A,1);
T = (A(end,1)-A(1,1))/numsamp;
fs = 1/T;

subplot(2,1,1)
plot(A(:,1),A(:,2))
xlim([A(1,1),A(end,1)]);
ylim([1 4]);
grid minor;
xlabel('Time (seconds)');
ylabel('Amplitude (Volts)');

subplot(2,1,2)
freqlim = [50:fs/numsamp:fs/2];
freqax = round(freqlim*numsamp/fs);
I = find(freqax==0);
freqax(I) = 1;
A_freq = abs(fft(A(:,2)));
loglog(freqlim,A_freq(freqax));
%hold on;
%temp = zeros(1,length(freqlim));
%temp(round(30*numsamp/fs)) = 100;
%stem(freqlim,temp,'LineStyle','--','Marker', 'none');
%str1 = '490Hz';
%text(350,0.1,str1,'Color','r')
%hold off;
%ylim([0,5]);
xlim([freqlim(1), freqlim(end)]);
ylim([1e-2 1e2]);
grid minor;
xlabel('Frequency (Hz)');
ylabel('Amplitude (dB)');
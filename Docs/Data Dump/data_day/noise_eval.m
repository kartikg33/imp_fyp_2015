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
B = csvread('scope_6.csv');
start = min(find(B(:,1)==-4e-3));
last = max(find(B(:,1)==4e-3));
A = B(start:last,:); %everything
numsamp = size(A,1);
T = (A(end,1)-A(1,1))/numsamp;
fs = 1/T;

plot(A(:,1),A(:,2))
xlim([A(1,1),A(end,1)]);
ylim([4.88 4.94]);
grid minor;
xlabel('Time (seconds)');
ylabel('Amplitude (Volts)');


%% RAIL NOISE 2
%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('scope_10.csv');
start = min(find(B(:,1)>-4e-3));
last = max(find(B(:,1)<4e-3));
A = B(start:last,:); %everything
%A = B(100:900,:); %everything
numsamp = size(A,1);
T = (A(end,1)-A(1,1))/numsamp;
fs = 1/T;

plot(A(:,1),A(:,2))
xlim([A(1,1),A(end,1)]);
ylim([4.75 4.8]);
grid minor;
xlabel('Time (seconds)');
ylabel('Amplitude (Volts)');

%% RAIL NOISE 3
%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('scope_14.csv');
start = min(find(B(:,1)>-4e-3));
last = max(find(B(:,1)<4e-3));
A = B; %everything
%A = B(100:900,:); %everything
numsamp = size(A,1);
T = (A(end,1)-A(1,1))/numsamp;
fs = 1/T;

%subplot(2,1,1)
plot(A(:,1),A(:,2))
xlim([A(1,1),A(end,1)]);
ylim([4.75 4.79]);
grid minor;
xlabel('Time (seconds)');
ylabel('Amplitude (Volts)');
%subplot(2,1,2)
diff = 100;
for i=diff+1:numsamp
   sig(i) = mean(A(i-diff:i,2)); 
end
sig(sig<4.772)=0;
sig(sig~=0)=1;
hold on
plot(A(:,1),(sig/50)+4.76, 'Color', 'r');
%ylim([4.768 4.776]);
hold off


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
temp(max(find(freqlim<500))) = 100;
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
temp(max(find(freqlim<500))) = 100;
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

%% SIGNAL NOISE 3
%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('sig_samp.csv');
%A = B(800:900,:);%idle
%A = B(82240:82500);%ringtap
%A=B(96575:96645)/204.8;
%figure;
%plot(B);
%figure;
%A=B(96500:96700)/204.8;
A=B(39505:39700);%/204.8;
numsamp = size(A,1);
fs = 2381;
%fs = 44100/12;
T = 1/fs;

subplot(2,1,1)
time = [0:T:(numsamp/fs)-T];
lime = [70:length(A)-55];
plot(A(lime))
%xlim([time(70),time(end-55)]);
xlim([1 length(lime)]);
ylim([0 1023]);
grid minor;
xlabel('Sample Number');
ylabel('Sample Value [0:1023]');

subplot(2,1,2)
freqlim = [50:fs/numsamp:fs/2];
freqax = round(freqlim*numsamp/fs);
I = find(freqax==0);
freqax(I) = 1;
A_freq = abs(fft(A));
loglog(freqlim,A_freq(freqax));
%hold on;
%temp = zeros(1,length(freqlim));
temp(max(find(freqlim<500))) = 100;
%stem(freqlim,temp,'LineStyle','--','Marker', 'none');
%str1 = '490Hz';
%text(350,0.1,str1,'Color','r')
%hold off;
%ylim([0,5]);
xlim([freqlim(1), 5026.63115845539/2]);
%ylim([1e-2 1e2]);
grid minor;
xlabel('Frequency (Hz)');
ylabel('Amplitude (dB)');
%% SIGNAL NOISE 4
%COLUMN 1: SECONDS
%COLUMN 2: VOLTS
B = csvread('sig_samp.csv');
%A = B(800:900,:);%idle
%A = B(82240:82500);%ringtap
%A=B(96575:96645)/204.8;
%figure;
%plot(B);
%figure;
%A=B(96500:96700)/204.8;
C=B(39505:39700);%/204.8;
over = round(44100/2381);
A = [C(1)];
for x=2:length(C)  
    inter = (C(x)-C(x-1))/over;
    for i=1:over-1
        A = [A, C(x-1)+(inter*i)]; 
    end
    A = [A, C(x)];
end
numsamp = size(A,2);
%fs = 2381;
fs = 44100;
%fs = 44100/12;
T = 1/fs;

subplot(2,1,1)
time = [0:T:(numsamp/fs)-T];
lime = [70*over:length(A)-(55*over)];
plot(A(lime))
%xlim([time(70*over),time(end-(55*over))]);
xlim([1 length(lime)]);
ylim([0 1023]);
grid minor;
xlabel('Sample Number');
ylabel('Sample Value [0:1023]');

subplot(2,1,2)
freqlim = [50:fs/numsamp:fs/2];
freqax = round(freqlim*numsamp/fs);
I = find(freqax==0);
freqax(I) = 1;
A_freq = abs(fft(A));
loglog(freqlim,A_freq(freqax));
%hold on;
%temp = zeros(1,length(freqlim));
temp(max(find(freqlim<500))) = 100;
%stem(freqlim,temp,'LineStyle','--','Marker', 'none');
%str1 = '490Hz';
%text(350,0.1,str1,'Color','r')
%hold off;
%ylim([0,5]);
%xlim([freqlim(1), 5026.63115845539/2]);
xlim([freqlim(1), fs/2]);
%ylim([1e-2 1e2]);
grid minor;
xlabel('Frequency (Hz)');
ylabel('Amplitude (dB)');
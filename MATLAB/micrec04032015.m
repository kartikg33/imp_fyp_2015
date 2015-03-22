x = csvread('micrec04032015_3.csv');
y=(x-512)/1023;
%sound(y,8000)
%%
subplot(2,1,1)
plot(y)
subplot(2,1,2)
spec = abs(fft(y));
plot(spec(1:length(spec)/2))
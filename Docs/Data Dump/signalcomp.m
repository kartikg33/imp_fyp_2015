A = csvread('qsampcsv_25052015.csv');
f = fft(A);
%%
subplot(1,2,1)
subplot(2,1,1)
B = A(30000:31800);
f = fft(B);
semilogx(abs(f))
ylim([0 100000])
subplot(2,1,2)
plot(B)
xlim([0 length(B)]);
subplot(1,2,2)
scope = imread('scope_1.png');
imshow(scope,[], 'XData', [0 .5], 'YData', [0 .1])

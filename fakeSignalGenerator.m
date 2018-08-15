mSignalID = fopen('meditation.txt', 'r');

formatSpec = '%f';

mSignal = fscanf(mSignalID, formatSpec);

fclose(mSignalID);

mSignalFT = fft(mSignal);
f = 0:250/length(mSignalFT):250;
f = f';
fakeSignalFT = mSignalFT;
fakeSignalFT = real(fakeSignalFT);
fakeSignalFT(51) = 2500; % where freq = 25Hz
fakeSignalFT(102) = 2500; %where freq = 50Hz
fakeSignalFT(162) = 2500; %where freq = 80Hz
fakeSignal = ifft(fakeSignalFT);
fakeSignal = real(fakeSignal);

[meditationPSD, meditationFreq] = powerSpectrum(mSignal, 250);
[fakePSD, fakeFreq] = powerSpectrum(fakeSignal, 250);


function [psdx, freqT] = powerSpectrum(x, ffss)
rng default;
Fs = ffss;
t = 0:1/Fs:1-1/Fs;

N = length(x);
xdft = fft(x);
xdft = xdft(1:N/2+1);
psdx = (1/(Fs*N)) * abs(xdft).^2;
psdx(2:end-1) = 2*psdx(2:end-1);
freq = 0:Fs/length(x):Fs/2;
freqT = freq';
end


function ccc = czConvolution(x, h)
x = x';
h = h';
m = length(x);
n = length(h);
X = [x, zeros(1, n)];
H = [h, zeros(1, m)];
for i=1:n+m-1
    Y(i) = 0;
    for j=1:m
        if (i-j+1>0)
            Y(i) = Y(i)+X(j)*H(i-j+1);
        else
        end
    end
end
ccc = Y;
end






















SignalID = fopen('meditation.txt', 'r');
formatSpec = '%f';
Signal = fscanf(SignalID, formatSpec);

[signalPSD, signalFreq] = powerSpectrum(Signal, 250);


function [psdx, freqT] = powerSpectrum(x, ffss)
rng default;
Fs = ffss;

N = length(x);
xdft = fft(x);
xdft = xdft(1:N/2+1);
psdx = (1/(Fs*N)) * abs(xdft).^2;
psdx(2:end-1) = 2*psdx(2:end-1);
%psdxT = psdx';
freq = 0:Fs/length(x):Fs/2;
freqT = freq';
end
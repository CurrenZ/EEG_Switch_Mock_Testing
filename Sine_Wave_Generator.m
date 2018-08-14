f1 = 25;
f2 = 50;
amp = 12;
ts = 1/250;
T = 2;

t = 0:ts:T;
y1 = amp*sin(2*pi*f1*t);
y2 = amp*sin(2*pi*f2*t);

ypy = y1+y2;
yppy = y1*(10^(-6/20)) + y2*(10^(-6/20));
ymy = y1.*y2;

[y1PSDX, y1FREQ] = powerSpectrum(y1, 250);
[y2PSDX, y2FREQ] = powerSpectrum(y2, 250);
[yppyPSDX, yppyFREQ] = powerSpectrum(yppy, 250);

y1 = y1';
y2 = y2';
ypy = ypy';
yppy = yppy';
%yppyn = yppyn';
ymy = ymy';

function [psdxT, freqT] = powerSpectrum(x, ffss)
rng default;
Fs = ffss;

N = length(x);
xdft = fft(x);
xdft = xdft(1:N/2+1);
psdx = (1/(Fs*N)) * abs(xdft).^2;
psdx(2:end-1) = 2*psdx(2:end-1);
psdxT = psdx';
freq = 0:Fs/length(x):Fs/2;
freqT = freq';
end
% ADRes
% [snd, Fs] = audioread("Fragile60-85.wav");
% [snd, Fs] = audioread("Chameleon60-85.wav");
% [snd, Fs] = audioread("Sweet25.wav");
% [snd, Fs] = audioread("damian_male.wav");
% [snd, Fs] = audioread("panSquareWave.wav");
% [snd, Fs] = audioread("panHarmonicWave.wav");
% [snd, Fs] = audioread("panHarmonicWaveBeta3.wav");
% [snd, Fs] = audioread("outPHWBeta3Pos-1.wav");
[snd, Fs] = audioread("outFragileBeta3p2gain12.wav");

M = 8192;
% M = 4096;
% win = triang(M);
win = hamming(M);
% win = hann(M);
% hopSize = M/2;
hopSize = M/4;


%% plot spectrogram of input file
[S,F,T] = spectrogram(snd(:,1),win,hopSize,M,Fs);
figure
imagesc(T,F,log10(abs(S)));

axis xy;
xlabel('Time (s)');
ylabel('Frequency (Hz)');
colormap bone;
%% do FFT
startTime = 10;
startNumber = startTime * Fs;

sampleInput = snd(startNumber : startNumber + M -1 ,1);
sampleFFT = fft( sampleInput.* win, M);

freqIndex = 1:M/2 +1;
freqIndex = freqIndex * Fs / M;

plot(freqIndex, 20 * log10(sampleFFT(1: M/2+1)))
xlabel('Frequency (Hz)')
ylabel('Magnitude (dB)')
grid on


%%
sound(snd,Fs);
%% pre-processing for HOPPING

tailForWin = mod(length(snd), hopSize) ;
sndPreProcess = zeros(length(snd) + M/2 + M/2 - tailForWin, 2);
sndPreProcess(M/2 + 1 : M/2 + length(snd), :) = snd;


%%
beta = 3;
positionChoice = 1; % range: 1 ~ 2*beta-1

plotAZPlaneTime = 1;   % seconds
plotAZPlaneNumber = plotAZPlaneTime * Fs;
plotAZplane = false;

outputPostProcess = zeros(length(sndPreProcess), 2);

% for i = M/2 + 1 : hopSize: length(outputPostProcess) - M/2
for i = M/2 + 1 : hopSize: length(outputPostProcess) - M
% i = 209364;   % for AZ plane plot

    inputSample = sndPreProcess(i : i + M - 1 , :);
    [outputSample, summary] = ADRess(inputSample, M, win, beta, positionChoice, plotAZplane);
    outputPostProcess(i : i + M - 1 , :) = outputPostProcess(i : i + M - 1 , :) + outputSample;
    if ( (plotAZPlaneNumber < i) && (i  < plotAZPlaneNumber + hopSize))
        plotAZplane = true;
        AZsummary = summary;
        AZifft = outputSample;
    else
        plotAZplane = false;
    end
end
% ADRessOut = zeros(length(snd),1);
ADRessOut = outputPostProcess(M/2 + 1 : M/2 + length(snd),: );

%%
sound((ADRessOut), Fs)
% audiowrite("Fragile03170501_b3p5.wav", real(outputPostProcess), Fs);


% Sweet: b3p1 & b3p5 are both great separation

%% plot AZsummary
possibleBeta = (max( AZsummary(:,1)) + 1 ) /2;
AZplane = zeros( length(AZsummary), possibleBeta*2 -1);
for b = 1 : possibleBeta * 2 - 1
    if b <= possibleBeta
        AZplane( find(AZsummary(:,1)==b) , b) = AZsummary( find(AZsummary(:,1) == b), 2);
    else
        AZplane( find(AZsummary(:,1)==b) , b) = AZsummary( find(AZsummary(:,1) == b), 3);
    end
end
% AZplane = (AZplane + 1e-6);
AZplane = 20 * log10 (AZplane + 1e0);

freqIndex = 1:length(AZsummary)/2 +1;
freqIndex = freqIndex * Fs / M;
%% mesh

mesh(( AZplane(1:4096,:)))
% mesh(20*log10( AZplane(1:4096,:)'))
% stem3(20*log10( AZplane(1:4096,:)))
%% plot 3D line for AZ Plane
plotEndFreqBin = 4096;

freqIndex = 1:plotEndFreqBin;
freqIndex = freqIndex * Fs / M;

for  i = 1: possibleBeta * 2 - 1
%     plot3( zeros(plotEndFreqBin,1) + i, freqIndex', AZplane(1:plotEndFreqBin, i), 'LineWidth', 8) % Matlab
    plot3( zeros(plotEndFreqBin,1) + i - possibleBeta, freqIndex', AZplane(1:plotEndFreqBin, i), 'LineWidth', 8) % Juce
    hold on
end

ylabel('Frequency')
xlabel('Azimuth position')
zlabel('Magnitude (dB)')
grid on

%% single fft
% plot(freqIndex, 20* log10(AZplane(1:4097, positionChoice)))

showPosition = 5;

freqIndex = 1:length(AZsummary)/2 +1;
freqIndex = freqIndex * Fs / M;
plot(freqIndex, (AZplane(1:2049, showPosition)))

%% plot time domain

timeIndex = 1:length(snd);
timeIndex = timeIndex'./Fs;
plot(timeIndex, ADRessOut(:,1))
hold on
plot(timeIndex, snd(:,1)*3/8)

%% audio write
audiowrite("Fragile03202027_b3p5_pow.wav",ADRessOut,Fs)


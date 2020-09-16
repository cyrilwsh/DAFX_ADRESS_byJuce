function [outputSample , AZsummary] = ADRess(inputSample, M, w, beta, positionChoice, plotAZplane)

%% step1: get the next frame of audio data

inputFFT = zeros(M,2);

for channel = 1:2
    % Do fft, zero-padding to M
    inputFFT( : , channel) = fft(inputSample(:, channel) .* w ,M);
end
    %% % for plot vs frequency
% plotOn = 1;
% if plotOn
%     inputFreqSeries=(1:M/2+1)./M*Fs;
%     figure()
%     plot(inputFreqSeries,inputFFTdB)
% end

% step2: apply a window function
% step3: FFT
% step4: compute the magnitude spectrum of the source(s) present at the chosen azimuth position(s)

%%
AL = zeros(M, beta);
AR = zeros(M, beta);
for i = 1: beta
    AL(:,i) =  abs(abs(inputFFT(:,1)) - i/beta * abs(inputFFT(:,2))) ;
    AR(:,i) =  abs(abs(inputFFT(:,2)) - i/beta * abs(inputFFT(:,1))) ;
%     AL(:,i) =  abs((inputFFT(:,1)) - i/beta * (inputFFT(:,2))).^2 ;
%     AR(:,i) =  abs((inputFFT(:,2)) - i/beta * (inputFFT(:,1))).^2 ;
end

%%
AZsummary = zeros(M,3);
for i = 1:M
    maxAL = max(AL(i,:));
    minAL = min(AL(i,:));
    maxAR = max(AR(i,:));
    minAR = min(AR(i,:));
    if minAL ~= 0 && minAR ~=0  % ignore occassionally all 0 part
        if minAL <= minAR
            AZsummary(i,1) = find(AL(i,:) == minAL);
        else
            AZsummary(i,1) = 2*beta - (find(AR(i,:) == minAR));
        end
    end

    AZsummary(i,2) = maxAL - minAL;
    AZsummary(i,3) = maxAR - minAR;
%     AZsummary(i,2) = sqrt(maxAL - minAL);
%     AZsummary(i,3) = sqrt(maxAR - minAR);

end

%%
% positionChoice = 3;

% AZplane = zeros(M, 2*beta + 1); % for plot AZ plane use

% for positionChoice = 1: 2*beta+1  % for plot AZ plane use

magnitudeFFT = zeros(M,2);
for i = 1:M
    if positionChoice <= beta   % choose from Left AZ-Plane
        if AZsummary(i,1) == positionChoice
            magnitudeFFT(i,1) = AZsummary(i,2); 
            magnitudeFFT(i,2) = AZsummary(i,2);
        end
    else % positionChoice > beta    % choose from Right AZ-Plane
        if AZsummary(i,1) == positionChoice
            magnitudeFFT(i,1) = AZsummary(i,3);
            magnitudeFFT(i,2) = AZsummary(i,3);
        end
    end
end

% if positionChoice <= beta   % choose from Left AZ-Plane
%     AZplane(:,positionChoice) = 20 * log10(1e-6 + magnitudeFFT(:,1)); % for plot AZ plane use
% else
%     AZplane(:,positionChoice) = 20 * log10(1e-6 + magnitudeFFT(:,2)); % for plot AZ plane use
% end

    
% apply the magnitude to the original
outputFFTL = magnitudeFFT(:,1) .* inputFFT(:,1) ./ abs(inputFFT(:,1));
outputFFTR = magnitudeFFT(:,2) .* inputFFT(:,2) ./ abs(inputFFT(:,2));


% end % for plot AZ plane use
% 
% if (plotAZplane)
%     mesh(AZplane(1:600,:))  % for plot AZ plane use
% end

outputSample = zeros(M,2);
outputSample(:,1) = ifft(outputFFTL) .*w;
outputSample(:,2) = ifft(outputFFTR) .*w;
% outputSample(:,1) = ifft(outputFFTL);
% outputSample(:,2) = ifft(outputFFTR);


%%

outputSample = real(outputSample);



% step5: reconstruct the complex spectrum using the original phase values
% step6: IFFT
% step7: apply a window function
% step8: overlap-add the frame to the output

end
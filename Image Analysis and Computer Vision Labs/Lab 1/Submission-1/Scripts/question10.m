F = [zeros(56, 128); ones(16, 128); zeros(56, 128)];
G = F';

% Shown way
subplot(3, 1, 1);
showgrey(F .* G);
title('F .* G');
subplot(3, 1, 2);
showfs(fft2(F .* G));
title('FFT of F .* G');

% Alternative way
% MATLAB's implementation requires that
% we scale down the result by length
Fhat = fft2(F)/128;
Ghat = fft2(G)/128;
con = conv2(Fhat, Ghat);
subplot(3, 1, 3);
showfs(con(1:128, 1:128));
title('Convolution of Fhat and Ghat');
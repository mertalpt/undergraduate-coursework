F = [zeros(56, 128); ones(16, 128); zeros(56, 128)];
G = F';
H = F + 2 * G;

Fhat = fft2(F);
Ghat = fft2(G);
Hhat = fft2(H);

% Display without hat
figure();
subplot(2, 2, 1);
showgrey(log(1 + abs(F)));
title('F');

subplot(2, 2, 2);
showgrey(log(1 + abs(G)));
title('G');

subplot(2, 2, 3);
showgrey(log(1 + abs(H)));
title('H');

subplot(2, 2, 4);
showgrey(log(1 + abs(fftshift(H))));
title('Shifted H');

% Display with hat
figure();
subplot(2, 2, 1);
showgrey(log(1 + abs(Fhat)));
title('Fhat');

subplot(2, 2, 2);
showgrey(log(1 + abs(Ghat)));
title('Ghat');

subplot(2, 2, 3);
showgrey(log(1 + abs(Hhat)));
title('Hhat');

subplot(2, 2, 4);
showgrey(log(1 + abs(fftshift(Hhat))));
title('Shifted Hhat');
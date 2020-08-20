F = [zeros(60, 128); ones(8, 128); zeros(60, 128)] .* ...
    [zeros(128, 48) ones(128, 32) zeros(128, 48)];

% Scaling
figure();
subplot(2, 1, 1);
showgrey(F);
title('F');

Fhat = fft2(F);
subplot(2, 1, 2);
showfs(Fhat);
title('Fhat');

% Rotation
rotation_experiment(F, 30);
rotation_experiment(F, 60);
rotation_experiment(F, 90);

function rotation_experiment(F, alpha)
    G = rot(F, alpha);
    Ghat = fft2(G);
    Hhat = rot(fftshift(Ghat), -alpha);

    figure();
    subplot(3, 1, 1);
    showgrey(G);
    axis on;
    title(sprintf('G (alpha: %d)', alpha));

    subplot(3, 1, 2);
    showfs(Ghat);
    title(sprintf('Ghat (alpha: %d)', alpha));
    
    subplot(3, 1, 3);
    showgrey(log(1 + abs(Hhat)));
    title(sprintf('Hhat (-alpha: -%d)', alpha));
end
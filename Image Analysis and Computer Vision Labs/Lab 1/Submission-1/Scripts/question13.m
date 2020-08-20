% Call for images
alpha = 10^(-10);

experiment(phonecalc128, alpha);
experiment(few128, alpha);
experiment(nallo128, alpha);

function experiment(image, alpha)
    % Processed image
    phase_preserved = pow2image(image, alpha);
    magnitude_preserved = randphaseimage(image);

    % Display
    figure();
    
    subplot(3, 1, 1);
    showgrey(image);
    title(sprintf('Original - alpha: (%.10g)', alpha));
    
    subplot(3, 1, 2);
    showgrey(phase_preserved);
    title('Magnitude Random, Phase Preserved');
    
    subplot(3, 1, 3);
    showgrey(magnitude_preserved);
    title('Magnitude Preserved, Phase Random');
end
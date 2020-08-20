values = [0.1 0.3 1.0 5.0 25.0];

for value = values
    impulse_response(value);
end

function impulse_response(t)
    psf = gaussfft(deltafcn(128, 128), t);
    cov = variance(psf);
    ideal_cov = t * eye(2);
    
    % Display as a figure
    figure();
    subplot(3, 1, 1);
    showgrey(psf);
    title(sprintf('Impulse response (t: %g)', t));
    
    subplot(3, 1, 2);
    imshow(cov);
    title(sprintf('Covariance matrix (factor: %g)', cov(1, 1)));
    
    subplot(3, 1, 3);
    imshow(ideal_cov);
    title(sprintf('Ideal covariance matrix (factor: %g)', ideal_cov(1, 1)));
end
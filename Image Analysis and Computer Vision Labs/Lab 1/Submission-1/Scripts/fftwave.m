% Procedure to experiment with FFT
function fftwave(u, v, sz)
    % Check arguments
    if (nargin < 2)
        error('Requires at least two input arguments.');
    end
    if (nargin == 2)
        sz = 128;
    end
    
    % Construct image base
    Fhat = zeros(sz);
    Fhat(u, v) = 1;
    
    % Calculate the image through inverse Fourier transform
    F = ifft2(Fhat);
    Fabsmax = max(abs(F(:)));
    
    % Display raw image
    subplot(3, 2, 1);
    showgrey(Fhat);
    title(sprintf('Fhat: (u, v) = (%d, %d)', u, v));
    
    % Shift the origin into the image center
    if (u <= sz/2)
        uc = u - 1;
    else
        uc = u - 1 - sz;
    end
    if (v <= sz/2)
        vc = v - 1;
    else
        vc = v - 1 - sz;
    end
    
    % let w_uc = 2PI*uc/sz
    % let w_vc = 2PI*vc/sz
    % then wavelength = 2PI/sqrt(w_uc^2 + w_vc^2)
    % simplifies to sz/sqrt(uc^2 + vc^2)
    wavelength = sz/sqrt(uc^2 + vc^2);
    
    % amplitude = 1 / N^2
    amplitude = 1/sz^2;
    
    % Display plots of other information
    subplot(3, 2, 2);
    showgrey(fftshift(Fhat));
    title(sprintf('centered Fhat: (uc, vc) = (%d, %d)', uc, vc));
    
    subplot(3, 2, 3);
    showgrey(real(F), 64, -Fabsmax, Fabsmax);
    title('real(F)');
    
    subplot(3, 2, 4);
    showgrey(imag(F), 64, -Fabsmax, Fabsmax);
    title('imag(F)');
    
    subplot(3, 2, 5);
    showgrey(abs(F), 64, -Fabsmax, Fabsmax);
    title(sprintf('abs(F) (amplitude %f)', amplitude));
    
    subplot(3, 2, 6);
    showgrey(angle(F), 64, -pi, pi);
    title(sprintf('angle(F) (wavelength %f)', wavelength));
end
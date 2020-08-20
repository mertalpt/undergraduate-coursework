function result = gaussfft(pic, t)
    % Step 1: Generate Gaussian filter
    [xsz, ysz] = size(pic);
    x_vec = -xsz/2 : xsz/2 - 1;
    y_vec = -ysz/2 : ysz/2 - 1;
    [X, Y] = meshgrid(x_vec, y_vec);
    gauss_sampled = fftshift((1 / (2*pi*t)) * exp(-(X.^2 + Y.^2) / (2*t)));
    
    % Step 2: Apply Fourier transform to filter and image
    pic_ft = fft2(pic);
    gauss_ft  = fft2(gauss_sampled);
    
    % Step 3: Multiply transforms
    mult_ft = pic_ft .* gauss_ft;
    
    % Step 4: Invert result
    result = ifft2(mult_ft);
end
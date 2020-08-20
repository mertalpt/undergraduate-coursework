img = phonecalc256;
smoothimg_gauss = img;
smoothimg_lp = img;
N=5;
t = 1;
threshold = 0.5;
for i=1:N
    if i>1
        % generate subsampled versions
        img = rawsubsample(img);
        smoothimg_gauss = gaussfft(smoothimg_gauss, t);
        smoothimg_gauss = rawsubsample(smoothimg_gauss);
        smoothimg_lp = gaussfft(smoothimg_lp, threshold);
        smoothimg_lp = rawsubsample(smoothimg_lp);
    end
    
    subplot(3, N, i)
    showgrey(img)
    if i == 1
        title('Unfiltered');
    end
    
    subplot(3, N, i+N)
    showgrey(smoothimg_gauss)
    if i == 1
        title(sprintf('Gaussian filter(t: %g)', t));
    end
    
    subplot(3, N, i+2*N)
    showgrey(smoothimg_lp)
    if i == 1
        title(sprintf('Ideal filter(t: %g)', threshold));
    end
end
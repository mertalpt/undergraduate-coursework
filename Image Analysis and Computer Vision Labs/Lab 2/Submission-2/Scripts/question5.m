image = few256;
scales = [0.0001, 1.0, 4.0, 16.0, 64.0];

experiment(image, scales);

function experiment(img, scales)
    len = length(scales);
    
    i = 1;
    for scale = scales
        pic = discgaussfft(img, scale);
        
        subplot(2, len, i);
        showgrey(pic);
        axis('image');
        axis('ij');
        title(sprintf('Scale: %g', scale));
        
        subplot(2, len, i+len);
        showgrey(lvvvtilde(pic, 'same') < 0);
        axis('image');
        axis('ij');
        title(sprintf('Scale: %g', scale));
        
        i = i + 1;
    end
end
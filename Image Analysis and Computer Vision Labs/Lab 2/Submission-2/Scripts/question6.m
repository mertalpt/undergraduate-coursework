image = few256;
scales = [0.0001, 1.0, 4.0, 16.0, 64.0];

experiment(image, scales, 0.4);

function experiment(img, scales, delta)
    len = length(scales);
    
    i = 1;
    for scale = scales
        pic = discgaussfft(img, scale);
        pic_vvv = lvvvtilde(pic, 'same');
        pic_vv = lvvtilde(pic, 'same');
        pic_vvv_lt = pic_vvv >= 0;
        pic_vv_abs = log(1 + abs(pic_vv));
        pic_vv_abs_delta = pic_vv_abs >= delta;
        
        subplot(4, len, i);
        showgrey(pic);
        axis('image');
        axis('ij');
        title(sprintf('Scale: %g', scale));
        
        subplot(4, len, i+len);
        showgrey(pic_vvv_lt);
        axis('image');
        axis('ij');
        title(sprintf('Scale: %g', scale));
        
        subplot(4, len, i+len*2);
        showgrey(pic_vv_abs_delta);
        axis('image');
        axis('ij');
        title(sprintf('Scale: %g', scale));
        
        subplot(4, len, i+len*3);
        showgrey(pic_vvv_lt .* pic_vv_abs_delta);
        axis('image');
        axis('ij');
        title(sprintf('Scale: %g', scale));
        
        i = i + 1;
    end
end
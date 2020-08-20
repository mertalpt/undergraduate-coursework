function edgecurves = extractedge(img, scale, threshold, shape)
    if (nargin < 3)
        threshold = 0;
        shape = 'same';
    elseif (nargin < 4)
        shape = 'same';
    end
    
    gauss_img = discgaussfft(img, scale);
    v = lv(gauss_img, shape);
    vv = lvvtilde(gauss_img, shape);
    vvv = lvvvtilde(gauss_img, shape);
    
    v_mask = (v > threshold) - 0.1;
    vvv_mask = (vvv < 0);
    
    edgecurves = zerocrosscurves(vv, vvv_mask);
    edgecurves = thresholdcurves(edgecurves, v_mask);
end
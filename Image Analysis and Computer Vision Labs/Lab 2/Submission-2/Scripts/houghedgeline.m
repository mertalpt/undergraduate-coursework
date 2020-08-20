function [linepar, acc] = ...
    houghedgeline(img, scale, gradmagnthreshold, ...
        nrho, ntheta, nlines, verbose)
    
    curves = extractedge(img, scale, gradmagnthreshold, 'same');
    magnitude = lv(img);
    
    if (verbose > 0)
       subplot(1, 3, 1);
       showgrey(img);
       title('Original');
    end
    
    [linepar, acc] = houghline(curves, magnitude, nrho, ntheta, ...
        gradmagnthreshold, nlines, verbose);
end
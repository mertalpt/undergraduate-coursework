bandwidths = [20.0];
thresholds = [0.3];
min_areas = [20];
max_depths = [5];

image = 'tiger3.jpg';

for bw = bandwidths
    for thresh = thresholds
        for area = min_areas
            for depth = max_depths
                experiment(image, bw, thresh, area, depth);
            end
        end
    end
end

function experiment(image, colour_bandwidth, ncuts_thresh, min_area, max_depth)
    radius = 10;              % maximum neighbourhood distance
    scale_factor = 0.4;      % image downscale factor
    image_sigma = 2.0;       % image preblurring scale

    I = imread(image);
    I = imresize(I, scale_factor);
    Iback = I;
    d = 2*ceil(image_sigma*2) + 1;
    h = fspecial('gaussian', [d d], image_sigma);
    I = imfilter(I, h);

    segm = norm_cuts_segm(I, colour_bandwidth, radius, ncuts_thresh, min_area, max_depth);
    Inew = mean_segments(Iback, segm);
    I = overlay_bounds(Iback, segm);
    
    figure();
    subplot(2, 1, 1);
    imshow(Inew);
    title(sprintf('Inew | BW: %g, T: %g, A: %g, D: %g', colour_bandwidth, ncuts_thresh, min_area, max_depth));
    
    subplot(2, 1, 2);
    imshow(I);
    title(sprintf('I | BW: %g, T: %g, A: %g, D: %g', colour_bandwidth, ncuts_thresh, min_area, max_depth));
end
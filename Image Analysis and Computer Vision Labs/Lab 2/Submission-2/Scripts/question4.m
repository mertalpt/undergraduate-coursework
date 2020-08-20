image = godthem256;
scales = [0.0001 1.0 4.0,16.0 64.0];

for scale = scales
    figure();
    experiment(image, scale);
    title(sprintf('Scale: %g', scale));
end

function experiment(img, scale)
    contour(lvvtilde(discgaussfft(img, scale), 'same'), [0 0]);
    axis('image');
    axis('ij');
end
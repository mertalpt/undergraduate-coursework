image = godthem256;

scales = [4.0 8.0 16.0];
thresholds = [4.0 16.0 32.0];

figure();
len_s = length(scales);
len_t = length(thresholds);
i = 1;
for scale = scales
   for threshold = thresholds
      subplot(len_s, len_t, i);
      curves = extractedge(image, scale, threshold);
      overlaycurves(image, curves);
      title(sprintf('S: %g, T: %g', scale, threshold));
      i = i + 1;
   end
end
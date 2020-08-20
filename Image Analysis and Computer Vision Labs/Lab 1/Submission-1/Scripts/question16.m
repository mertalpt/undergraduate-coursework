ts = [0.1 1.0 16.0 256.0];

pic = phonecalc128;
i = 1;
figure();
for t = ts
    gauss_pic = gaussfft(pic, t);
    subplot(length(ts), 1, i);
    showgrey(gauss_pic);
    title(sprintf('t: %g', t));
    i = i + 1;
end

pic = few128;
i = 1;
figure();
for t = ts
    gauss_pic = gaussfft(pic, t);
    subplot(length(ts), 1, i);
    showgrey(gauss_pic);
    title(sprintf('t: %g', t));
    i = i + 1;
end

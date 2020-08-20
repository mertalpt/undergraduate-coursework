office = office256;

add = gaussnoise(office, 16);
sap = sapnoise(office, 0.1, 255);

experiment(add, [0.25 1 4 16], @gaussfft, 'Gaussian', 'Gaussian');
experiment(sap, [0.25 1 4 16], @gaussfft, 'Gaussian', 'S&P');

experiment(add, [2 3 4 5], @medfilt, 'Median', 'Gaussian');
experiment(sap, [2 3 4 5], @medfilt, 'Median', 'S&P');

experiment(add, [0.1 0.25 0.5 1], @ideal, 'Ideal', 'Gaussian');
experiment(sap, [0.1 0.25 0.5 1], @ideal, 'Ideal', 'S&P');

function experiment(pic, ts, filter, filter_name, noise_type)
    figure();

    % Originals
    subplot(1, length(ts)+1, 1);
    showgrey(pic);
    title(sprintf('N: %s / F: %s', noise_type, filter_name));

    i = 2;
    for t = ts
       subplot(1, length(ts)+1, i);
       i = i + 1;
       
       curr = filter(pic, t);
       showgrey(curr);
       title(sprintf('t: %g', t));
    end
end
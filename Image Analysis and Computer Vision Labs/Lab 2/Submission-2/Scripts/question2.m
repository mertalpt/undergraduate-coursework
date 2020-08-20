experiment(few256, [100 150 200]);
experiment(discgaussfft(few256, 2), [50 100 150]);

experiment(godthem256, [100 200 300]);
experiment(discgaussfft(godthem256, 1), [50 100 150]);

function experiment(img, thresholds)
    % Sobel operator kernels
    g_x = [1 0 -1; 2 0 -2; 1 0 -1];
    g_y = [1 2 1; 0 0 0; -1 -2 -1];

    % Computation
    dximg = filter2(g_x, img, 'valid');
    dyimg = filter2(g_y, img, 'valid');

    % Gradient approximation
    gradmagnimg = sqrt(dximg .^2 + dyimg .^2);
    
    % Display in a figure
    figure();
    subplot(2, 3, 1);
    showgrey(img);
    title('Original');
    
    subplot(2, 3, 2);
    showgrey(gradmagnimg);
    title('Apprx. Grad. Magn.');
   
    subplot(2, 3, 3);
    histogram(gradmagnimg);
    title('Histogram of Apprx. Grad. Magn.');
    
    subplot(2, 3, 4);
    showgrey((gradmagnimg - thresholds(1) > 0));
    title(sprintf('Threshold = %g', thresholds(1)));
    
    subplot(2, 3, 5);
    showgrey((gradmagnimg - thresholds(2) > 0));
    title(sprintf('Threshold = %g', thresholds(2)));
    
    subplot(2, 3, 6);
    showgrey((gradmagnimg - thresholds(3) > 0));
    title(sprintf('Threshold = %g', thresholds(3)));
end
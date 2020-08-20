tools = few256;

% Sobel operator kernels
g_x = [1 0 -1; 2 0 -2; 1 0 -1];
g_y = [1 2 1; 0 0 0; -1 -2 -1];

% Computation
dxtools = conv2(tools, g_x, 'valid');
dytools = conv2(tools, g_y, 'valid');

figure();
subplot(3, 1, 1);
showgrey(tools);
title('Original');

subplot(3, 1, 2);
showgrey(dxtools);
title('dx');

subplot(3, 1, 3);
showgrey(dytools);
title('dy');
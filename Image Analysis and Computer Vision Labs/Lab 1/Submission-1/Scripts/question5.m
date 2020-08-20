% Points to exercise with
points = [64 65 64 65; 64 64 65 65];

% Generate plots
for point = points
    figure();
    fftwave(point(1), point(2));
end
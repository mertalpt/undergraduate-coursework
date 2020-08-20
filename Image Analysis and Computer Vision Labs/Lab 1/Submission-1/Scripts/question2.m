% Points to exercise with
points = [6 5 4 1; 1 4 5 6];

% Generate plots
for point = points
    figure();
    fftwave(point(1), point(2));
end
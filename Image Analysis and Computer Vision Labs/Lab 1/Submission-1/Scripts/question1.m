% Points to exercise with
points = [5 9 17 17 5 125; 9 5 9 121 1 1];

% Generate plots
for point = points
    figure();
    fftwave(point(1), point(2));
end
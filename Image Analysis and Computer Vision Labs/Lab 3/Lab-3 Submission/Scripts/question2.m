image = imread('orange.jpg');
Ls = [1 2 3 4 5 6];
K = 10;
seed = 100;

for L = Ls
    [segmentation, centers] = kmeans_segm(image, K, L, seed);
    segmented = zeros(size(segmentation, 1), size(segmentation, 2), 3);

    for i = 1 : size(segmentation, 1)
        for j = 1 : size(segmentation, 2)
           curr_center = segmentation(i, j);
           color = centers(curr_center, :);
           segmented(i, j, :) = color;
        end
    end

    figure();
    imshow(uint8(segmented));
    title(sprintf('L: %d', L));
end
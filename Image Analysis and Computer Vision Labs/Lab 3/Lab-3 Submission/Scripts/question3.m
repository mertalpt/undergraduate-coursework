image = imread('tiger1.jpg');
Ks = [3 4 5 6 7 8];
Ls = [2 4 6 8];
seed = 161;
row = 0;

for L = Ls
    row = row + 1;
    col = 0;
    for K = Ks
        col = col + 1;
        [segmentation, centers] = kmeans_segm(image, K, L, seed);
        segmented = zeros(size(segmentation, 1), size(segmentation, 2), 3);

        for i = 1 : size(segmentation, 1)
            for j = 1 : size(segmentation, 2)
               curr_center = segmentation(i, j);
               color = centers(curr_center, :);
               segmented(i, j, :) = color;
            end
        end

        subplot(length(Ls), length(Ks), (row-1) * length(Ks) + col);
        imshow(uint8(segmented));
        title(sprintf('K: %d, L: %d', K, L));
    end
end
function [segmentation, centers, iterations] = kmeans_segm_controlled(image, K, threshold, seed)
    % Seed RNG
    rng(seed);

    % Reshape image
    [heigth, width, ~] = size(image);
    img2d = reshape(image, heigth * width, 3);
    img2d = double(img2d);

    % Randomly initialize the K cluster centers
    rand_indices = randperm(size(img2d, 1), K);
    centers = img2d(rand_indices, :);
    
    % Compute all distances between pixels and cluster centers
    % Will be done inside the loop
    iterations = 0;
    delta = threshold + 1;
    while (delta > threshold)
        iterations = iterations + 1;
        
        % Assign each pixel to the cluster center for which the distance is
        % minimum
        [~, nearest] = pdist2(centers, img2d, 'euclidean', 'Smallest', 1);
    
        % Count cluster sizes for optimization
        count = zeros(1, K);
        for cluster = nearest
           count(1, cluster) = count(1, cluster) + 1;
        end
        % Save old count
        old_count = count;
        
        % Separate clusters
        numpixels = size(img2d, 1);
        clusters = cell(1, K);
        for j = 1 : K
           clusters{j} = zeros(count(1, K), 3); 
        end
        for j = 1 : numpixels
           cluster_idx = nearest(1, j);
           cluster_size = count(1, cluster_idx);
           color = img2d(j, :);
           clusters{cluster_idx}(cluster_size, :) = color;
           count(1, cluster_idx) = cluster_size - 1;
        end

        % Recalculate centers
        for j = 1 : K
            if (size(clusters{K}, 1) ~= 0)
                centers(K, :) = mean(clusters{K}, 1);
            end
        end
        
        % Calculate difference
        new_count = zeros(1, K);
        for cluster = nearest
           new_count(1, cluster) = new_count(1, cluster) + 1;
        end
        delta = max(abs(new_count - old_count));
    end

    % Cast cluster matrix
    segmentation = reshape(uint8(nearest), [heigth, width]);
    
    % Done.
end
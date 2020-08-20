function result = lvvvtilde(img, shape)
    if (nargin < 2)
        shape = 'same';
    end
    
    dxmask = zeros(5);
    dymask = zeros(5);
    dxmask(3, 2:4) = [-1/2, 0, 1/2];
    dymask(2:4, 3) = [-1/2; 0; 1/2];
    
    L_x = filter2(dxmask, img, shape);
    L_y = filter2(dymask, img, shape);
    
    L_xx = filter2(dxmask, L_x, shape);
    L_yy = filter2(dymask, L_y, shape);
    
    L_xxx = filter2(dxmask, L_xx, shape);
    L_xxy = filter2(dymask, L_xx, shape);
    L_yyx = filter2(dxmask, L_yy, shape);
    L_yyy = filter2(dymask, L_yy, shape);
    
    result = L_x.^3 .* L_xxx + 3 .* L_x.^2 .* L_y .* L_xxy + ...
             3 .* L_x .* L_y.^2 .* L_yyx + L_y.^3 .* L_yyy;
end
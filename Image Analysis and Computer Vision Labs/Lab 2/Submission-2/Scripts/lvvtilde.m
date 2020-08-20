function result = lvvtilde(img, shape)
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
    L_xy = filter2(dymask, L_x, shape);
    L_yy = filter2(dymask, L_y, shape);
    
    result = L_x.^2 .* L_xx + 2 .* L_x .* L_y .* L_xy + L_y.^2 .* L_yy;
end
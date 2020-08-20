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
    
    result = L_x.^2 + L_y.^2;
end
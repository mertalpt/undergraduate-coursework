function [linepar, acc] = ...
    houghline(curves, magnitude, ...
        nrho, ntheta, threshold, nlines, verbose)
   
    % Check if input is valid
    assert(nargin == 6 || nargin == 7);
    assert(nrho > 0 && ntheta > 0 && nlines > 0);
    
    if (nargin == 6)
        verbose = 0;
    end
    
    % Allocate accumulator space
    acc = zeros(nrho, ntheta);
    
    % Define a coordinate system in the accumulator space
    modulus = sqrt(size(magnitude, 1)^2 + size(magnitude, 2)^2);
    range_theta = linspace(-pi, pi, ntheta);
    range_rho = linspace(-modulus, modulus, nrho);
    
    % Loop over all the input curves (cf. pixelplotcurves)
    index = 1;
    while index <= size(curves, 2)
        polylength = curves(2, index);
        index = index + 1;

        % For each point on the curve
        for polyindex = 1 : polylength
            x = curves(2, index);
            y = curves(1, index);

            % Check if valid point with respect to threshold
            if (magnitude(round(x), round(y)) > threshold)
                % Loop over a set of theta values
                for thetaindex = 1 : ntheta
                    curr_theta = range_theta(thetaindex);
                    
                    % Computer rho for each theta value
                    rho = x*cos(curr_theta) + y*sin(curr_theta);
                    
                    % Compute index values in the accumulator space
                    [~, rhoindex] = min(abs(range_rho - rho));
                    
                    % Update the accumulator
                    % acc(rhoindex, thetaindex) = ...
                    %    acc(rhoindex, thetaindex) + 1;
                    
                    % Question 10 increment
                    magn = magnitude(round(x), round(y));
                    acc(rhoindex, thetaindex) = ...
                        acc(rhoindex, thetaindex) + log(1 + abs(magn));
                end
            end
            
            index = index + 1;
        end
    end
    
    % Extract local maxima from the accumulator
    [pos, value] = locmax8(acc);
    [~, indexvector] = sort(value);
    nmaxima = size(value, 1);
    
    % Delimit the number of responses if necessary
    nlines = min([nlines, nmaxima]);
    
    % Compute a line for each one of the strongest responses in the 
    % accumulator
    linepar = zeros(nlines, 2);
    for index = 1 : nlines
        rhoidxacc = pos(indexvector(nmaxima - index + 1), 1);
        thetaidxacc = pos(indexvector(nmaxima - index + 1), 2);
        linepar(index, :) = [range_rho(rhoidxacc), range_theta(thetaidxacc)];
    end
    
    % Overlay these curves on the gradient magnitude image
    if (verbose > 0)
        outcurves = zeros(2, 4*nlines);
        for index = 1 : nlines
            rho = linepar(index, 1);
            theta = linepar(index, 2);
            x = rho * cos(theta);
            y = rho * sin(theta);
            dx = modulus * sin(theta);
            dy = modulus * -cos(theta);
            
            outcurves(1, 4*(index-1) + 1) = 0;
            outcurves(2, 4*(index-1) + 1) = 3;
            outcurves(2, 4*(index-1) + 2) = x-dx;
            outcurves(1, 4*(index-1) + 2) = y-dy;
            outcurves(2, 4*(index-1) + 3) = x;
            outcurves(1, 4*(index-1) + 3) = y;
            outcurves(2, 4*(index-1) + 4) = x+dx;
            outcurves(1, 4*(index-1) + 4) = y+dy;
        end
        
        % Display
        
        subplot(1, 3, 2);
        overlaycurves(magnitude, outcurves);
        axis([1 size(magnitude, 2) 1 size(magnitude, 1)]);
        title('Lines');
        
        subplot(1, 3, 3);
        showgrey(acc);
        title('Accumulator');
    end
    
    % Return the output data
    % Calculated values should return themselves
end
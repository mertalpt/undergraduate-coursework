ntheta = 200;
nrho = 200;
image = houghtest256;
func = @() houghedgeline(image, 1.0, 250, nrho, ntheta, 5, 0);
time = timeit(func);
display(time);
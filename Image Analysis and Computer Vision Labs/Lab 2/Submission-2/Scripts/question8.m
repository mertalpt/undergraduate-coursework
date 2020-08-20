testimage1 = triangle128;
smalltest1 = binsubsample(testimage1);

testimage2 = houghtest256;
smalltest2 = binsubsample(binsubsample(testimage2));

% Image to use

experiment(smalltest1);
experiment(smalltest2);
experiment(few256);
experiment(phonecalc256);
experiment(godthem256);

function experiment(image)
    figure();
    houghedgeline(image, 1.0, 250, 256, 256, 10, 1);
end
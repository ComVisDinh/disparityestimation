
thisimage = imread('sceneFlow_disp.png');
thisimage = im2double(thisimage);
maxD = 0.7;
thisimage = thisimage ./ maxD;
thisimage = im2uint8(thisimage);
rgbImage = ind2rgb(thisimage, jet(256));
imwrite(rgbImage,'sceneFlow_disp_color.png');


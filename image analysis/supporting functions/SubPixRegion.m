function region=SubPixRegion(arr,extent,x,y)

%region=SubPixRegion(arr,extent,x,y)
%
%SubPixRegion.m extracts a region of an image, centered around the subpixel
%locations given by x and y. Rather than simply centering at the rounded
%values of x and y, it extracts all images from pixels surrounding the
%subpixel location, and weights the respective images appropriately.
%Effectively, it is creating a custom linearly interpolated image centered
%at the subpixel location. 
%
%INCLUDE:   
%
%INPUTS:    ARR:        The input image array
%           EXTENT:     The number of pixels (+ and -) to extract. For
%                       example, an extent of 3 would yield a 7x7 region
%                       centered at the subpixel location (7 = 2*3+1)
%           X:          The subpixel x location
%           Y:          The subpixel y location
%
%OUTPUTS:   REGION:     The final, subpixel interpolated subregion
%
%Written by Stephen Anthony, U. Illinois Urbana-Champaign, 11/2009
%Last modified by Stephen Anthony on 11/20/2009

%Determine the floor and ceiling values for the desired points.
fx=floor(x);
fy=floor(y);

%Determine the offsets
dx=x-fx;
dy=y-fy;

%Extract the appropriate regions. Collectively, these four regions are the
%appropriate regions, each centered around one pixel bounding the subpixel
%location specified.
range=-extent:extent;
ff=arr(fy+range,fx+range);
cf=arr(fy+range,fx+1+range);
fc=arr(fy+1+range,fx+range);
cc=arr(fy+1+range,fx+1+range);

%The appropriate weights for the floor values are (1-dx) or (1-dy), and for
%the ceiling values are dx or dy
region= (1-dx)*(1-dy)*ff + (1-dx)*dy*cf + dx*(1-dy)*fc + dx*dy*cc;
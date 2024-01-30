function y=gaussian(x,x0,H,w)

%y=gaussian(x,x0,H,w)
%
%INCLUDE:
%
%This function is designed to return a Gaussian peak from a vector x. 
%
%Inputs:    x:  The vector of x coordinates to construct a peak from.
%           x0: The point around which to center the peak.
%           H:  The height of the Gaussian.
%           w:  The square root of the variance
%
%Outputs:   y:  The vector corresponding to the gaussian peak.
%
%Written by Stephen Anthony, U. Illinois Urbana-Champaign 6/2004
%Last modified by Stephen Anthony, 6/30/2004

y=H*exp(-(x-x0).^2/(2*w^2));

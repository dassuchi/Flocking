function [sdev,avg]=stdbackground(arr)

%[sdev,avg]=stdbackground(arr)
%
%STDBACKGROUND.m is designed to find a close approximation of the standard
%deviation of the background, or the noise level, regardless of the
%influence of signal. In order to do so, it makes a few assumptions:
%   1) for a histogram of pixel intensities, the highest points will
%   correspond to the background signal
%   2) the background signal (noise) is approximately gaussian
%   3) the image is flat
%
%INCLUDE:   gauss1dfit.m
%
%INPUTS:    ARR:        The image for which these values need to be
%                       determined
%
%OUTPUTS:   SDEV:       The standard deviation (width of the gaussian) of
%                       the background
%           AVG:        The mean value of the background pixels
%
%Written by Stephen Anthony 10/2009 U. Illinois Urbana-Champaign
%Last Modified by Stephen Anthony on 11/06/2009

%We need to construct the appropriate histogram, with proper bin size

%Many times, we will have zeroed out sections of the image. Most likely,
%any values which are EXACTLY zero are not real. Remove these
tmp=arr==0;
arr=arr(~tmp);

%First, we need the median and the standard deviation
med=median(arr);
s=std(arr);

%Now, we can construct an appropriate histogram
x=(med-5*s):(.05*s):(med+5*s);
z=hist(arr,x);
z(1)=0;z(length(z))=0;

%Also, we significantly reduce the noise if we set any values which are
%less than 1/20th the largest peak to zero, at least initially
z2=z;
z2(z<.1*max(z))=0;

%For the first round, a Gaussian fit weighted by the values of the
%histrogram itself will manage a good job of applying the majority of the
%weight to the background pixels. 

%Apply a rapid 1d Gaussian fit, initially significantly overweighting the
%peak
[xc,Amp,width]=gauss1dfit(z,x,z2);

for j=1:5
    %Now that we have a reasonable guess, we can exclude much of the
    %non-background data by redoing with a more accurate weight, in this case
    %weighting by the value obtained by the first fit. 
    wght=gauss([xc Amp norm(width)],x,0);
    [xc,Amp,width]=gauss1dfit(z,x,wght);
end

sdev=norm(width);
avg=xc;

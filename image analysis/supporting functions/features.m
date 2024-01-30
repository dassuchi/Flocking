function ftr=features(arr,radius,optional,psf)

%ftr=features(arr,radius,optional)
%
%FEATURES.m locates all local maxima in the image, and depending upon the
%input conditions, determines which constitute features, as well as
%computing some basic properties of each feature
%
%Inspired by:
%   J.C. Crocker and D.G. Grier, J. Colloid Sci. *179*, 298 (1996)
%In contrast to that method, this method (optionally) uses a rapid 
%2D-Gaussian fitting algorithm to achieve subpixel resotion. 
%   Anthony, S.M., and Granick, S. Langmuir *25*, 8152 (2009).
%Additionally, the computed parameters of the feature are also found using 
%subpixel resolution. 
%
%INCLUDE:   gaussian.m
%           conv1and1.m
%           localextrema.m
%           stdbackground.m
%           gauss2dcirc.m
%           SubPixRegion.m
%           delaunaynSegs.m
%               gauss1dfit.m
%               isodd.m
%
%INPUTS:    ARR:        The input image array. This should be background
%                       corrected to provide a flat, zero background level.
%           RADIUS:     The radius of the features to be found. Must be a
%                       positive integer. 
%           OPTIONAL:   A structure, which need not be provided. If
%                       provided, it allows many optional inputs, allowing
%                       much greater control.
%           .peakmin:   Only features whose central pixel is greater than
%                       this value (defaults to 0) will be considered
%                       features
%           .intbright: If set, then only features whose total brightness
%                       is greater than this value will be retained
%           .eccentric: If set, then only features whose eccentricity
%                       is less than this value will be retained
%           .minradius: If set, then only features whose radius of gyration
%                       is greater than this value will be retained
%           .gauss2d:   The method used to achieve subpixel localization. 
%                       If set to true (default=true), then a rapid 2d
%                       gaussian fitting is performed. If false, then a
%                       simpler centroid method is employed. Default is
%                       true.
%           .minsep:    The minimum spacing between points identified as
%                       features. (default=DIAMETER)
%
%OUTPUTS:   FTR:        FTR is an array list, with each row giving
%                       parameters for one feature. 
%           FTR(:,1)    The subpixel x position, in pixels
%           FTR(:,2)    The subpixel y position, in pixels
%           FTR(:,3)    The total brightness
%           FTR(:,4)    The radius. Depending upon the subpixel
%                       localization method employed, the definition varies
%                       slightly.
%                       optional.gauss2d=true
%                           the radius here is the width of the gaussian
%                       optional.gauss2d=false
%                           the radius here is the radius of gyration
%           FTR(:,5)    The eccentricity of the features
%           FTR(:,6)    The SSE for the gaussian fitting
%Written by Stephen Anthony 11/2009 U. Illinois Urbana-Champaign
%Last modified on 01/28/10 by Stephen Anthony



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Confirm whether the optional inputs were provided, and provide defaults
%for any essential ones not provided
if ~exist('optional','var')
    optional=struct();
end

if ~isfield(optional,'peakmin')
    optional.peakmin=0;
end
if ~isfield(optional,'gauss2d')
    optional.gauss2d=true;
end
if ~isfield(optional,'minsep')
    optional.minsep=2*radius+1;
end

if ~isfield(optional,'checkintbright')
    optional.checkintbright=true;
end

if ~isfield(optional,'checkminsep')
    optional.checkminsep=true;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Locate potential features

%Create a gaussian smoothed copy, useful for finding the maxima
gaussx=gaussian(-radius:radius,0,1,radius/5);
arr2=conv1and1(arr,gaussx,1);

%Find all local maxima, as they are potential features
compmat=ones(3);
[yy,maxima]=localextrema(arr2,compmat);

%Additionally, to be a potential feature, the point has to be greater than
%the minimum peak threshold
above=arr2>=optional.peakmin;
potential=maxima & above;

%Determine the location of all potential features. Exclude any features
%which are too near an edge, and hence incomplete
N=size(arr);
[y,x]=find(potential((radius+2):(N(1)-radius-1),(radius+2):(N(2)-radius-1)));
x=x+radius+1;
y=y+radius+1;
SSE = zeros(size(x));        
        
%Determine the number of potential features
Npot=length(x);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Create some filters which will be needed

%Create a circular filter, of diameter specified
tmpdisk=fspecial('disk',radius);
circ=ceil(tmpdisk-tmpdisk((radius+1),(radius+1))/3);

precision=0.1;

[x_coor y_coor]=meshgrid(1:2*radius+1);
[x1_coor y1_coor]=meshgrid(1:precision:2*radius+1);
%Create a filter for the radius of gyration
[xgrid,ygrid]=meshgrid(-radius:radius,-radius:radius);
[xgrid2,ygrid2]=meshgrid(-radius:precision:radius,-radius:precision:radius);
filtgyr=xgrid.^2+ygrid.^2;

%Create a matrix showing the angle around the center for use in
%calculating the eccentricity
theta=atan2(ygrid,xgrid);

%Construct masks for the eccentricity
CosMask=cos(theta*2);
CosMask(radius+1,radius+1)=0;
SinMask=sin(theta*2);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%More accurate results are obtained if we determine the position with
%subpixel resolution at this point. By default, we will employ a rapid 2D
%gaussian fit, which is generally the most precise. Optionally, instead a
%centroid method may be applied. 

if optional.gauss2d
    %Use the default, rapid 2D gaussian fit

    %We need to find the approximate noiselevel of the image
    noiselevel=stdbackground(arr);

    %Initialize
    Width=zeros(Npot,1);
    for j=1:Npot
        %Extract the appropriate region of the image
        regsquare=(arr((y(j)-radius:y(j)+radius),(x(j)-radius:x(j)+radius)));
        
        %Apply the circular filter, getting the region
        region=circ.*regsquare;
        region2=interp2(x_coor,y_coor,region,x1_coor,y1_coor,'bicubic');
        
        %Apply a rapid, matrix based 2D gaussian fit
        [dx,dy,Amp,width]=gauss2dcirc(region2,xgrid2,ygrid2,noiselevel);
        region_gauss = Amp*exp(-((xgrid - dx).^2+(ygrid-dy).^2)/2/width^2);
        SSE(j) = sum(sum(((region_gauss .*circ-region)/Amp).^2))/radius^2/pi;
%         subplot(2,2,1),imagesc(region);colormap('gray');
%         subplot(2,2,2),imagesc(region_gauss);
%         colormap('gray');
%         pause
%         if SSE(j)<0.03
            %Convert to more accurate subpixel resolution
            x(j)=x(j)+dx;
            y(j)=y(j)+dy;
            Width(j)=width;
%         else
%             x(j)=sqrt(-1);
%             y(j)=sqrt(-1);
%         end
    end
else
        psf = circ.*psf;
        psf2=interp2(x_coor,y_coor,psf,x1_coor,y1_coor,'bicubic');

        psf_intensity_span =max(max(psf2))-min(min(psf2));
    %Use a centroid method
    for j=1:Npot
        %Extract the appropriate region of the image
        regsquare=(arr((y(j)-radius:y(j)+radius),(x(j)-radius:x(j)+radius)));


        %Apply the circular filter, getting the region
        region=circ.*regsquare;
        region2=interp2(x_coor,y_coor,region,x1_coor,y1_coor,'bicubic');
        intensity_span =max(max(region2))-min(min(region2));
        scale=psf_intensity_span/intensity_span;
        region2=region2*scale;
        c = normxcorr2(psf2,region2);

%         subplot(2,2,1), imagesc(region2);
%         subplot(2,2,2), imagesc(psf2);
%         subplot(2,2,3), surf(c);
%         pause
        [yy, imax] = max(abs(c(:)));
        [ypeak, xpeak] = ind2sub(size(c),imax(1));
        dx=(xpeak-size(region2,2))*precision;
        dy=(ypeak-size(region2,1))*precision;
        
        %Compute the centroids
%         dx=sum(region(:).*xgrid(:))/sum(region(:));
%         dy=sum(region(:).*ygrid(:))/sum(region(:));
        
        %Convert to more accurate subpixel resolution
        x(j)=x(j)+dx;
        y(j)=y(j)+dy;
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%By virtue of the centroids, the center of the potential features may have
%shifted into the zone too close to the edge. Exclude these. 
bad= x<(radius+2) | y<(radius+2) | x>(N(2)-radius-1) | y>(N(1)-radius-1);
x=x(~bad);
y=y(~bad);

if optional.gauss2d
    SSE = SSE(~bad);
    Width=Width(~bad);
end
Npot=size(x,1);

%Now that we have a reduced list, it is time to determine the properties of
%these potential features:
%   integrated brightness
%   radius of gyration
%   eccentricity
%Initialize
Brightness=zeros(Npot,1);
RadiusGyration2=zeros(Npot,1);
Eccentricity=zeros(Npot,1);

for j=1:Npot
    %Extract the appropriate region of the image, WITH subpixel resolution
    regsquare=SubPixRegion(arr,radius,x(j),y(j));
    
    %Apply the circular filter, getting the region
    region=circ.*regsquare;
    
    %Calculate and record the integrated brightness
    Brightness(j)=sum(region(:));
    
    %Calculate and record the radius of gyration
    RadiusGyration2(j)=sum(sum(region.*filtgyr))/Brightness(j);
    
    %Calculate and record the eccentricity
    CosComp=region.*CosMask;
    SinComp=region.*SinMask;
    NormFactor=max(Brightness(j)-region(radius+1,radius+1),1e-6);
    EccBase=sqrt(sum(CosComp(:)).^2+sum(SinComp(:)).^2);
    Eccentricity(j)=EccBase/NormFactor;
end

%Construct the output ftr list. If 2d gaussian fitting was used, rather
%than the radius of gyration, the more precise width of the gaussian will
%be recorded. 
if optional.gauss2d
    ftr=[x y Brightness Width Eccentricity SSE];
else
    ftr=[x y Brightness sqrt(RadiusGyration2) Eccentricity];
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Some additional optional criteria may have been provided to screen
%features. If they were, employ them.

%Minimum Total Brightness
if optional.checkintbright
    if isfield(optional,'intbright')
        keep=ftr(:,3)>=optional.intbright;
        ftr=ftr(keep,:);
    else
        keep=ftr(:,3)>=0;
        ftr=ftr(keep,:);
    end
end
%Minimum Radius
if isfield(optional,'minradius')
    keep=ftr(:,4)>=optional.minradius;
    ftr=ftr(keep,:);
end

%Maximum Eccentricity
if isfield(optional,'eccentric')
    keep=ftr(:,5)<=optional.eccentric;
    ftr=ftr(keep,:);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%At this point, we have found all the features. However, since we searched
%all local maxima, there is the possibility that features were found too
%close together. Operate until no features are too close
if optional.checkminsep
    if optional.minsep>0
        notdone=true;
        while notdone
            %Find all neighbor pairs
            segs=delaunaynSegs(ftr(:,1:2));    

            %Determine the separation between such pairs
            dist2=sum((ftr(segs(:,1),1:2)-ftr(segs(:,2),1:2)).^2,2);

            %Determine which pairs are too close
            tooclose=dist2<=optional.minsep^2;

            %For each pair which is too close, keep the brighter feature
            pairs=segs(tooclose,:);
            keepfirst=ftr(pairs(:,1),3)>ftr(pairs(:,2),3);
            discard=[pairs(keepfirst,2); pairs(~keepfirst,1)];
            keep=setdiff(1:size(ftr,1),discard);
            ftr=ftr(keep,:);

            %Determine whether problem points were found, in which case it is
            %best to repeat
            notdone=length(discard);
        end
    end
end
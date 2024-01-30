function [centers,radii]=crystaltrackmethod3(centers,arr,radius,radii,percent,plotstatus)

% This is based on crystaltrackmethod. The percent is how much of the
% darker area in the particles you want to turn totally black. 0.4 works
% fine usually.
% during center tracking, some particles are at the edge, not fully
% captured in the image. Need removal first. 
keep = (centers(:,1)>radius+1 & centers(:,1)<size(arr,2)-radius-1 & centers(:,2)>radius+1 & centers(:,2)<size(arr,1)-radius-1);
centers=centers(keep,:);
radii=radii(keep);

arr=double(arr);
%Create a circular filter, of diameter specified
tmpdisk=fspecial('disk',radius);
circ=ceil(tmpdisk-tmpdisk((radius+1),(radius+1))/3);

%Create a filter for centroid weighting
[xgrid,ygrid]=meshgrid(-radius:radius,-radius:radius);


%For each feature, we now need to determine the orientation
for j=1:size(centers,1)
    %Extract the appropriate region of the image
    regsquare=SubPixRegion(arr,radius,centers(j,1),centers(j,2));

    %Apply the circular filter, getting the region
    region=circ.*regsquare;
    %Compute the centroids
%     dx=sum(region(:).*xgrid(:))/sum(region(:));
%     dy=sum(region(:).*ygrid(:))/sum(region(:));
%     [theta1,rho]=cart2pol(dx,dy);
     [counts,binlocations] = imhist(region);
     counts_cum = cumsum(counts);
     counts_scaled = counts_cum./counts_cum(end);
     percent_new = (counts_scaled(end)-counts_scaled(1)).*percent;
     ind = (abs(counts_scaled-percent_new)==min(abs(counts_scaled-percent_new)));
      threshold = binlocations(ind);
    region2 = im2bw(region,threshold(1));      
      %Determine the orientation
    dx=sum(region2(:).*xgrid(:))/sum(region2(:));
    dy=sum(region2(:).*ygrid(:))/sum(region2(:));
    [theta,rho]=cart2pol(dx,dy);
    centers(j,3:4)=[theta rho];
%     centers(j,5)=dx/sqrt(dx*dx+dy*dy);
%     centers(j,6)=dy/sqrt(dx*dx+dy*dy);
end

%  I think dx,dy is the centroid of the white region diplaced from the
%  center. Below plus points to white, minus points to black.
 if plotstatus==1
 n = radius*2;
 line([centers(:,1) centers(:,1)+n.*cos(centers(:,3))]',[centers(:,2) centers(:,2)+n.*sin(centers(:,3))]','Color','b','LineWidth',2)
 end
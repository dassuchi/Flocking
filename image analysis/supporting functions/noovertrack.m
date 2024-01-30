function [centers,radii] = noovertrack(centers,radii,radius,thresh)
% Centers tracked by imfindcircles are sorted according to some "strength",
% which characterized how good a circle it is. It may be hard to find a
% global threshold to avoid overtracking, but locally, in close-by pairs,
% the overtracked circle almost always has lower strength thus could be
% eliminated. 

pairs2 = findnearneighbor(centers,2*thresh*radius,0);
centers(pairs2(:,2),:)=[];
radii(pairs2(:,2),:)=[];
end
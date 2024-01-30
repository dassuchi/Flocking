function pairs2 = findnearneighbor(centers,thresh,plotstatus)
% centers is the collection of coordinates of tracked particle centers.
% thresh is the threshold to define whether it is a nearest neighbor.
pairs = delaunaynSegs(centers(:,1:2));
vectpairs = centers(pairs(:,2),:)-centers(pairs(:,1),:);
dist=sqrt(sum(vectpairs.^2,2));
keep = dist<thresh;
pairs2 = pairs(keep,:);

if plotstatus==1
  for i=1:size(pairs2,1);
      line(centers(pairs2(i,:),1),centers(pairs2(i,:),2),'LineWidth',3);
  end
end
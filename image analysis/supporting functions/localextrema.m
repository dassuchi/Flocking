function [minima,maxima]=localextrema(arr,compmat)

%[minima,maxima]=localextrema(arr,compmat)
%
%LOCALEXTREMA.m determines which elements of the matrix are extrema,
%uniformly greater or less than the surrounding neighbors to which they are
%being compared. Edges are explicitly accounted for, with elements always
%considered extrema relative to elements beyond the edge.
%
%INCLUDE:   ISODD.m
%
%INPUTS:    ARR:        The array to examine
%           COMPMAT:    An array, with each dimension containing an odd
%                       number of elements. The center element corresponds
%                       to zero displacement. Each non-zero element
%                       specifies an element relative to the center
%                       element. Extrema are elements of ARR which are
%                       minima or maxima for all such surrounding elements.
%
%OUTPUTS:   MINIMA:     The elements which are minimum, within the search 
%                       region specified.
%           MINIMA:     The elements which are maximum, within the search 
%                       region specified.
%
%Written by Stephen Anthony, U. Illinois Urbana-Champaign, 10/2009
%Last modified by Stephen Anthony on 10/04/2009


%Determine the size of the comparison matrix. It must be odd in order to
%have a well defined center. 
Csize=size(compmat);
if isodd(Csize)
    center=ceil(Csize/2);
else
    error('Could not determine the center. Input dimensions must have an odd number of elements')
end

%Determine the size of the array
Asize=size(arr);
[x,y]=meshgrid(1:Asize(2),1:Asize(1));

%Remove the trivial case
compmat(center(1),center(2))=0;

%Determine which displacements we need to check
[dx,dy]=find(compmat);
disp=[dx(:) dy(:)]-ones(size(dx(:)))*center;

%Initialize the outputs
minima=zeros(size(arr));
maxima=zeros(size(arr));

N=size(disp,1);
for j=1:N
    %Create the appropriately displaced matrix
    offset=circshift(arr,disp(j,:));
    
    %Determine which elements satisfy the condition
    tmpmax=arr>=offset;
    tmpmin=arr<=offset;
    
    %We need to find the edges
    edge(:,:,1)=x>Asize(2)+disp(j,2);
    edge(:,:,2)=x<=disp(j,2);
    edge(:,:,3)=y>Asize(1)+disp(j,1);
    edge(:,:,4)=y<=disp(j,1);
    edge=logical(sum(edge,3));

    %If the element is an edge, we cannot apply any test for this
    %displacement. Set the results as true.
    tmpmax=tmpmax | edge;
    tmpmin=tmpmin | edge;
    
    %Record
    minima=minima+tmpmin;
    maxima=maxima+tmpmax;
end

%The results are true local mimima and maxima if they were minima or maxima
%for all displacements checked. 
minima=minima==N;
maxima=maxima==N;
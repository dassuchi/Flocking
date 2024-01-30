function traj = linktrajsim(traj,distance,boundary_cutoff,xdimension,ydimension)

% This function link the tracked trajectory with a simplified way.
% each particle is given an ID
lost = size(traj(1).centers,1)+1; % If a particle does not find its
% counterpart in the previous frame, it is given a new ID.  

% extra step: particles at the boundary give many artificial results, such
% as number of neighbors. So remove first. 
% xdimension = size(traj(1).arr,1);
% ydimension = size(traj(1).arr,2);
for frame = 1:size(traj,2)
    keep =(traj(frame).centers(:,1)>boundary_cutoff & traj(frame).centers(:,2)>boundary_cutoff & traj(frame).centers(:,1)<ydimension-boundary_cutoff & traj(frame).centers(:,2)<xdimension-boundary_cutoff);
    traj(frame).centers = traj(frame).centers(keep,:);
%   traj(frame).radii = traj(frame).radii(keep,:);
end

for n = 1:size(traj(1).centers,1)
    traj(1).id(n) = n; % for the first frame, sequeantially give ID
end
traj(1).id = traj(1).id';

for frame = 1:size(traj,2)-1;
    length = size(traj(frame).centers,1);
    length2 = size(traj(frame+1).centers,1);
    traj(frame+1).id = zeros(length2,1);
    % For each particle in current frame, try to find its counter part in
    % the next frame (within a radius<distant)
    % Note this has a assumption that fails sometime: next frame another
    % particle can come into a distance closer than the real particle. This
    % can happen easily if an overtracking occur between two touching
    % particles. 
     for n = 1:length
        h = ones(length2,1);
        [X,Y] = meshgrid(traj(frame).centers(n,1:2),h);
        check = traj(frame+1).centers(:,1:2)-X;
        check(:,3) = sqrt(check(:,1).^2+check(:,2).^2);
        ind = find(check(:,3)==min(check(:,3)));
        if min(check(:,3))<distance
                traj(frame+1).id(ind) = traj(frame).id(n);
                traj(frame).vel(n,:) = traj(frame+1).centers(ind,1:2)-traj(frame).centers(n,1:2);
        end
    end
    % Taking care of the missing particles. 
    for n = 1:size(traj(frame+1).centers,1)
        if  traj(frame+1).id(n)==0
            traj(frame+1).id(n) = lost;
            lost = lost+1;
        end
    end
    
    if mod(frame,50)==0
        frame
    end
    
end
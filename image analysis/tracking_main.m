%% for 5x data
obj = VideoReader(filepath);
startframe = 1;
endframe = 100; % For example, from frame 1 to 100 is the duration of interest.
interval = 10; % If we don't want to track every single frame.
centers = [];
for t = startframe:interval:endframe
arr = read(obj,t);
arr = im2double(arr(:,:,1)); % 
arr = imcomplement(imresize(arr,2,'method','bilinear'));
optional.peakmin = 0.3; % see function features for explanation of this parameter.
centers((t-startframe)/interval*2+1).ftr=features(arr,2,optional);
centers((t-startframe)/interval*2+1).frame=t;
arr = read(obj,t+1);
arr = im2double(arr(:,:,1)); % 
arr = imcomplement(imresize(arr,2,'method','bilinear'));
optional.peakmin = 0.3;
centers((t-startframe)/interval*2+2).ftr=features(arr,2,optional);
centers((t-startframe)/interval*2+2).frame=t+1;
t
end
% estimate velocity here by finding neareast pairs between two frames
for t = 1:2:size(centers,1)
ftr1 = centers(t).ftr;
ftr2 = centers(t+1).ftr;
length1 = size(ftr1,1);
    minidist = [];
for n = 1:length1    
    dist = sqrt((ftr2(:,1)-ftr1(n,1)).^2+(ftr2(:,2)-ftr1(n,2)).^2);
    minidist(n,1) = min(dist);
end
t
centers(t).minidist = minidist; % this can be converted to speed taking into
                                % consideration pixel size and time interval.
end
%% for 40x data
obj = VideoReader(filepath);
startframe = 1;
endframe = 100; % For example, from frame 1 to 100 is the duration of interest.
for t = startframe:endframe
arr = read(obj,t); % 
arr = im2double(arr(:,:,1)); % 
arr = imresize(arr,2,'method','bilinear');% 
[centers,radii] = imfindcircles(imadjust(arr),[16,20],'ObjectPolarity','dark','sensitivity',0.98);
[centers1,radii1] = noovertrack(centers,radii,20,0.8); 
[centers2,radii2] = crystaltrackmethod3(centers1,arr,17,radii1,0.5,0);
m = t-startframe+1;
traj(m).centers = centers2;
traj(m).radii = radii2;
traj(m).frame = t;
if mod(t,50)==0
    t
end
end
clear arr t centers radii centers1 radii1
trajlink = linktrajsim(traj,30,20,1920,2560);
%% for 64x data 1
obj = VideoReader('D:\chrome downloads\20140508\7th\senventh time 30khz 10v 2.avi');
startframe = 1;
endframe = 100; % For example, from frame 1 to 100 is the duration of interest.
for t = startframe:endframe
arr = read(obj,t); % 
arr = im2double(arr(:,:,1)); % 
arr = imresize(arr,2,'method','bilinear');
[centers,radii] = imfindcircles(arr,[25,32],'ObjectPolarity','dark','sensitivity',0.95);
if size(centers,1)
[centers1,radii1] = noovertrack(centers,radii,28,0.8); 
[centers1,radii1] = crystaltrackmethod3(centers1,arr,28,radii,0.5,0);
else
    centers1 = centers;
    radii1 = radii; % just in case there are empty images.
end
m = t-startframe+1;
traj(m).centers = centers1;
traj(m).radii = radii1;
traj(m).frame = t;
if mod(t,100)==0
    t
end
end
traj = linktrajsim(traj,48,32,1920,2560);
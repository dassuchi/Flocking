data = load('/data/biophys/suchismita/dipole/repulsive/Experiment/Experiments/new_set_nov2023/40x_&_64x trajectories/m12/trajlink0508311.mat');

% Open the .dat files for writing
fid = fopen('/data/biophys/suchismita/dipole/repulsive/Experiment/Experiments/new_set_nov2023/40x_&_64x trajectories/m12/output_config_PD.dat', 'w');
fid1 = fopen('/data/biophys/suchismita/dipole/repulsive/Experiment/Experiments/new_set_nov2023/40x_&_64x trajectories/m12/radii_oc_PD.dat', 'w');
fid2 = fopen('/data/biophys/suchismita/dipole/repulsive/Experiment/Experiments/new_set_nov2023/40x_&_64x trajectories/m12/id_oc_PD.dat', 'w');
fid3 = fopen('/data/biophys/suchismita/dipole/repulsive/Experiment/Experiments/new_set_nov2023/40x_&_64x trajectories/m12/vel_oc_PD.dat', 'w');
fid4 = fopen('/data/biophys/suchismita/dipole/repulsive/Experiment/Experiments/new_set_nov2023/40x_&_64x trajectories/m12/time_particle_PD.dat', 'w');

allTimeSnaps = data.traj0508311;

% Loop over each time snap
for i = 1:length(allTimeSnaps)
    timeSnap = allTimeSnaps(i);
    centers = timeSnap.centers;
    radii = timeSnap.radii;
    id = timeSnap.id;
    vel = timeSnap.vel;
    
    N1 = size(centers, 1);
    N2 = size(vel, 1);
    fprintf(fid, '%d\n %d\n', i, N1);
    fprintf(fid1, '%d\n %d\n', i, N1);
    fprintf(fid2, '%d\n %d\n', i, N1);
    fprintf(fid3, '%d\n %d\n', i, N2);
    fprintf(fid4, '%d\t %d\n', i, N1);
    
    
    for j = 1:N1
        fprintf(fid, '%f\t%f\t%f\t%f\n', centers(j, 1), centers(j, 2), centers(j, 3), centers(j, 4));
        fprintf(fid1, '%f\n', radii(j,1));
        fprintf(fid2, '%d\n', id(j,1));
    end
    
    for j = 1:N2
        fprintf(fid3, '%f\t%f\n', vel(j,1),vel(j,2));
    end    

    fprintf('Processing time snap %d of %d\n', i, length(allTimeSnaps));
end

% Close the .dat files
fclose(fid);
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);


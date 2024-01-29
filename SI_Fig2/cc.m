
% Set the total number of time steps
tmax = 3358;

% Initialize arrays to store the data
theta = zeros(tmax, 1);
theta_vp = zeros(tmax, 1);

% Read data from files
fid1 = fopen('polarity_op_or.dat', 'r');
fid2 = fopen('polarity_op_vp.dat', 'r');

for t = 1:tmax
    % Read time and value from each file
    data1 = fscanf(fid1, '%d %f\n', [1 2]);
    data2 = fscanf(fid2, '%d %f\n', [1 2]);
    
    % Store the values
    theta(t) = data1(2);
    theta_vp(t) = data2(2);
end

% Close the files
fclose(fid1);
fclose(fid2);

% Calculate the cross-correlation
maxLag = tmax - 1; % Adjust maxLag if needed
cross_corr = xcorr(theta_vp, theta, maxLag, 'unbiased');

% Extract the portion of the cross-correlation for positive lags
positiveLags = maxLag+1 : 2*maxLag+1;
positiveCrossCorr = cross_corr(positiveLags);

% Open a new file for writing
fileID = fopen('cross_correlation_data.dat', 'w');

% Check if the file was opened successfully
if fileID == -1
    error('File could not be opened.');
end

% Write a header to the file
fprintf(fileID, 'Lag\tCross-Correlation\n');

% Write the data to the file
for i = 1:length(positiveCrossCorr)
    fprintf(fileID, '%d\t%f\n', i-1, positiveCrossCorr(i));
end

% Close the file
fclose(fileID);



plot(0:maxLag, positiveCrossCorr);
ytickformat('%.2f');
axis([0 maxLag 0.0 0.4]);
xlabel('Lag');
ylabel('Cross-correlation');
% Increase the size of x-axis tick labels
ax = gca; % Get handle to current axes.
ax.XAxis.FontSize = 12; % Set x-axis font size. Adjust the value as needed.
% Increase the size of x-axis tick labels
ax.YAxis.FontSize = 12; % Set x-axis font size. Adjust the value as needed.

%title('Cross-Correlation between p_i and v_p}');
grid off;

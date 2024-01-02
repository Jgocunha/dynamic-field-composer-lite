% Load the data
data = load('normalizedTargetFieldActivation.txt');

% Create a spatial vector (assuming a spatial step of 1 )
space = 0:1:length(data)-1;

% Plot the data
plot(space, data);

% Add labels and title
xlabel('Space');
ylabel('Value');
title('Example Plot');

% Show the grid
grid on;

% Show the plot
figure;


%% matrix
% Open the file
fid = fopen('input - target_weights.txt', 'r');

% Read the size of the matrix
matrix_size = fscanf(fid, '%d %d', [1, 2]);

% Skip the first line
fgetl(fid);

% Read the data
data = fscanf(fid, '%f', [matrix_size(2), matrix_size(1)]);

% Close the file
fclose(fid);

% Reshape the data to a 2D matrix
data = data';

% Create 3D plot
figure;
[X, Y] = meshgrid(1:matrix_size(1), 1:matrix_size(2));
surf(X, Y, data);

title('Your 3D Matrix');
xlabel('X-axis');
ylabel('Y-axis');
zlabel('Z-axis');
colormap('jet'); % Change the colormap (e.g., 'jet', 'gray', 'hot', etc.)
colorbar;
grid on;



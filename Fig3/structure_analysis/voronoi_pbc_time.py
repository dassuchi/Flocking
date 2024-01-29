import numpy as np
from scipy.spatial import Voronoi, voronoi_plot_2d
import matplotlib.pyplot as plt
import math

# Define a function to calculate the Voronoi tessellation
def voronoi_tessellation(positions, box_size, time, f):
    # Repeat the positions in each direction to account for periodicity
    positions = np.concatenate((positions, positions + [box_size, 0], positions + [0, box_size], positions + [-box_size, 0], positions + [0, -box_size], positions + [box_size, box_size], positions + [-box_size, box_size], positions + [-box_size, -box_size], positions + [box_size, -box_size]))
    
    # Perform the Voronoi tessellation
    vor = Voronoi(positions)

    # Extract the Voronoi vertices and regions
    voronoi_vertices = vor.vertices
    voronoi_regions = vor.regions
    
    # Create a dictionary to store the neighbours for each point
    neighbors = {i: set() for i in range(len(positions))}

    # Iterate through the ridge points to find the neighbors
    for r in vor.ridge_points:
      if -1 not in r: # ignore ridges with missing points
        neighbors[r[0]].add(r[1])
        neighbors[r[1]].add(r[0])

    # Original number of coordinates
    N = int(len(positions)/9)
    #print("N =",N)
    
    # Write the current time step to the file
    f.write(str(time)+"\n")
    # Iterate through the nodes
    for i in range(N):
        # Get the number of neighbors for the current node
        num_neighbors = len(neighbors[i])
        # Iterate through the neighbors of the current node
        f.write(str(i) +"\t" +str(num_neighbors) + "\n")
        for n in neighbors[i]:
            # Write the node index, neighbor value and the number of neighbors to the file
            k = n%N #Gives the index of the original neighbour (accounted from pbc)
            f.write(str(k)+"\n")
    return voronoi_vertices, voronoi_regions
    
# Read the positions from file
data = np.genfromtxt("config_time.dat")
time_series = []

N = 2500
phi = 0.1
box_size = math.sqrt((N*math.pi)/(4.0*phi))
print("box length =",box_size)

# Store the data in a list or a numpy array
for i in range(0, data.shape[0], N):
    time_series.append(data[i:i+N])

# Open the file in append mode before the loop
with open("voronoi_neighbours_time.txt", "w") as f:
    for time in range(len(time_series)):
        print(time)
        positions = time_series[time]
        voronoi_vertices, voronoi_regions = voronoi_tessellation(positions, box_size, time, f)


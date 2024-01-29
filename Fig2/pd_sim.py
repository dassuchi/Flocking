import numpy as np
import matplotlib.pyplot as plt

# Load the data

extrapolated_values = np.loadtxt('extrapolated_values.txt')
steepest_new = np.loadtxt('steepest_new.dat', delimiter=',')

pol_pd_data = np.loadtxt('pol_pd_full_new.dat')
# Set up the plot
fig, ax = plt.subplots(figsize=(10, 7))

plt.rcParams['font.sans-serif'] = ['Helvetica', 'Arial', 'sans-serif']

# Plot the pol_pd_data
#sc = ax.scatter(pol_pd_data[:,1] * 5.0 / 11.11, pol_pd_data[:,0], c=pol_pd_data[:,2], s=250, cmap='binary', edgecolors='none', vmin=0, vmax=1.0)
# Plot the pol_pd_data with edges
sc = ax.scatter(pol_pd_data[:,1] * 5.0 / 11.11, pol_pd_data[:,0], 
                c=pol_pd_data[:,2], s=250, cmap='binary', 
                edgecolors='black',  # Adding a black edge to each point
                linewidths=0.5,      # You can adjust the linewidth of the edge
                vmin=0, vmax=1.0)

# Plot the interpolated_values with lines and points
ax.plot(extrapolated_values[:,1], extrapolated_values[:,0], linewidth=4, linestyle='-', color='limegreen', marker='s', fillstyle='none', markersize=8)

# Plot the steepest_new data
ax.plot(steepest_new[:,1]* 5.0 / 11.11, steepest_new[:,0] , linewidth=4, linestyle='-', color='mediumblue', marker='s', fillstyle='none', markersize=8)


# Set x and y range
ax.set_ylim(0, 0.27)
ax.set_xlim(-2.5, 62.5)

# Set x and y ticks format
ax.yaxis.set_major_formatter(plt.FormatStrFormatter('%.2f'))

# Set font size for ticks
ax.tick_params(axis='both', which='major', labelsize=25)

# Set line width of the border
for spine in ax.spines.values():
    spine.set_linewidth(2.5)

# Add a color bar
cbar = plt.colorbar(sc, ax=ax)
cbar.ax.tick_params(labelsize=25)

# Show the plot
plt.tight_layout()
plt.savefig('pd_sim.pdf', format='pdf', dpi=300)
plt.show()


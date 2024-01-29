import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
from matplotlib.cm import ScalarMappable

# Load data
np_data = np.loadtxt('data_full.dat')
#spinodal_data = np.loadtxt('spinodal_r.dat')

# Prepare data for scatter plot
x = np_data[:, 0]
y = np_data[:, 1]
z = np_data[:, 2]  # This will determine the color of each point

# Plotting
plt.figure(figsize=(10, 8))
scatter = plt.scatter(x, y, c=z, cmap='jet', s=180)  # Original scatter plot

# Plot spinodal curve
#plt.plot(spinodal_data[:, 0], spinodal_data[:, 1], 'k-', lw=2, label='Spinodal')
#plt.plot(spinodal_data[:, 0], spinodal_data[:, 2], 'k-', lw=2)

# Setting the range of x and y axes
plt.xlim(-0.025, 0.425)
plt.ylim(0, 32)

# Enhancing border width of plot
ax = plt.gca()
for axis in ['top','bottom','left','right']:
    ax.spines[axis].set_linewidth(2)  # Increase border width

ax.tick_params(axis='both', which='major', labelsize=20)  # Increase tick label size

# Custom colorbar that reflects the range 0 to 1
norm = colors.Normalize(vmin=0, vmax=1)
mappable = ScalarMappable(norm=norm, cmap='jet')
mappable.set_array([])  # Setting an empty array for the mappable
cbar = plt.colorbar(mappable, ticks=np.arange(0, 1.1, 0.2))
cbar.ax.tick_params(labelsize=20)  # Increase colorbar tick label size
cbar.outline.set_linewidth(2)  # Increase colorbar border width

# Labels and customizations
#plt.legend(loc='lower left', fontsize = 21)

# Save the figure
plt.savefig("phase_space.pdf", dpi = 300)

# Show the plot
plt.show()


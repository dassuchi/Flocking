import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter
import matplotlib as mpl

# Function to format y-ticks
def format_ytick(tick_val, tick_pos):
    return "{:.1f}".format(tick_val)

# Load data
data1 = np.loadtxt("liq_flock.dat", usecols=(0, 1, 2))
data2 = np.loadtxt("liq_noflock.dat", usecols=(0, 1, 2))
data3 = np.loadtxt("hex_flock.dat", usecols=(0, 1, 2))
data4 = np.loadtxt("crystal_noflock.dat", usecols=(0, 1, 2))
data5 = np.loadtxt("crystal_flock.dat", usecols=(0, 1, 2))

phase_boundaries = np.loadtxt("steepest_gamma6_new1.dat", delimiter=',', usecols=(0, 1))
data_extrapolation = np.loadtxt("steepest_gamma6_new1.dat", delimiter=',', usecols=(0, 1, 2))

# Scale x-axis values and x-data by 5.0/11.11
scaling_factor = 5.0/11.11
data1[:, 1] *= scaling_factor
data2[:, 1] *= scaling_factor
data3[:, 1] *= scaling_factor
data4[:, 1] *= scaling_factor
data5[:, 1] *= scaling_factor

#phase_boundaries[:, 1] *= scaling_factor
#data_extrapolation[:, 1] *= scaling_factor

# Create a figure and axis
fig, ax = plt.subplots()

plt.rcParams['font.sans-serif'] = ['Helvetica', 'Arial', 'sans-serif']

# Use the gist_earth colormap
gist_earth = plt.cm.gist_earth
# Define a new colormap that uses the colors of the original gist_earth colormap, 
# but scaled such that the color at 0.9 in the original is now at 1.0.
colors = gist_earth(np.linspace(0, 0.9, gist_earth.N))
new_gist_earth = mpl.colors.LinearSegmentedColormap.from_list("new_gist_earth", colors)

# Setup colormap
cmap = new_gist_earth


# Plot data
sc1 = ax.scatter(data1[:, 1], data1[:, 0], c=data1[:, 2], s=105, cmap=cmap, marker='o', vmin=0, vmax=1, linewidths=1.5)
sc2 = ax.scatter(data2[:, 1], data2[:, 0], c=data2[:, 2], s=105, cmap=cmap, marker='o', vmin=0, vmax=1, linewidths=1.5)
sc3 = ax.scatter(data3[:, 1], data3[:, 0], c=data3[:, 2], s=105, cmap=cmap, marker='h', vmin=0, vmax=1, linewidths=1.5)
sc4 = ax.scatter(data4[:, 1], data4[:, 0], c=data4[:, 2], s=105, cmap=cmap, marker='s', vmin=0, vmax=1, linewidths=1.5)
sc5 = ax.scatter(data5[:, 1], data5[:, 0], c=data5[:, 2], s=105, cmap=cmap, marker='s', vmin=0, vmax=1, linewidths=1.5)

# Plot phase boundaries
#ax.plot(phase_boundaries[:, 1], phase_boundaries[:, 0], 'k-', label='Phase Boundary', linewidth=2)

# Polynomial extrapolation
x_extrapolation = np.linspace(0.1, 0.7, 100)
coefficients = np.polyfit(data_extrapolation[:,0], data_extrapolation[:,1], 3)
polynomial = np.poly1d(coefficients)
y_poly_extrapolation = polynomial(x_extrapolation)
#ax.plot(x_extrapolation, y_poly_extrapolation, '--', linewidth=2, color='red')

# Linear extrapolation
x1, y1 = data_extrapolation[-2, 0], data_extrapolation[-2, 1]
x2, y2 = data_extrapolation[-1, 0], data_extrapolation[-1, 1]
slope = (y2 - y1) / (x2 - x1)
y_linear_extrapolation = y2 + slope * (x_extrapolation - x2)

# Create mask for phi > 0.55
mask = x_extrapolation > 0.55
x_extrapolation_linear = x_extrapolation[mask]
y_linear_extrapolation_masked = y_linear_extrapolation[mask]

# Plot linear extrapolation for phi > 0.55
#ax.plot(y_linear_extrapolation_masked, x_extrapolation_linear, '--', linewidth=2, color='grey')

# Additional plot settings
yticks = np.arange(0, 1.42, 0.2)
xticks = np.arange(0, 65, 10)
ax.set_yticks(yticks)
ax.set_xticks(xticks)
ax.set_xlim(-2.5, 62.5)
ax.tick_params(axis='x', labelsize=20)
ax.tick_params(axis='y', labelsize=20)
#ax.yaxis.set_major_formatter(FuncFormatter(format_ytick))
for spine in ax.spines.values():
    spine.set_linewidth(3)  # Increase linewidth of the plot border

# Add colorbar
cbar = fig.colorbar(sc1, ax=ax, format="%.1f")
cbar.ax.tick_params(labelsize=20)
cbar_ticks = np.arange(0.0, 1.15, 0.2)
cbar.set_ticks(cbar_ticks)
cbar.outline.set_linewidth(3)  # Increase linewidth of the colorbar border

# Show and save figure
plt.tight_layout()
plt.savefig("structure1.pdf", format='pdf', dpi=300)
plt.show()


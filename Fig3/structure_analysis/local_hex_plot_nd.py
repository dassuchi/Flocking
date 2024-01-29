import matplotlib.pyplot as plt
import numpy as np
import math
import matplotlib as mpl


data = np.genfromtxt('local_hexatic_order_angle_complex_argument_time.txt')
N = 2500
phi = 0.1
box = math.sqrt((N*math.pi)/(4.0*phi))
t = data[:,0]
x = data[:,1]
y = data[:,2]
local_hexatic_order = data[:,3]
angle = data[:,4]

# Create a scatter plot
#sc = plt.scatter(x, y, s=2.5, c=local_hexatic_order, cmap='hsv')
sc = plt.scatter(x, y, s=2.5, c=angle, cmap='hsv')

# Adjust the plot settings
cb = plt.colorbar(sc)

# Adjust border width for main plot
ax = plt.gca()  # Get the current axis
for spine in ax.spines.values():
    spine.set_linewidth(2.5)  # Adjust the value as needed
    
 	

# Adjust border width for colorbar
cb.outline.set_linewidth(2.5)  # Adjust the value as needed
plt.clim(vmin=-3.1416, vmax=3.1416)

cb.outline.set_linewidth(2.5)

# Modify ticks and labels for colorbar
cb.set_ticks([-np.pi, -np.pi/2.0, 0, np.pi/2.0, np.pi])
cb.set_ticklabels([r'$-\pi$', r'$-\frac{\pi}{2}$', '0', r'$\frac{\pi}{2}$', r'$\pi$'])

# Increase tick width of the colorbar
cb.ax.tick_params(width=2.5)    

plt.xlim(0, box)
plt.ylim(0, box)
#plt.clim(-1.0, 1.0)
plt.xticks(fontsize=15)
plt.yticks(fontsize=15)
cb.ax.tick_params(labelsize=15)

# Save the plot as a high-quality PDF with a DPI of 600
plt.savefig('local_hex_phi0.1_v122_angle_polar_argument.pdf', dpi=600, format='pdf', bbox_inches='tight')

plt.show()


import numpy as np
import matplotlib.pyplot as plt

# Load the data
expt_data_flock_40x = np.loadtxt('pol_expt_40x_jan_flock.dat')
expt_data_iso_40x = np.loadtxt('pol_expt_40x_iso.dat')

expt_data_iso_64x = np.loadtxt('pol_expt_64x_iso.dat')

expt_data_flock_5x = np.loadtxt('pol_expt_5x_jan_flock.dat')
expt_data_iso_5x = np.loadtxt('pol_expt_5x_iso.dat')

extrapolated_values = np.loadtxt('extrapolated_values.txt')
steepest_new = np.loadtxt('steepest_new.dat', delimiter=',')
scattering_boundary = np.loadtxt('boundary_scattering_new.dat')

#pol_pd_data = np.loadtxt('pol_pd_full_new.dat')
# Set up the plot
fig, ax = plt.subplots(figsize=(10, 7))

plt.rcParams['font.sans-serif'] = ['Helvetica', 'Arial', 'sans-serif']

# Plot the pol_pd_data
#sc = ax.scatter(pol_pd_data[:,1] * 5.0 / 11.11, pol_pd_data[:,0], c=pol_pd_data[:,2], s=250, cmap='jet', edgecolors='none', vmin=0, vmax=1.0)

# Plot the interpolated_values with lines and points
ax.plot(extrapolated_values[:,1], extrapolated_values[:,0], linewidth=4, linestyle='-', color='limegreen', marker='s', fillstyle='none', markersize=8)

# Plot the steepest_new data
ax.plot(steepest_new[:,1]* 5.0 / 11.11, steepest_new[:,0] , linewidth=4, linestyle='-', color='mediumblue', marker='s', fillstyle='none', markersize=8)

# Plot the scattering boundary with lines and points
ax.plot(scattering_boundary[:,0], scattering_boundary[:,1], linewidth=4, linestyle='-', color='red', marker='s', fillstyle='none', markersize=8)

# Plot the expt_data_list with error bars
ax.errorbar(expt_data_flock_5x[:,2], expt_data_flock_5x[:,0], xerr=expt_data_flock_5x[:,3], yerr=expt_data_flock_5x[:,1], fmt='o', markersize=12, color='black', capsize=5)
ax.errorbar(expt_data_iso_5x[:,2], expt_data_iso_5x[:,0], xerr=expt_data_iso_5x[:,3], yerr=expt_data_iso_5x[:,1], fmt='o', markersize=12, color='black', capsize=5, fillstyle='none')

ax.errorbar(expt_data_flock_40x[:,2], expt_data_flock_40x[:,0], xerr=expt_data_flock_40x[:,3], yerr=expt_data_flock_40x[:,1], fmt='o', markersize=12, color='black', capsize=5)
ax.errorbar(expt_data_iso_40x[:,2], expt_data_iso_40x[:,0], xerr=expt_data_iso_40x[:,3], yerr=expt_data_iso_40x[:,1], fmt='o', markersize=12, color='black', capsize=5, fillstyle='none')

ax.errorbar(expt_data_iso_64x[:,2], expt_data_iso_64x[:,0], xerr=expt_data_iso_64x[:,3], yerr=expt_data_iso_64x[:,1], fmt='o', markersize=12, color='black', capsize=5, fillstyle='none')

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
#cbar = plt.colorbar(sc, ax=ax)
#cbar.ax.tick_params(labelsize=25)

# Show the plot
plt.tight_layout()
plt.savefig('pd_expt.pdf', format='pdf', dpi=300)
plt.show()


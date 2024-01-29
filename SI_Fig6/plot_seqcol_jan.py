import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap

plt.rcParams["text.usetex"] = True

# Specific parameter values
parameters = [0.02, 0.05, 0.10, 0.15, 0.25, 0.40, 1.0]

# Create file name list based on the parameters
file_names = ["pol_op_phi{:.2f}.dat".format(i) for i in parameters]

datasets = []

# Load data from files
for file_name in file_names:
    data = np.loadtxt(file_name)
    if data.shape == (13, 3):  # To ensure the data has the right shape
        datasets.append({
            "x": data[:, 0] * (5.0 / 11.11),  # Scale x-axis values
            "y": data[:, 1],
            "dy": data[:, 2]
        })

# Create a figure and axis
fig, ax = plt.subplots(figsize=(6, 6))

# Define a custom colormap (GnBu) with the range from 0.2 to 1.0
colors = plt.cm.GnBu(np.linspace(0.2, 1.0, len(datasets)))

# Define a single line style (dotted)
linestyle = '--'

# Plot each dataset with error bars using custom colors, dotted lines, markers, and caps
for idx, data in enumerate(datasets):
    label_text = r"{:.2f}".format(parameters[idx])
    ax.errorbar(data["x"], data["y"], yerr=data["dy"], fmt='o', capsize=5,
                color=colors[idx], label=label_text, linestyle=linestyle, linewidth=2.5)

# Ensure all borders (spines) are visible and increase their width
for spine in ax.spines.values():
    spine.set_visible(True)
    spine.set_linewidth(2)

# Adjust legend position and increase its border width
legend = ax.legend(loc='lower right', fontsize=18, bbox_to_anchor=(0.95, 0.16))
legend.get_frame().set_edgecolor('black')
legend.get_frame().set_linewidth(2)

# Increase size of x and y tick labels
ax.tick_params(axis='both', labelsize=18)

# Remove grid
ax.grid(False)

# Save the figure in high resolution
plt.tight_layout()
plt.savefig("Fig2_modified.pdf", dpi=600)
plt.show()

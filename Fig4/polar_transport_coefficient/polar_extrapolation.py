import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.interpolate import interp1d

# Disabling LaTeX typesetting as it may not be supported in all environments
plt.rcParams["text.usetex"] = False
plt.rcParams['font.family'] = 'sans-serif'
plt.rcParams['font.sans-serif'] = 'Helvetica'
plt.rcParams['axes.linewidth'] = 2.5

# Specific parameter values
parameters = [0.02, 0.05, 0.10, 0.15, 0.20, 0.25]
scaling_factor = 5.0 / 11.11
conversion_second = 0.15

# Create file name list based on the parameters
file_list = ["{:.2f}.dat".format(i) for i in parameters]

# Create a figure and axis
fig, ax = plt.subplots(figsize=(8, 6))

# Define a custom colormap (GnBu) with the range from 0.3 to 1.0
colors = plt.cm.GnBu(np.linspace(0.3, 1.0, len(parameters)))

# Define a single line style (dotted)
linestyle = '--'

# Function to fit a linear model for extrapolation
def linear_model(x, a, b):
    return a * x + b

extrapolated_values = {}

# Define the upper limit for x-axis extrapolation
x_upper_limit = 100  # Adjust this value as needed

for i, file in enumerate(file_list):
    data = np.loadtxt(file, skiprows=0)
    data[:, 1] *= scaling_factor
    data[:, 3] *= conversion_second
    x = data[:, 1]
    y = data[:, 3]

    # Fit a linear model to the data for extrapolation
    popt, _ = curve_fit(linear_model, x, y)
    
    # Extrapolate to find the zero-crossing
    x_cross = -popt[1] / popt[0]
    extrapolated_values[parameters[i]] = x_cross

    # Plot the extrapolated line
    x_extended = np.linspace(min(x), x_upper_limit, 100)
    y_extended = linear_model(x_extended, *popt)
    ax.plot(x_extended, y_extended, linestyle=linestyle, color=colors[i])
    ax.plot(x_cross, 0, 'x', color='k', markersize=18, markerfacecolor="None", markeredgewidth=1.5)

    # Plot the original data
    ax.plot(x, y, 'o', color=colors[i], label="{:.2f}".format(parameters[i]), markersize=14)
  
    
# Setting y-axis limit
ax.set_ylim(bottom=-0.15, top=0.15)
ax.set_xlim(left=-2.5, right=45)

# Adding horizontal line at y=0 and setting x-ticks
ax.axhline(0, color='black', linestyle='--', linewidth=3.5)
ax.set_xticks(np.arange(0, 45, 10))
ax.set_yticks(np.arange(-0.15, 0.16, 0.05))
ax.tick_params(axis='both', which='major', labelsize=20)


# Adding legend
legend = ax.legend(fontsize=18, loc='upper right', bbox_to_anchor=(0.93, 0.98), edgecolor='black', handlelength=1, handletextpad=0.5)
legend.get_frame().set_linewidth(2.5)


# Saving the interpolated values to a file
with open("extrapolated_values.txt", "w") as file:
    for param, x_cross in extrapolated_values.items():
        file.write("{:.2f}  {:.4f}\n".format(param, x_cross))

plt.tight_layout()
plt.savefig("plot_with_extrapolation_y_limit_new.pdf", dpi=600)
plt.show()


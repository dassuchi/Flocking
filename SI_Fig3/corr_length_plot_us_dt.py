import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

def exponential_func(r, C_0, xi):
    return C_0 * np.exp(-r/xi)

data_file = "orientation_corr_rav_data_scaled.dat"
phi = 0.05
N_max = 12800
sigma = 5.0
conversion = 0.6
box = np.sqrt((N_max * np.pi *sigma *sigma)/ (4.0 * phi))

data = np.loadtxt(data_file)
r_vals = data[:, 0]*0.6 #convert to \mu m
C_vals = data[:, 1]
times = data[:, 2]

unique_times = np.unique(times)
xi_values = []
xi_errors = []  # List to store the uncertainties
time_values = []

for time in unique_times:
    indices = np.where(times == time)
    r = r_vals[indices]
    C = C_vals[indices]

    # Dynamic range selection based on a threshold
   # 
    
   
    if C[1] > C[0]:
        print("check time ", time)
        
    if C[0]<0.8:
       max_index = len(r) // 8 # Integer division to get half the length
       # Slice the r and C arrays to use only up to max_index
       r = r[:max_index]
       C = C[:max_index]
        
    else:	
       max_index = len(r) // 8  # Integer division to get half the length
       # Slice the r and C arrays to use only up to max_index
       r = r[:max_index]
       C = C[:max_index]
     

    # Improved initial guesses
    initial_C_0 = C[0]
    initial_xi = 20  # Simple estimate, can be improved

    try:
        popt, pcov = curve_fit(exponential_func, r, C, p0=(initial_C_0, initial_xi), bounds=([0, 0], [initial_C_0, box]))
        xi = popt[1]
        xi_error = np.sqrt(pcov[1, 1])
    except RuntimeError:
        xi = np.nan
        xi_error = np.nan

    time_values.append(time)
    xi_values.append(xi)
    xi_errors.append(xi_error)

 

# Assuming 'r' is your data range
# Generate a list from min(r) to max(r) with steps of 25
    xticks = range(0, 120, 25)

# Your existing plotting code
    plt.plot(r, C, 'b-', marker="o", label='Data')
    plt.plot(r, exponential_func(r, *popt), 'r--', label='Fit: $\\xi={:.2f}\mu$m'.format(xi))
    plt.title('Exponential fit for time = {}'.format(time))
    plt.xlabel('r')
    plt.ylabel('C(r)')
    #plt.legend()

# Apply the generated x-ticks to the plot
    plt.xticks(xticks)
    
    # Set y-axis limits
    plt.ylim(0, 0.25)

# Double the border width
    for spine in plt.gca().spines.values():
    	spine.set_linewidth(2)

    plt.legend(fontsize=14, loc='upper right', bbox_to_anchor=[0.8, 0.8])
# Increase the tick sizes
    plt.tick_params(axis='both', which='major', labelsize=15)  # Increase major tick size
    plt.tick_params(axis='both', which='minor', labelsize=8)  # Increase minor tick size, if applicable

# Save and show the plot
    plt.savefig("corr_length_snap_{}2.pdf".format(time), dpi=300)
    plt.show()


       


# Save the results
output_file = "correlation_length_vs_time_rav_data_us_df_pixel2.dat"
with open(output_file, 'w') as f:
    for t, xi, xi_err in zip(time_values, xi_values, xi_errors):
       f.write("{}\t{}\t{}\t{}\t{}\n".format(t, xi, xi_err, xi*conversion, xi_err*conversion))


import numpy as np
import matplotlib.pyplot as plt

# Assuming z is loaded correctly
z = np.loadtxt('config_time_spp.dat')

X = z[:,0]
Y = z[:,1]
theta = z[:,2]  # Angle

u = np.cos(theta)  # x component of the arrow
v = np.sin(theta)  # y component of the arrow
theta = theta % (2 * np.pi)  # map to [0, 2*pi]
theta[theta > np.pi] -= 2 * np.pi  # shift values in (pi, 2*pi] to [-pi, 0]

tres = 0.1
time = 1000
tgap = 1
tinit = 0.0

# Specific time instants for plotting
time_instants = [5, 30, 150, 999]

for nt in time_instants:
    fig, ax1 = plt.subplots(figsize=(14, 14))
    print('nt = ',nt)
    # Increase border width
    for spine in ax1.spines.values():
        spine.set_linewidth(4)

    #ticks = np.arange(0, 1254, 100)
    #ax1.xaxis.set_ticks(ticks)
    #ax1.yaxis.set_ticks(ticks)
    ax1.axis([-0.5, 561, -0.5, 561])
    #ax1.tick_params(axis='both', which='major', labelsize=40, width=2, length=6)
    #ax1.tick_params(axis='both', which='minor', labelsize=20, width=1, length=2)

    plt.quiver(X[40000*nt:40000*nt+39999], Y[40000*nt:40000*nt+39999], u[40000*nt:40000*nt+39999], v[40000*nt:40000*nt+39999], theta[40000*nt:40000*nt+39999], headaxislength=5, headwidth=2, scale_units='inches', scale=50, cmap='hsv', clim=[-np.pi, np.pi])

    cb = plt.colorbar(shrink=0.8, aspect=30)
    ax1.set_aspect('equal')
    plt.hsv()
    plt.clim(vmin=-3.1416, vmax=3.1416)

    cb.outline.set_linewidth(4)

    # Modify ticks and labels for colorbar
    cb.set_ticks([-np.pi, -np.pi/2.0, 0, np.pi/2.0, np.pi])
    cb.set_ticklabels([r'$-\pi$', r'$-\frac{\pi}{2}$', '0', r'$\frac{\pi}{2}$', r'$\pi$'])

    # Increase tick width of the colorbar
    cb.ax.tick_params(width=4)

    for t in cb.ax.get_yticklabels():
        t.set_fontsize(55)

    plt.title('Simulation time = '+str(nt*tres*tgap+tinit), fontsize=30, y=1.05)
    plt.savefig('snap' + str(nt * 10) + ".png", dpi=300, bbox_inches='tight')
    plt.show()
    plt.close()




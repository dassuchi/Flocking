#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

int main() {
    int N = 2500;  // Total number of particles
    double phi_g = 0.1;  // Global area fraction
    double boxsize = pow(((N * PI) / (4.0 * phi_g)), 0.5);  // Size of simulation box
    double dr = 0.1;  // Width of radial bins
    double dphi = PI / 180;
    double dtheta = PI / 2.0;
    int n_r_bins = (int)(boxsize / (2.0 * dr));
    int n_phi_bins = (int)(2.0 * PI / dphi);
    int n_theta_bins = (int)(2.0 * PI / dtheta);
    double phi_range = 2 * PI; // Range of phi [-pi, pi]
    double phi_bin_width = phi_range / n_phi_bins; // Width of each phi bin

    // Allocate memory for g
    double ***g = (double ***)malloc(n_r_bins * sizeof(double **));
    if (g == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }
    for (int i = 0; i < n_r_bins; i++) {
        g[i] = (double **)malloc(n_phi_bins * sizeof(double *));
        if (g[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            return 1;
        }
        for (int j = 0; j < n_phi_bins; j++) {
            g[i][j] = (double *)malloc(n_theta_bins * sizeof(double));
            if (g[i][j] == NULL) {
                fprintf(stderr, "Out of memory\n");
                return 1;
            }
        }
    }

    double ***theta_read = (double ***)malloc(n_r_bins * sizeof(double **));
    if (theta_read == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    for (int i = 0; i < n_r_bins; i++) {
        theta_read[i] = (double **)malloc(n_phi_bins * sizeof(double *));
        if (theta_read[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            return 1;  // Added return for memory allocation failure
        }

        for (int j = 0; j < n_phi_bins; j++) {
            theta_read[i][j] = (double *)malloc(n_theta_bins * sizeof(double));
            if (theta_read[i][j] == NULL) {
                fprintf(stderr, "Out of memory\n");
                return 1;  // Added return for memory allocation failure
            }
        }
    }

    double ***phi_read = (double ***)malloc(n_r_bins * sizeof(double **));
    if (phi_read == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    for (int i = 0; i < n_r_bins; i++) {
        phi_read[i] = (double **)malloc(n_phi_bins * sizeof(double *));
        if (phi_read[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            return 1;  // Added return for memory allocation failure
        }

        for (int j = 0; j < n_phi_bins; j++) {
            phi_read[i][j] = (double *)malloc(n_theta_bins * sizeof(double));
            if (phi_read[i][j] == NULL) {
                fprintf(stderr, "Out of memory\n");
                return 1;  // Added return for memory allocation failure
            }
        }
    }

    double ***r_read = (double ***)malloc(n_r_bins * sizeof(double **));
    if (r_read == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    for (int i = 0; i < n_r_bins; i++) {
        r_read[i] = (double **)malloc(n_phi_bins * sizeof(double *));
        if (r_read[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            return 1;  // Added return for memory allocation failure
        }

        for (int j = 0; j < n_phi_bins; j++) {
            r_read[i][j] = (double *)malloc(n_theta_bins * sizeof(double));
            if (r_read[i][j] == NULL) {
                fprintf(stderr, "Out of memory\n");
                return 1;  // Added return for memory allocation failure
            }
        }
    }


    // Initialize g array
    for (int i = 0; i < n_r_bins; i++) {
        for (int j = 0; j < n_phi_bins; j++) {
            for (int k = 0; k < n_theta_bins; k++) {
                g[i][j][k] = 0;
            }
        }
    }

    FILE *file = fopen("../../g_clubbed.dat", "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file\n");
        return 1;
    }
    double rcos, rsin, g_tot;
    for (int theta = 0; theta < n_theta_bins; theta++) {
        for (int r = 0; r < n_r_bins; r++) {
            for (int phi = 0; phi < n_phi_bins; phi++) {
                fscanf(file, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", &theta_read[r][phi][theta], &r_read[r][phi][theta], &phi_read[r][phi][theta], &rcos, &rsin, &g_tot, &g[r][phi][theta]);
            }
        }
    }
    fclose(file);

    FILE *fp2 = fopen("g_diff1_mirror_check.dat", "w");
    FILE *fp3 = fopen("g_diff1_mirror.dat","w");

for (int theta = 0; theta < n_theta_bins; theta++) {
    for (int r = 0; r < n_r_bins; r++) {
        for (int phi = 0; phi < n_phi_bins; phi++) {
            
                double phi_value = phi_read[r][phi][theta];
                double mirror_phi_value = -phi_value;
                if (mirror_phi_value < -PI) mirror_phi_value += 2 * PI;
                if (mirror_phi_value > PI) mirror_phi_value -= 2 * PI;
                int mirror_phi_bin = (int)((mirror_phi_value + PI) / phi_bin_width);
                if (mirror_phi_bin >= n_phi_bins) mirror_phi_bin = 0;
                
                 int neg_theta = (theta + n_theta_bins / 2) % n_theta_bins;

                double diff1 = g[r][phi][theta] - g[r][mirror_phi_bin][neg_theta];
                fprintf(fp2,"%d\t%d\t%d\t%d\t%d\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", r, phi, theta, mirror_phi_bin, neg_theta, theta_read[r][phi][theta], phi_read[r][phi][theta], r_read[r][phi][theta], theta_read[r][mirror_phi_bin][neg_theta], phi_read[r][mirror_phi_bin][neg_theta], g[r][phi][theta], g[r][mirror_phi_bin][neg_theta], diff1);
                if (phi_read[r][phi][theta] > PI) phi_read[r][phi][theta] -= 2.0 * PI;
                if (theta_read[r][phi][theta] > PI) theta_read[r][phi][theta] -= 2.0 * PI;
                fprintf(fp3, "%lf\t%lf\t%lf\t%lf\n",  theta_read[r][phi][theta],r_read[r][phi][theta], phi_read[r][phi][theta], diff1);
            }
        }
    }

    fclose(fp2);
    fclose(fp3);

    // Free allocated memory for g, theta_read, phi_read, r_read
    // ...

    return 0;
}


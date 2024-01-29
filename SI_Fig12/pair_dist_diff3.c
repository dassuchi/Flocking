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
    double pi = 4.0*atan(1.0);
      
    double ***g = (double ***)malloc(n_r_bins * sizeof(double **));
    if (g == NULL) {
        fprintf(stderr, "Out of memory\n");
        return 1;
    }

    for (int i = 0; i < n_r_bins; i++) {
        g[i] = (double **)malloc(n_phi_bins * sizeof(double *));
        if (g[i] == NULL) {
            fprintf(stderr, "Out of memory\n");
            return 1;  // Added return for memory allocation failure
        }

        for (int j = 0; j < n_phi_bins; j++) {
            g[i][j] = (double *)malloc(n_theta_bins * sizeof(double));
            if (g[i][j] == NULL) {
                fprintf(stderr, "Out of memory\n");
                return 1;  // Added return for memory allocation failure
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

    FILE *file = fopen("../../../g_clubbed.dat", "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file\n");
        return 1;
    }
     FILE *fp2 = fopen("g_diff3_check.dat", "w");
     FILE *fp3 = fopen("g_diff3_r.dat","w");

    double rcos, rsin, g_tot;

    for (int theta = 0; theta < n_theta_bins; theta++) {
        for (int r = 0; r < n_r_bins; r++) {
            for (int phi = 0; phi < n_phi_bins; phi++) {
                fscanf(file, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", &theta_read[r][phi][theta], &r_read[r][phi][theta], &phi_read[r][phi][theta], &rcos, &rsin, &g_tot, &g[r][phi][theta]);
                //printf("%lf\n",g[r][phi][theta]);
                 }
            }
        }
    
   int diag_theta;
  
  // fprintf(fp2,"r\tphi\ttheta\tdiag_theta\tt_r\tphi_r\tr_r\ttheta_r_rev\tg1\tg2\tdiff3\n");
  for (int theta = 0; theta < n_theta_bins; theta++) {
    printf("theta = %d\t%d\n",theta,n_theta_bins);
    for (int r = 0; r < n_r_bins; r++) {
        for (int phi = 0; phi < n_phi_bins; phi++) {
       
               if (theta == 0) {diag_theta = 3;}
               if (theta == 1) {diag_theta = 2;}
               if (theta == 2) {diag_theta = 1;}
               if (theta == 3) {diag_theta = 0;}
               
         
                double diff3 = g[r][phi][theta] - g[r][phi][diag_theta];

              //  fprintf(fp2,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\%lf\n", r,phi,theta, neg_phi,diag_theta,(phi + n_phi_bins / 2) % n_phi_bins,(theta + n_theta_bins / 2) % n_theta_bins, diff1, diff2, diff3);
               fprintf(fp2,"%d\t%d\t%d\t%d\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", r,phi,theta, diag_theta,theta_read[r][phi][theta],phi_read[r][phi][theta],r_read[r][phi][theta],theta_read[r][phi][diag_theta],g[r][phi][theta],g[r][phi][diag_theta],diff3);
               if(phi_read[r][phi][theta]>pi){phi_read[r][phi][theta]-=2.0*pi;}
               if(theta_read[r][phi][theta]>pi){theta_read[r][phi][theta]-=2.0*pi;}
               fprintf(fp3,"%lf\t%lf\t%lf\t%lf\n",theta_read[r][phi][theta],r_read[r][phi][theta], phi_read[r][phi][theta],diff3);
            }
        }
         printf("theta = %d\t%d\n",theta,n_theta_bins);
        fprintf(fp2,"\n");
    }

    // Free allocated memory
    for (int i = 0; i < n_r_bins; i++) {
        for (int j = 0; j < n_phi_bins; j++) {
            free(g[i][j]);
        }
        free(g[i]);
    }
    free(g);

    return 0;
}


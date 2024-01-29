#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

// Define a particle structure to store position
typedef struct {
    double x;
    double y;
    double particle_theta;
} Particle;


int main()
{
    // Define simulation parameters
    int N = 2500;                   // Total number of particles
    double phi_g = 0.1;             // Global area fraction
    double boxsize = pow(((N * PI) / (4.0 * phi_g)), 0.5); // Size of simulation box
    double dr = 0.1;                // Width of radial bins
    double dphi = PI/40;       
    double dtheta = PI/40.0 ;      
    double times;
    int i, j, z, t, tmax;
    int file, line;
    FILE *fp0 = fopen("nline.dat", "r");
    fscanf(fp0, "%d\t%d\t%d\n", &file, &line, &tmax);
    char c;
    double xij, yij, rij;
    double rho = N / (boxsize * boxsize);
    double theta;
    int tsteady = 0;
    int tcount = 0;
    double phi_r, box_r, Fp;
    int i1, j1, k1;
    int counter = 0;

    // Allocate memory for particle positions
    Particle *particles = (Particle *)malloc(N * sizeof(Particle));

    // Allocate memory for pair distribution function
    int n_r_bins = (int)(boxsize / (2.0*dr));
    int n_phi_bins = (int)(2.0 * PI / dphi);
    int n_theta_bins = (int)(2.0* PI / dtheta); 
    printf("nr_bins = %d\t n_phi_bins = %d\t n_theta_bins = %d\n",n_r_bins,n_phi_bins,n_theta_bins);

    double ***g_counts = (double ***)malloc(n_r_bins * sizeof(double **));

    if (g_counts == NULL)
    {
        fprintf(stderr, "Out of memory");
        exit(0);
    }

    for (int i = 0; i < n_r_bins; i++)
    {
        g_counts[i] = (double **)malloc(n_phi_bins * sizeof(double *));

        if (g_counts[i] == NULL)
        {
            fprintf(stderr, "Out of memory");
            exit(0);
        }

        for (int j = 0; j < n_phi_bins; j++)
        {
            g_counts[i][j] = (double *)malloc(n_theta_bins * sizeof(double));
            if (g_counts[i][j] == NULL)
            {
                fprintf(stderr, "Out of memory");
                exit(0);
            }
        }
    }

    for (i1 = 0; i1 < n_r_bins; i1++)
    {
        for (j1 = 0; j1 < n_phi_bins; j1++)
        {
            for (k1 = 0; k1 < n_theta_bins; k1++)
            {
                g_counts[i1][j1][k1] = 0;
            }
        }
    }

   //int counter = 0;
    FILE *fp1 = fopen("dipole_N2500_v0.xyz", "r");
    FILE *fp2 = fopen("g3_tp_rplot.dat", "w");
    
   

    for (t = 0; t < tmax; t++)
    {
        fscanf(fp1, "%d\n %lf\t%lf\t%lf\n", &N, &phi_r, &Fp, &times);
        for (i = 0; i < N; i++)
        {
            fscanf(fp1, "%lf\t%lf\t%lf\n", &particles[i].x, &particles[i].y, &particles[i].particle_theta);
        }

       // if (t > tsteady)
       if(t>0)        // First frame not spaced at t = 1.5 > Dr
        {
        tcount++;
            // Select a tagged particle
            for(i = 0; i < N-1 ; i++)
            {
            
            for (j = i+1; j < N; j++)
            {
            
            counter++;
                    // Calculate distance between particles (rij) and other variables
                    xij = particles[j].x - particles[i].x;
                    yij = particles[j].y - particles[i].y;

                    if (xij > (boxsize) * 0.5)
                    {
                        xij -= (boxsize);
                    }
                    if (xij <= -(boxsize) * 0.5)
                    {
                        xij += (boxsize);
                    }
                    if (yij > (boxsize) * 0.5)
                    {
                        yij -= (boxsize);
                    }
                    if (yij <= -(boxsize) * 0.5)
                    {
                        yij += (boxsize);
                    } // PBC along x and y;

                    // Calculate distance between particles
                    rij = pow(xij * xij + yij * yij, 0.5);

		    //For calculating the angle between relative distance and the direction of self-propulsion of tagged particle
		    
                    double quad_t = cos(particles[i].particle_theta) * yij / rij - sin(particles[i].particle_theta) * xij / rij;
                    double quad_ta = cos(particles[i].particle_theta) * xij / rij + sin(particles[i].particle_theta) * yij / rij;

                    if (quad_t < 0)
                    {
                        theta = -acos(quad_ta);
                    }
                    else
                    {
                        theta = acos(quad_ta);
                    }

		    if ((quad_ta <= -1.000000)&&(quad_ta>-1.000001))
                    {
                        theta = PI;
                    }
                    
                     if ((quad_ta >= 1.000000)&&(quad_ta<1.000001))
                    {
                        theta = 0;
                    }

                   
                    // Calculate relative difference in self-propulsion direction
                    double p_theta1_rounded = fmod(particles[j].particle_theta, 2.0*PI);
                    
                    if(p_theta1_rounded < 0){
                    	p_theta1_rounded += 2.0*PI;
                    	}
                    
                    double p_theta2_rounded = fmod(particles[i].particle_theta, 2.0*PI);
                    
                     if(p_theta2_rounded < 0){
                    	p_theta2_rounded += 2.0*PI;
                    	}
                    	
                    double theta_diff = p_theta1_rounded - p_theta2_rounded;
                    double theta_diff_og = theta_diff;
                    
                    if(theta_diff < 0){
                    	theta_diff += 2.0*PI;
                    	}
                    

                    // Calculate r_bin, phi_bin, and theta_bin
                    int r_bin = (int)(rij / dr);
                    int phi_bin = (int)((theta + PI) / dphi);        // Shift by PI to make range [0, 2,pi]
                    int theta_bin = (int)(theta_diff / dtheta);
                    if(theta_bin<0){ printf("theta_bin = %d\t theta_diff = %lf\t %lf\t%lf\n",theta_bin,theta_diff, p_theta1_rounded,p_theta2_rounded);}
                    if(phi_bin<0){ printf("phi_bin = %d\t theta = %lf\t quad_ta = %lf\n",phi_bin,theta,quad_ta);}
                   
                   
                    //printf("i = %d\t j = %d\t rij = %lf\t theta = %lf\t theta_diff = %lf\t theta_diff_og = %lf\tr_bin = %d\t phi_bin = %d\t theta_bin = %d\n",i,j,rij,theta,theta_diff,theta_diff_og,r_bin,phi_bin,theta_bin);
		    
                    if (r_bin < n_r_bins && phi_bin < n_phi_bins && theta_bin < n_theta_bins)
                    {
                        g_counts[r_bin][phi_bin][theta_bin]+=2;
                    }
                }
            }
        }
    }


 
    printf("counter= %d\n",counter);
    printf("Normalisation\t tcount = %d\n",tcount);
    double sum0=0.0,sum1 = 0.0;
    double g1,g2;
    
    // Calculate sum of g_counts
/*	double sum_counts = 0.0;
	for (i = 0; i < n_r_bins; i++) {
  	  for (j = 0; j < n_phi_bins; j++) {
       		 for (z = 0; z < n_theta_bins; z++) {
     	       sum_counts += g_counts[i][j][z];
      	  }
   	 }
	}
    printf("sum_counts = %lf\n",sum_counts);
 */   
    // Normalize g(r, \phi, theta)
   
   
     for (i = 0; i < n_r_bins; i++)
   	 {
   	  for (j = 0; j < n_phi_bins; j++)
           {
            for (z = 0; z < n_theta_bins; z++)
    		 {
                double r = (i + 0.5) * dr;
                double phi = (j + 0.5) * dphi - PI;
                if(phi<0) {phi += 2.0*PI;}    // phi = pi was calculated as front of the tagged particle. Now shifting the front to \phi = 0. Also wrapping phi in the range 0 to 2*pi instead of -pi to pi.
                double theta = (z + 0.5) * dtheta;

                // Calculate normalization factor
                double area = PI * (pow((i + 1) * dr, 2) - pow(i * dr, 2)) * dphi/(2.0*PI);  
                double norm = area * rho * tcount * N* dtheta / (2.0 *PI);   

                // Normalize g_counts
                 g1 = g_counts[i][j][z] / norm;
                // g2 = g_counts[i][j][z] / sum_counts;

                // Print or process the normalized pair distribution function
                fprintf(fp2, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",  r, phi, r * cos(phi), r * sin(phi), theta, g_counts[i][j][z], g1);
                sum0 +=g1;
                //sum1 +=g2;
            }
           // fprintf(fp2, "\n");
        }
       // fprintf(fp2, "\n");
    }

	printf("sum0 = %lf\n",sum0);
	//printf("sum1 = %lf\n",sum1);
    // Free memory
    for (i = 0; i < n_r_bins; i++)
    {
        for (j = 0; j < n_phi_bins; j++)
        {
            free(g_counts[i][j]);
        }
        free(g_counts[i]);
    }
    free(g_counts);
    free(particles);

    fclose(fp0);
    fclose(fp1);
    fclose(fp2);

    return 0;
}


#include <stdio.h>
#include <math.h>
#include <complex.h>

// Define the constants
#define N_r 700    // Number of intervals for r
#define N_phi 80   // Number of intervals for phi
#define N_theta 80 // Number of intervals for theta

#define PI 3.14159265358979323846

// Function to compute T(r)
double computeT(double r) {
    double epsilon = 1.0;
    double lambda = 40.0;
    double dip_length = 3.0/16.0;
    double dipole_head = -53.39;
    double dipole_tail = 112.39;
    double tmag;
    double factor;
    factor = 3*exp(-r/lambda)/(4*PI*epsilon*pow(r,5));
    tmag = factor*dip_length*((dipole_head*dipole_head)-
                                      (dipole_tail*dipole_tail));
            //printf("tmag = %lf\n",tmag);
    return tmag;
   }

int main() {
    // Variables for integration
    double delta_r, delta_phi, delta_theta;
    double r,r1, phi, theta;
    double x,y;
    double g_tot,g1;
    double xi_0k = 0.0;
    double a_p1,a_p2;
    double Dr = 1.0;
    double xi_r = 3;
    double phi_g = 0.1;
    double R = 0.5;
    double pi = 4.0*atan(1.0);
    double rho = phi_g/(pi*R*R);
    double v0 = 22;
    
    //double g_r_phi_theta;
    
    // Step sizes for integration
    delta_r = 0.1;
    delta_phi = 2.0 * PI / N_phi;
    delta_theta = 2.0 * PI / N_theta;

    FILE *fp1 = fopen("g3_av_all_tp_rplot_calculation.dat","r");
    FILE *fp2 = fopen("polar_transport_coefficient.dat","w");
    
    int count = 0;
    //int N_r = 700;
    // Loop over r, phi, and theta
    for (int i = 0; i < N_r; i++) {
        r = (i+0.5) * delta_r;
        double T_r = computeT(r);

        for (int j = 0; j < N_phi; j++) {
            //phi = (j+0.5) * delta_phi;

            for (int k = 0; k < N_theta; k++) {
                //theta = (k+0.5) * delta_theta;
                count++;
                //if(count%6400==0){fscanf(fp1,"\n");}
                fscanf(fp1,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",&r1,&phi,&x,&y,&theta,&g_tot,&g1);
                
                printf("%d\t%lf\t%lf\n",count,r,g1);
                

                // Accumulate the contribution to xi_0k
                xi_0k += T_r * r1* delta_r * delta_phi * delta_theta * sin(phi) * sin (theta) * g1;
            }
        }
    }

       // Print the result
    printf("xi_0k: %f\n", xi_0k);
    a_p1 = rho*xi_0k/(2.0*pi*xi_r) - Dr;
    a_p2 = phi_g*xi_0k/(2.0*pi*xi_r) - Dr; 
    fprintf(fp2,"%lf\t%lf\t%lf\t%lf\t%lf\n",phi_g,v0,xi_0k,a_p1,a_p2);
    	

    return 0;
}


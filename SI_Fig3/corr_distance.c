#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {

    int t, tmax=22;  
    int i, j, N_max=12800,N,N1,times1,times2,times3,times4;
    double *x = (double *)malloc((N_max + 1) * sizeof(double)); 
    double *y = (double *)malloc((N_max + 1) * sizeof(double)); 
    double *theta = (double *)malloc((N_max + 1) * sizeof(double));
    double phi = 0.05;  
    double pi = 4.0*atan(1.0);
    double R_av = 2.5;                                                  // particle radius in pixel
    double sigma = 5.0;                                                 // particle diameter in pixel
    double boxsize = pow(((N_max*pi*sigma*sigma) / (4.0 * phi)), 0.5);  // boxsize in pixel
    
    double binwidth = 4.0*sigma;
    int rindex;
    int r_index_max = (boxsize/(sqrt(2.0)));
    double corr[r_index_max];
    double xij, yij, rij, add;
    int npairs[r_index_max];
    int count_line;
    double rho = phi/(pi*R_av*R_av);
    double av_ip_dist = sqrt(1.0/rho);                                 // average interparticle distance in pixel
    double conversion = 0.6;                                           // 1 pixel = 0.6 \mu m
    double threshold1 = 5.0;
    double threshold2 = 10.0;
     
    FILE *fp1 = fopen("../../output_config.dat", "r");
    FILE *fp3 = fopen("../../particle_vel_orientation.dat","r");
    FILE *fp2 = fopen("orientation_corr_rav_data_scaled.dat", "w");
    FILE *fp4 = fopen("average_interparticle_distance.dat","w");
    
    fprintf(fp4,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",av_ip_dist,av_ip_dist*conversion,threshold1*av_ip_dist,threshold1*av_ip_dist*conversion,threshold2*av_ip_dist,threshold2*av_ip_dist*conversion);
       
    for (t = 0; t < tmax; t++) { printf("t = %d\n",t);
        //fscanf(fp0, "%lf\t%lf\n", &R_av,&eb);
        count_line=0;
        
        fscanf(fp1, "%d\t%d\n%d\n", &times1,&times2,&N);
        fscanf(fp3, "%d\t%d\n%d\n", &times3,&times4,&N1);
        
        
        for (i = 0; i < N; i++) {
            fscanf(fp1, "%lf\t%lf\n", &x[i], &y[i]);
            //printf("%lf\t%lf\n", x[i], y[i]);
        }
        
         for (i = 0; i < N1; i++) {
            fscanf(fp3, "%lf\n", &theta[i]);
            //printf("%lf\n", theta[i]);
        }
        
       for (i = 0; i < r_index_max; i++) {
        corr[i] = 0;
        npairs[i] = 0;
        }
      
        if(N1!=0){  
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (i != j) {			
                   
                  xij = (x[j] - x[i]);
                  yij = (y[j] - y[i]);

		   if (xij > (boxsize) * 0.5) {
                        xij -= (boxsize);
                    }
                    if (xij <= -(boxsize) * 0.5) {
                        xij += (boxsize);
                    }
                    if (yij > (boxsize) * 0.5) {
                        yij -= (boxsize);
                    }
                    if (yij <= -(boxsize) * 0.5) {
                        yij += (boxsize);
                    }   // PBC along x and y;

	
         
                    // Calculate distance between particles
                    rij = pow(xij * xij + yij * yij, 0.5);   // interparticle distance in pixel 
                    rindex = (int)(rij / binwidth);
                    //printf("%lf\t %d \n",rij,rindex);
                    corr[rindex] += cos(theta[i] - theta[j]);
                    npairs[rindex]++;
                }
            }
        }
       
       
 
     
      for (j = 0; j < r_index_max; j++) {
        if (npairs[j] > 0) {
            fprintf(fp2, "%lf\t%lf\t%d\n", (j+0.5)*binwidth,corr[j]/(npairs[j]),times2);
            count_line++;
			}}
	    //fprintf(fp3,"%d\t%d\n",times,count_line);		
			}
			}
    

    }

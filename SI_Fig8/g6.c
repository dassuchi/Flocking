//  hexatic_order.c
//  
//
//  Created by Suchismita Das on 25.01.23.
//

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <complex.h>

int main(){
    int nt, time,t_read;
    int i, j, N = 2500;
    double phi = 0.55;
    int file,line;
    FILE *fp0 = fopen("nline.dat","r");
    fscanf(fp0,"%d\t%d\t%d\n",&file,&line,&time);
    printf("time = %d\n",time);

    double **x = (double **)malloc((time+1)*sizeof(double *));
	for(i = 0; i < time+1; i++){
		x[i] = (double *)malloc((N+1)*sizeof(double));
	}

    double **y = (double **)malloc((time+1)*sizeof(double *));
	for(i = 0; i < time+1; i++){
		y[i] = (double *)malloc((N+1)*sizeof(double));
	}

    int p_index,tot_neighbour,neighbour_id;
    double dx,dy,L;
    double pi = 4.0*atan(1.0);
    L = pow(((N*pi)/(4.0*phi)),0.5);
    double theta,r;

    double **hexatic_order_real = (double **)malloc((time+1)*sizeof(double *));
	for(i = 0; i < time+1; i++){
		hexatic_order_real[i] = (double *)malloc((N+1)*sizeof(double));
	}

    double **hexatic_order_imag = (double **)malloc((time+1)*sizeof(double *));
	for(i = 0; i < time+1; i++){
		hexatic_order_imag[i] = (double *)malloc((N+1)*sizeof(double));
	} 

    double **hexatic_order_mag = (double **)malloc((time+1)*sizeof(double *));
	for(i = 0; i < time+1; i++){
		hexatic_order_mag[i] = (double *)malloc((N+1)*sizeof(double));
	} 

    double complex psi_6;
   
    double global_hexatic_order;   
    double Gamma_hexatic_order;
   
    
    FILE *fp1 = fopen("config_time.dat","r");
    
    for(nt = 0; nt < time; nt++){
        for(i = 0; i <N; i++){
            fscanf(fp1,"%lf\t%lf\n",&x[nt][i],&y[nt][i]);
            //printf("%lf\n",y[nt][i]);
        }}
    
    FILE *fp2 = fopen("voronoi_neighbours_time.txt","r");
    //FILE *fp3 = fopen("hexatic_order_time.txt","w");
    
    for(nt = 0 ; nt < time; nt++){
         fscanf(fp2,"%d\n",&t_read);
         
        for(i = 0; i < N; i++){
            //printf("i = %d\n",i);
            hexatic_order_real[nt][i]=0;
            hexatic_order_imag[nt][i]=0;
	        hexatic_order_mag[nt][i]=0;
            fscanf(fp2,"%d\t%d\n",&p_index,&tot_neighbour);
            //printf("%d\t%d\n",p_index,tot_neighbour);
            
            for(j = 0; j < tot_neighbour; j++){
                fscanf(fp2,"%d\n",&neighbour_id);
                //printf("%d\n",neighbour_id);
                dx = x[nt][neighbour_id] - x[nt][p_index];
                dy = y[nt][neighbour_id] - y[nt][p_index];
                if (dx > L * 0.5) {dx -= L;}
                if (dx <= -L * 0.5) {dx +=L;}
                if (dy > L * 0.5) {dy -= L;}
                if (dy <= -L * 0.5) {dy +=L;}
                theta = atan2(dy,dx);
                hexatic_order_real[nt][p_index]+=cos(6.0*theta);
                hexatic_order_imag[nt][p_index]+=sin(6.0*theta);
            }
            hexatic_order_real[nt][p_index]/=tot_neighbour;
            hexatic_order_imag[nt][p_index]/=tot_neighbour;
            hexatic_order_mag[nt][p_index]=pow((hexatic_order_real[nt][p_index]*hexatic_order_real[nt][p_index]+hexatic_order_imag[nt][p_index]*hexatic_order_imag[nt][p_index]),0.5);
            //fprintf(fp3,"%d\t%lf\t%lf\t%lf\n",i,x[nt][p_index],y[nt][p_index],hexatic_order_mag[nt][p_index]);
        }
    }
    
   // Parameters for g_6(r) calculation
    int num_bins = 100; // Adjust as needed
    double max_r = L / 2.0; // Maximum distance to consider
    double bin_size = max_r / num_bins;
    
    // Arrays for storing g_6(r) values and counts
    double *g_6_num = (double *)malloc(num_bins * sizeof(double));
    double *g_6_den = (double *)malloc(num_bins * sizeof(double));
    int *count = (int *)malloc(num_bins * sizeof(int));

    // File to write g_6(r) data
    FILE *fp_g6 = fopen("g6.dat", "w");

    // Calculate g_6(r) for each time step
    for (nt = 0; nt < time; nt++) {

        for (i = 0; i < N; i++) {
            for (j = i + 1; j < N; j++) {
                dx = x[nt][j] - x[nt][i];
                dy = y[nt][j] - y[nt][i];
                // Apply periodic boundary conditions
                if (dx > L * 0.5) { dx -= L; }
                if (dx <= -L * 0.5) { dx += L; }
                if (dy > L * 0.5) { dy -= L; }
                if (dy <= -L * 0.5) { dy += L; }

                r = sqrt(dx * dx + dy * dy);
                int bin = (int)(r / bin_size);
                if (bin < num_bins) {
                    double psi_6i = hexatic_order_real[nt][i] + I*hexatic_order_imag[nt][i];
                    double psi_6j = hexatic_order_real[nt][j] - I*hexatic_order_imag[nt][j];
                    double psi_6i_star = hexatic_order_real[nt][i] - I*hexatic_order_imag[nt][i];
                    g_6_num[bin] += psi_6i * psi_6j;
                    g_6_den[bin] += psi_6i * psi_6i_star;
                    count[bin]++;
                }
            }
        }}

        // Normalize and output g_6(r) for this time step
        for (int bin = 0; bin < num_bins; bin++) {
            
            fprintf(fp_g6, "%lf\t%lf\t%lf\n", (bin+0.5) * bin_size, g_6_num[bin]/count[bin],g_6_num[bin]/g_6_den[bin]);
        }
    

    fclose(fp_g6);
    free(g_6_num);
    free(g_6_den);
    free(count);

    
}


    
    
    


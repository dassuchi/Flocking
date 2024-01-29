//
//  hexatic_order.c
//  
//
//  Created by Suchismita Das on 25.01.23.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

int main(){
    int nt, time,t_read;
    int i, j, N = 2500;
    double phi = 0.1;
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
	
    double complex **hexatic_order_complex = (double complex **)malloc((time+1)*sizeof(double complex *));
	for(i = 0; i < time+1; i++){
		hexatic_order_complex[i] = (double complex *)malloc((N+1)*sizeof(double complex));
	} 	
	
     double dot_product(complex double z1, complex double z2){
     return creal(z1)*creal(z2) + cimag(z1)*cimag(z2);
     }	
     
     double angle(complex double z1, complex double z2){
     double dot = dot_product(z1,z2);
     double norm_z1 = cabs(z1);
     double norm_z2 = cabs(z2);
     return (dot/(norm_z1*norm_z2));
     }
     
     double angle_actual(complex double z1, complex double z2){
     complex double z = z1 / z2;
     return atan2(cimag(z), creal(z));
     }

     double alpha,beta,beta_mean=0.0,beta_diff;
    double complex psi_6;
   
    double global_hexatic_order_time;   
    double v0 = 122;
    
    FILE *fp1 = fopen("config_time.dat","r");
    
    for(nt = 0; nt < time; nt++){
        for(i = 0; i <N; i++){
            fscanf(fp1,"%lf\t%lf\n",&x[nt][i],&y[nt][i]);
            //printf("%lf\n",y[nt][i]);
        }}
    
    FILE *fp2 = fopen("voronoi_neighbours_time.txt","r");
    FILE *fp3 = fopen("local_hexatic_order_angle_complex_argument_time.txt","w");
    //FILE *fp4 = fopen("local_hexatic_order_mag_time.txt","w");
    
    for(nt = 0 ; nt < time; nt++){
         fscanf(fp2,"%d\n",&t_read);
         psi_6 = 0.0 + 0.0*I;
         global_hexatic_order_time=0.0;
        for(i = 0; i < N; i++){
            printf("i = %d\n",i);
            hexatic_order_real[nt][i]=0;
            hexatic_order_imag[nt][i]=0;
	    hexatic_order_mag[nt][i]=0;
            fscanf(fp2,"%d\t%d\n",&p_index,&tot_neighbour);
            printf("%d\t%d\n",p_index,tot_neighbour);
            
            for(j = 0; j < tot_neighbour; j++){
                fscanf(fp2,"%d\n",&neighbour_id);
                printf("%d\n",neighbour_id);
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
            hexatic_order_complex[nt][p_index]=hexatic_order_real[nt][p_index] + I*hexatic_order_imag[nt][p_index];
            psi_6 += hexatic_order_real[nt][p_index] + I*hexatic_order_imag[nt][p_index];
	    hexatic_order_mag[nt][p_index]=pow((hexatic_order_real[nt][p_index]*hexatic_order_real[nt][p_index]+hexatic_order_imag[nt][p_index]*hexatic_order_imag[nt][p_index]),0.5);
	    //if(nt==time-1){
            //fprintf(fp4,"%d\t%lf\t%lf\t%lf\n",i,x[nt][p_index],y[nt][p_index],hexatic_order_mag[nt][p_index]);
        //}
        }
        global_hexatic_order_time = psi_6/N;
        if(nt==time-1){
         for(i = 0; i < N; i++){
            alpha = angle(hexatic_order_complex[nt][i],global_hexatic_order_time);
            beta = angle_actual(hexatic_order_complex[nt][i],global_hexatic_order_time);
            fprintf(fp3,"%d\t%lf\t%lf\t%lf\t%lf\n",i,x[nt][i],y[nt][i],alpha,beta);       
            
            }
         }}
        }
    
      

    


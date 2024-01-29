//
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
    
      

    FILE *fp4 = fopen("hexatic_order_time.dat","w");

    
    for(nt = 0; nt < time; nt++){
    
      psi_6 = 0.0 + 0.0*I;
      Gamma_hexatic_order = 0.0;
      
      for(i = 0; i < N; i++){
        psi_6 += hexatic_order_real[nt][i] + I*hexatic_order_imag[nt][i];
	Gamma_hexatic_order +=hexatic_order_mag[nt][i];}
	
    global_hexatic_order = cabs(psi_6)/N;	
    Gamma_hexatic_order/=N;
    fprintf(fp4,"%d\t%lf\t%lf\n",nt,global_hexatic_order,Gamma_hexatic_order);
      }
    
    
}

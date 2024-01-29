#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

int main(){
	int i, j, nfile = 20; 
	int nline = 4480000;
	double r,phi,theta,rcos,rsin,g_tot,g_scaled1;
	double* r_sum = (double*)malloc(nline * sizeof(double));
	double* phi_sum = (double*)malloc(nline * sizeof(double));
	double* theta_sum = (double*)malloc(nline * sizeof(double));
	double* rcos_sum = (double*)malloc(nline * sizeof(double));
	double* rsin_sum = (double*)malloc(nline * sizeof(double));
	double* g_tot_sum = (double*)malloc(nline * sizeof(double));
	double* g_scaled_sum1 = (double*)malloc(nline * sizeof(double));
	

	int i1,j1,k1;
	
	int N = 2500;                   // Total number of particles
 	double phi_g = 0.1;             // Global area fraction
        double boxsize = pow(((N * PI) / (4.0 * phi_g)), 0.5); // Size of simulation box
        double dr = 0.1;                // Width of radial bins
        double dphi = PI/40.0;       
        double dtheta = PI/40.0;      
   	double rho = N / (boxsize * boxsize);
   	
   	
	// Allocate memory for pair distribution function
        int n_r_bins = (int)(boxsize / (2.0*dr));
        int n_phi_bins = (int)(2.0 * PI / dphi);
        int n_theta_bins = (int)(2.0* PI / dtheta); 
        
        
	FILE *fpo = fopen("g3_av_all_tp_rplot_calculation.dat","w");
	
	for(i = 0; i < nline; i++){
		r_sum[i]=0.0;
		theta_sum[i]=0.0;
		phi_sum[i]=0.0;
		rcos_sum[i]=0.0;
		rsin_sum[i]=0.0;
		g_tot_sum[i]=0.0;
		g_scaled_sum1[i]=0.0;
		}
		
	char filename[50];
	
	
	for(i = 1; i <= nfile ; i++){  //printf("%d\t",i);
   	
  	sprintf(filename, "dir%d/g3_tp_rplot.dat", i); 

  	FILE *fp_particle = fopen(filename, "r"); 

		for(j = 0; j < nline; j++){
		fscanf(fp_particle,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",&r,&phi,&rcos,&rsin,&theta,&g_tot,&g_scaled1);
		r_sum[j]+=r;
		phi_sum[j]+=phi;
		rcos_sum[j]+=rcos;
		rsin_sum[j]+=rsin;
		theta_sum[j]+=theta;
		g_tot_sum[j]+=g_tot;
		g_scaled_sum1[j]+=g_scaled1;
		if(j==2){printf("%lf\t%lf\n",r_sum[j],g_scaled_sum1[j]);}
		
			}}
		
		
		
	
	for(i = 0; i < nline; i++){ //printf("i = %d\t nline = %d\n",i,nline);
		r_sum[i]/=nfile;
		phi_sum[i]/=nfile;
		rcos_sum[i]/=nfile;
		rsin_sum[i]/=nfile;
		theta_sum[i]/=nfile;
		g_tot_sum[i]/=nfile;
		g_scaled_sum1[i]/=nfile;
		fprintf(fpo,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",r_sum[i],phi_sum[i],rcos_sum[i],rsin_sum[i],theta_sum[i],g_tot_sum[i],g_scaled_sum1[i]); 
		//if((i!=0)&&(i%(6400)==0)){fprintf(fpo,"\n");}
		}
		}	

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {

    int t, tmax=11;  
    double t1,corr_length_pixel,corr_length_fit_err,corr_length_mu,corr_length_mu_err;
    double threshold1_pixel,threshold2_pixel,threshold1_mu,threshold2_mu;
    double corr_length_tav_mu = 0.0;
    double av_ip_dist_pixel,av_ip_dist_mu;
    char c = 'I';
    double phi_0 = 0.05,phi_0_sd = 0.002;
    double v0 = 20.4,v0_sd = 6.4;
    
    FILE *fp1 = fopen("correlation_length_vs_time_rav_data_us_df_pixel.dat","r");
    FILE *fp2 = fopen("average_interparticle_distance.dat","r");
    FILE *fp3 = fopen("check_transition_r10.dat","w");
    
    fscanf(fp2,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",&av_ip_dist_pixel,&av_ip_dist_mu,&threshold1_pixel,&threshold1_mu,&threshold2_pixel,&threshold2_mu);
    
    for(t = 0; t < tmax; t++){
    	fscanf(fp1,"%lf\t%lf\t%lf\t%lf\t%lf\n",&t1,&corr_length_pixel,&corr_length_fit_err,&corr_length_mu,&corr_length_mu_err);
    	corr_length_tav_mu+=corr_length_mu;
    	}
    	corr_length_tav_mu/=tmax;
    	//if(corr_length_tav_mu>threshold1_mu){c = 'F';}
    	//fprintf(fp3,"%lf\t%lf\t%c\n",corr_length_tav_mu,threshold1_mu,c);
    	//c = 'I';
    	if(corr_length_tav_mu>threshold2_mu){c = 'F';}
    	fprintf(fp3,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%c\n",phi_0,phi_0_sd,v0,v0_sd,corr_length_tav_mu,threshold2_mu,c);
    	}
    	
   
    	

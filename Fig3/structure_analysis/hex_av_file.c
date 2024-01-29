#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
	int nfile = 6;
	int i;
	FILE*fp1;
	char fn[50];
	double psi6,psi6_sd,Gamma6,Gamma6_sd;
	double psi6_sum,Gamma6_sum,psi6_sd_sum,Gamma6_sd_sum;
	double psi6sq,Gamma6sq,err_b_run_psi6,err_b_run_Gamma6;
	double phi=0.1;
	double Pe=122;
	
		psi6_sum=0.0;
		Gamma6_sum=0.0;
		psi6_sd_sum=0.0;
		Gamma6_sd_sum=0.0;	
	
	for(i=0;i<nfile;i++){
	sprintf(fn,"%d/hex_op_tav.dat",i);
	fp1 = fopen(fn,"r");

	fscanf(fp1,"%lf\t%lf\t%lf\t%lf\n",&psi6,&psi6_sd,&Gamma6,&Gamma6_sd);
	psi6_sum+=psi6;
	Gamma6_sum+=Gamma6;
	psi6_sd_sum+=psi6_sd;
	Gamma6_sd_sum+=Gamma6_sd;
        psi6sq+=psi6*psi6;
        Gamma6sq+=Gamma6*Gamma6;
	}

	FILE*fp2=fopen("hex_order_av.dat","w");

	psi6_sum/=nfile;
	Gamma6_sum/=nfile;
	psi6_sd_sum/=nfile;
	Gamma6_sd_sum/=nfile;
	psi6sq/=nfile;
	Gamma6sq/=nfile;
	err_b_run_psi6=sqrt(psi6sq - psi6_sum*psi6_sum);
	err_b_run_Gamma6=sqrt(Gamma6sq - Gamma6_sum*Gamma6_sum);
	fprintf(fp2,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",phi,Pe,psi6_sum,psi6_sd_sum,Gamma6_sum,Gamma6_sd_sum,err_b_run_psi6,err_b_run_Gamma6);	
				

	
	}
		


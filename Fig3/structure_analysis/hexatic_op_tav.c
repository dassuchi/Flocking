#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
int file,line;
int tmax;
int t1;
FILE *fp0 = fopen("nline.dat","r");
fscanf(fp0,"%d\t%d\t%d\n",&file,&line,&tmax);
int i;
double psi_6,Gamma_6;
double psi6_sum=0.0,Gamma6_sum=0.0;
double psi6_sq=0.0,Gamma6_sq=0.0;
double psi6_err,Gamma6_err;


FILE *fp1 = fopen("hexatic_order_time.dat","r");
FILE *fp2 = fopen("hex_op_tav.dat","w");

for(i = 0; i < tmax; i++){ 
	fscanf(fp1,"%d\t%lf\t%lf\n",&t1,&psi_6,&Gamma_6);
	psi6_sum += psi_6;
	psi6_sq+=psi_6*psi_6;
	Gamma6_sum+= Gamma_6;
	Gamma6_sq+=Gamma_6*Gamma_6;
	}
	psi6_sum/=(tmax);
	psi6_sq/=(tmax);
	psi6_err = sqrt(psi6_sq-psi6_sum*psi6_sum);
	Gamma6_sum/=tmax;
	Gamma6_sq/=tmax;
	Gamma6_err = sqrt(Gamma6_sq-Gamma6_sum*Gamma6_sum);
	fprintf(fp2,"%lf\t%lf\t%lf\t%lf\n",psi6_sum,psi6_err,Gamma6_sum,Gamma6_err);	

		
}
		

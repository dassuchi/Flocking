#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
int file,line;
int tmax = 92;
double t1;
int i;
double p_op;
double p_sum=0.0;
double psq=0.0;
double p_err;
int tsteady = 0;
int count = 0;
double phi = 0.03;
double v0 = 19.1;

FILE *fp1 = fopen("polarity_op_PD_or.dat","r");
FILE *fp2 = fopen("pol_op_tav.dat","w");

for(i = 0; i < tmax; i++){ 
	fscanf(fp1,"%lf\t%lf\n",&t1,&p_op);
	printf("t1 = %lf\n",t1);
	if(i>=tsteady){
	p_sum += p_op;
	psq+=p_op*p_op;
	count++;
	}}
	p_sum/=(tmax-tsteady);
	psq/=(tmax-tsteady);
	//printf("count = %d\t tmax-tsteady = %d\n",count,tmax-tsteady);
	p_err = sqrt(psq-p_sum*p_sum);
	fprintf(fp2,"%lf\t%lf\t%lf\t%lf\n",phi,v0,p_sum,p_err);	

		
}
		

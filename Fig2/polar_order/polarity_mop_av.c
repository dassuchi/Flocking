#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
int file,line;
int tmax;
double t1;
FILE *fp0 = fopen("nline.dat","r");
fscanf(fp0,"%d\t%d\t%d\n",&file,&line,&tmax);
int i;
double p_op;
double p_sum=0.0;
double psq=0.0;
double p_err;

FILE *fp1 = fopen("polarity_op.dat","r");
FILE *fp2 = fopen("pol_op_tav.dat","w");

for(i = 0; i < tmax; i++){ 
	fscanf(fp1,"%lf\t%lf\n",&t1,&p_op);
	p_sum += p_op;
	psq+=p_op*p_op;
	}
	p_sum/=tmax;
	psq/=tmax;
	p_err = sqrt(psq-p_sum*p_sum);
	fprintf(fp2,"%lf\t%lf\n",p_sum,p_err);	

		
}
		

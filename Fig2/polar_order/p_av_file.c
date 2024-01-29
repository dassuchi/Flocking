#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
	int nfile = 6;
	int i;
	FILE*fp1;
	char fn[50];
	double p,eb;
	double p_sum,eb_sum;
	double psq,err_b_run;
	double phi = 0.1;
	double Pe = 22;
	
		p_sum=0.0;
		eb_sum=0.0;
			
	
	for(i=0;i<nfile;i++){
	sprintf(fn,"%d/pol_op_tav.dat",i);
	fp1 = fopen(fn,"r");

	fscanf(fp1,"%lf\t%lf\n",&p,&eb);
	p_sum+=p;
	eb_sum+=eb;
        psq+=p*p;
	}

	FILE*fp2=fopen("polarity_op.dat","w");

	p_sum/=nfile;
	eb_sum/=nfile;
	psq/=nfile;
	err_b_run=sqrt(psq - p_sum*p_sum);
	fprintf(fp2,"%lf\t%lf\t%lf\t%lf\t%lf\n",phi,Pe,p_sum,eb_sum,err_b_run);	
				

	
	}
		


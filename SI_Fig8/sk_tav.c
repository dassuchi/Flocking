#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){

	double tread;
	int file,line,tmax;
       
        FILE *fp0 = fopen("nline.dat","r");
        fscanf(fp0,"%d\t%d\t%d\n",&file,&line,&tmax);
        printf("tmax = %d\n",tmax);
	
	int t1;
	int i,nline=3737;
	double k[nline],Sk[nline];
	double k_av[nline],Sk_av[nline];
	
	for(i = 0; i < nline; i++){
		k_av[i]=0;
		Sk_av[i]=0;
		}
	
	FILE *fp1 = fopen("sk.dat","r");
	FILE *fp2 = fopen("sk_tav.dat","w");
	
	for(t1=0; t1< tmax; t1++){
		fscanf(fp1,"%lf\n",&tread);	 
		for(i = 0; i < nline; i++){
			fscanf(fp1,"%lf\t%lf\n",&k[i],&Sk[i]);
			//printf("%lf\t%lf\n",k[i],Sk[i]);
			k_av[i]+=k[i];
			Sk_av[i]+=Sk[i];
			}}
			
	for(i = 0; i < nline; i++){
		k_av[i]/=tmax;
		Sk_av[i]/=tmax;
		//printf("%lf\t%lf\n",k_av[i],Sk_av[i]);
		fprintf(fp2,"%lf\t%lf\n",k_av[i],Sk_av[i]);
		}			
		}

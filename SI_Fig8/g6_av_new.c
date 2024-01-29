#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
	int nfile = 6;
	int i,j;
	FILE*fp1;
	char fn[30];
	int nline = 100;
	double y[nline],g6_count[nline],g6[nline];
	double y_sum[nline],g6_count_sum[nline],g6_sum[nline];
	
	for(i=0;i<nline;i++){
		y_sum[i]=0.0;
		g6_count_sum[i]=0.0;
		g6_sum[i]=0.0;
			}
	
	for(i=0;i<nfile;i++){
	sprintf(fn,"%d/g6.dat",i);
	fp1 = fopen(fn,"r");

	for(j=0;j<nline;j++){
		fscanf(fp1,"%lf\t%lf\t%lf\n",&y[j],&g6_count[j],&g6[j]);
		y_sum[j]+=y[j];
		g6_count_sum[j]+=g6_count[j];
		g6_sum[j]+=g6[j];
			}}

	FILE*fp2=fopen("g6_file_av.dat","w");

	for(i=0;i<nline;i++){
		y_sum[i]/=nfile;
		g6_count_sum[i]/=nfile;
		g6_sum[i]/=nfile;
		fprintf(fp2,"%lf\t%lf\t%lf\n",y_sum[i],g6_count_sum[i],g6_sum[i]);	
				}

		
}
		

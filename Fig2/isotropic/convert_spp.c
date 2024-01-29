#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){

int t,tmax;  
int file,line;
int N,i;
double phi,boxsize,Fp,times;
char c;
double x,y,spp_theta;
int z;
FILE *fp0 = fopen("nline.dat","r");
fscanf(fp0,"%d\t%d\t%d\n",&file,&line,&tmax);

FILE *fp1 = fopen("dipole.xyz","r");
FILE *fp2 = fopen("config_time_spp.dat","w");

for(t = 0; t < tmax; t++){
	fscanf(fp1,"%d\t%lf\t%lf\t%lf\t%lf\n",&N,&phi,&boxsize,&Fp,&times);
	for(i = 0; i < N; i++){
		fscanf(fp1,"%c\t%lf\t%lf\t%d\t%lf\n",&c,&x,&y,&z,&spp_theta);
                fprintf(fp2,"%lf\t%lf\t%lf\n",x,y,spp_theta);
				}}}







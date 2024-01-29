#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
int t,tmax;
double t1;
double x,y,theta;
int i,N;
int file,line;
FILE *fp0 = fopen("nline.dat","r");
fscanf(fp0,"%d\t%d\t%d\n",&file,&line,&tmax);
int tsteady = 0;
double pi = 4.0*atan(1.0);
double phi,boxsize,Fp,times;
char c;
int z;

FILE *fp1 = fopen("dipole.xyz","r");
FILE *fp2 = fopen("polarity_op.dat","w");

double px_sum=0.0,py_sum=0.0;
double p_mag_m;
 
for(t = 0; t < tmax; t++){ 
	fscanf(fp1,"%d\t%lf\t%lf\t%lf\t%lf\n",&N,&phi,&boxsize,&Fp,&times);
        px_sum=0.0;
        py_sum=0.0;
     
	for(i = 0; i < N; i++){
	        fscanf(fp1,"%c\t%lf\t%lf\t%d\t%lf\n",&c,&x,&y,&z,&theta);
		px_sum+=cos(theta);
		py_sum+=sin(theta);
        	}
		p_mag_m = (sqrt(px_sum*px_sum+py_sum*py_sum))/N;
		fprintf(fp2,"%lf\t%lf\n",times,p_mag_m);

		}
        

}

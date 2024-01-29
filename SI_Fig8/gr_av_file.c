#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
	int nfile = 6;
	int i,j;
	FILE*fp1;
	char fn[50];
        int nline = 298;
        double r[nline],gr[nline];
        double r_sum[nline],gr_sum[nline];

        for(i=0;i<nline;i++){
                r_sum[i]=0.0;
                gr_sum[i]=0.0;
                        }

			
        for(i=0;i<nfile;i++){
        sprintf(fn,"%d/gr_tav.dat",i);
        fp1 = fopen(fn,"r");

        for(j=0;j<nline;j++){
                fscanf(fp1,"%lf\t%lf\n",&r[j],&gr[j]);
                r_sum[j]+=r[j];
                gr_sum[j]+=gr[j];
                        }}
	

	FILE*fp2=fopen("gr_fav.dat","w");

        for(i=0;i<nline;i++){
                r_sum[i]/=nfile;
                gr_sum[i]/=nfile;
                fprintf(fp2,"%lf\t%lf\n",r_sum[i],gr_sum[i]);
                                }
				

	
	}
		


		

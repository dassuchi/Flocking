#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
	int nfile = 6;
	int i,j;
	FILE*fp1;
	char fn[50];
        int nline = 3737;
        double k[nline],S[nline];
        double k_sum[nline],S_sum[nline];

        for(i=0;i<nline;i++){
                k_sum[i]=0.0;
                S_sum[i]=0.0;
                        }

			
        for(i=0;i<nfile;i++){
        sprintf(fn,"%d/sk_tav.dat",i);
        fp1 = fopen(fn,"r");

        for(j=0;j<nline;j++){
                fscanf(fp1,"%lf\t%lf\n",&k[j],&S[j]);
                k_sum[j]+=k[j];
                S_sum[j]+=S[j];
                        }}
	

	FILE*fp2=fopen("sk_fav.dat","w");

        for(i=0;i<nline;i++){
                k_sum[i]/=nfile;
                S_sum[i]/=nfile;
                fprintf(fp2,"%lf\t%lf\n",k_sum[i],S_sum[i]);
                                }
				

	
	}
		


		

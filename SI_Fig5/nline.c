#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main(){
	int nfile = 1;
	FILE *fp1;
	char fn1[100];
	int count;
	int i,j=0;
	char c;
	int tmax;
        int N = 1024;


	FILE *fp2 = fopen("nline.dat","w");

	for(i = 0; i < nfile; i++){
		sprintf(fn1,"dipole.xyz");
		fp1 = fopen(fn1,"r");
		count = 0;
		for(c = getc(fp1); c!=EOF; c = getc(fp1))
			if(c== '\n')
				count++;
        //printf("30\n");
		tmax = (count/(N+2));  // (N+2) lines written in xyz files for every snapshot
		fprintf(fp2,"%d\t%d\t%d\n",j,count,tmax);
	fflush(fp2);
	}



	}
	

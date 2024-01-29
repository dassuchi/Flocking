/*Time Averaged Particle Density*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void free_data(double ***data, size_t xlen, size_t ylen)
{
	size_t i,j;

	for(i=0;i<xlen;++i){
		if(data[i]!=NULL){
			for(j=0;j<ylen;++j)
				free(data[i][j]);
			free(data[i]);
		}
	}
	free(data);
}


double ***alloc_data(size_t xlen, size_t ylen, size_t zlen)
{
	double ***p;
	size_t i,j;

	if((p=malloc(xlen*sizeof *p))==NULL){
		perror("malloc 1");
		return NULL;
	}

	for(i=0;i<xlen;++i)
		p[i]=NULL;

	for(i=0;i<xlen;++i)
		if((p[i]=malloc(ylen*sizeof *p[i]))==NULL){
			perror("malloc 2");
			free_data(p,xlen,ylen);
			return NULL;
		}

	for(i=0;i<xlen;++i)
		for(j=0;j<ylen;++j)
			p[i][j]=NULL;

	for(i=0;i<xlen;++i)
		for(j=0;j<ylen;++j)
			if((p[i][j]=malloc(zlen*sizeof *p[i][j]))==NULL){
				perror("malloc 3");
				free_data(p, xlen, ylen);
				return NULL;
			}
	
	return p;
}



int main(){

int xindex,yindex,N=1024,i,j,N_read,z,count=0,t1,nbin=32;               //nbin ::number of bin along x axis
double phi,length,Fp;
double theta; 
char c; 
double ***bin;
int tmax;
int fn,ln;
	FILE *fpn = fopen("nline.dat","r");
	fscanf(fpn,"%d\t%d\t%d\n",&fn,&ln,&tmax);
 
size_t xlen=tmax+1;
size_t ylen=nbin+1;
size_t zlen=nbin+1;
size_t i1,j1,k1;
double times;
int tsteady = 150;
double pi = 4.0*atan(1.0);
double factor = pi/4.0; 
double bin_av[nbin][nbin];  


	double **x=(double **)malloc((tmax+1)*sizeof(double *));       //x(t,i)
	for(i=0;i<tmax+1;i++){
	x[i]=(double *)malloc((N+1)*sizeof(double));
	}

	
	double **y=(double **)malloc((tmax+1)*sizeof(double *));       //y(t,i)
	for(i=0;i<tmax+1;i++){
	y[i]=(double *)malloc((N+1)*sizeof(double));
	}

	FILE *fp1;
	fp1=fopen("dipole.xyz","r");

	for(t1=0;t1<tmax;t1++){
	fscanf(fp1,"%d\n%lf\t%lf\t%lf\t%lf\n",&N_read,&phi,&length,&Fp,&times);
		for(i=0;i<N;i++){
  		fscanf(fp1,"%c\t%lf\t%lf\t%d\t%lf\n",&c,&x[t1][i],&y[t1][i],&z,&theta);
		//printf("%f\t%f\n",x[t1][i],y[t1][i]);
	}}

double binwidth=length/nbin;

if((bin=alloc_data(xlen,ylen,zlen))==NULL){
		return EXIT_FAILURE;}

for(i1=0;i1<xlen;++i1){
		for(j1=0;j1<ylen;++j1){
			for(k1=0;k1<zlen;++k1){
				bin[i1][j1][k1]=0;
				}}}

FILE *fp2,*fp3;

fp2=fopen("local_density_map.dat","w");
fp3=fopen("local_density.dat","w");
 
	

	for(i=0;i<nbin;i++){
		for(j=0;j<nbin;j++){
	        bin_av[i][j]=0;
		}}


	for(t1=tsteady;t1<tmax;t1++){
		for(i=0;i<N;i++){
			xindex=(int)(x[t1][i]/binwidth);
			yindex=(int)(y[t1][i]/binwidth);//printf("%f\t%f\t%d\t%d\n",x[t1][i],y[t1][i],xindex,yindex);
			bin[t1][xindex][yindex]+=1;
			//printf("%d\n",bin[xindex][yindex]);
			}

		for(i=0;i<nbin;i++){
			for(j=0;j<nbin;j++){
				bin_av[i][j]+=bin[t1][i][j];
				//bin_sq[i][j]+=bin[t1][i][j]*bin[t1][i][j];
			}}
			//fprintf(fp2,"%d\t%d\n",t1,(bin[xindex][yindex])/1000);
			}
	
	for(t1=tsteady;t1<tmax;t1++){
			for(i=0;i<nbin;i++){
				for(j=0;j<nbin;j++){
					count+=bin[t1][i][j];
			}}}
			printf("count=%d\n",count);


	for(i=0;i<nbin;i++){
		for(j=0;j<nbin;j++){
			bin_av[i][j]=bin_av[i][j]/(double)(tmax-tsteady);
			//bin_sq[i][j]=bin_sq[i][j]/(double)(tmax-tsteady);
			//bin_delta[i][j]=sqrt(bin_sq[i][j]-(bin_av[i][j]*bin_av[i][j]));
			}}

	for(i=0;i<nbin;i++){
		for(j=0;j<nbin;j++){
		  fprintf(fp2,"%lf\t%lf\t%lf\n",(i+0.5)*binwidth,(j+0.5)*binwidth,(bin_av[i][j]*factor)/(binwidth*binwidth));
	  	  fprintf(fp3,"%lf\n",(bin_av[i][j]*factor)/(binwidth*binwidth));
		
		}

		fprintf(fp2,"\n");
		}




fclose(fp1);
fclose(fp2);
fclose(fp3);

}

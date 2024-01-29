#include<stdio.h>
#include<stdlib.h>
#include<math.h>


typedef struct {                // structure for particle
		double x,y;
		}particle_t;

int main(){
    int i,j,t,z,tmax;   // Maximum snapshots to analyse
    int file,line;
    FILE *fp0 = fopen("nline.dat","r");
    fscanf(fp0,"%d\t%d\t%d\n",&file,&line,&tmax);
    int  tsteady = 0;
    int N=2500;              // Number of particles
    particle_t *particle;
    particle = (particle_t *)malloc((N)*sizeof(particle_t));
    double theta;
    double boxsize,phi=0.55,Fp,times;
    double pi = 4.0*atan(1.0);
    boxsize = pow(((N*pi)/(4.0*phi)),0.5);
    double rho = N/(boxsize*boxsize);printf("rho = %lf\n",rho);
    char c;
    double rxij,ryij,rijsq,rij;
    double d_min = 0.0, d_max = boxsize/2.0;  // Maximum relative distance for binning
    double dr = 0.1;
    int nbin = (int)(boxsize / (2.0*dr));
    
    //int nbin = 24;                    // Number of bins
    //double dr =(d_max - d_min)/nbin;  // Binwidth
    //printf("Binwidth = %lf\n",dr);
    int bin_index;
    double rdf_bin[nbin+1];
  
    for(i = 0; i< nbin; i++){
		rdf_bin[i]=0.0;   // Initialising value
			}

    FILE *fp1 = fopen("dipole.xyz","r");   // Configuration for scanning
    FILE *fp2 = fopen("gr_tav.dat","w");

    for(t = 0; t < tmax; t++){ printf("t = %d\n",t);		// Time loop
        /* Scanning file */
	fscanf(fp1,"%d\n%lf\t%lf\t%lf\t%lf\n",&N,&phi,&boxsize,&Fp,&times);
	//fprintf(fp2,"%lf\n",times);
        for(i = 0; i< N; i++){
	  fscanf(fp1,"%c\t%lf\t%lf\t%d\t%lf\n",&c,&particle[i].x,&particle[i].y,&z,&theta);
                               }
	
        
        for(i = 0; i < N-1; i++){
            for(j = i+1; j < N ; j++){
			
			rxij=particle[j].x-particle[i].x; 
			ryij=particle[j].y-particle[i].y; 
                        /* Periodic boundary condition */
			if(rxij>(boxsize)*0.5){rxij-=boxsize;}
			if(rxij<=-(boxsize)*0.5){rxij+=boxsize;}
			if(ryij>(boxsize)*0.5){ryij-=boxsize;}
			if(ryij<=-(boxsize)*0.5){ryij+=boxsize;}

			rijsq=rxij*rxij+ryij*ryij;
			rij=pow(rijsq,0.5);   
                        /* Binning */        
                        bin_index = (int)(rij/dr); //printf("%lf\t%d\n",rij,bin_index);
                        if(bin_index<nbin){
                        rdf_bin[bin_index]+=2;     
                        
				}}}}
    

   double r_lower,r_upper,area;
   for(bin_index=0; bin_index< nbin; bin_index++){
                r_lower = (double)(bin_index)*dr;
                r_upper = r_lower + dr;
                area = pi*(r_upper*r_upper - r_lower*r_lower); // Area  of ring
		fprintf(fp2,"%lf\t%lf\n",(bin_index+0.5)*dr,rdf_bin[bin_index]/(area*rho*N*tmax));
                
   }
                       
  // printf("count = %ld\ntc = %d\n",count,tc);
		
        
}     
    


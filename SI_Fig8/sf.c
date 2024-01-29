#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAX_Nk_wave 20000 // for S(k)

#define SQR(x) ((x)*(x))
#define CUB(x) ((x)*(x)*(x))

int main()
{
  int file,line,tmax;
  FILE *fp0 = fopen("nline.dat","r");
  fscanf(fp0,"%d\t%d\t%d\n",&file,&line,&tmax);
  int t_config=tmax,t_steady=0,N_p=2500;
  int N = N_p;
  int SampleNumber;
  int snapshot;
  double boxsize,phi=0.55,Fp,times;
  double pi = 4.0*atan(1.0);
  boxsize = pow(((N*pi)/(4.0*phi)),0.5);
  double rho = N/(boxsize*boxsize);printf("rho = %lf\n",rho);

  double L = boxsize;
 
  double dr,dq;
  char c;
  int NumberOfrBins=49;
  int NumberOfqBins=100;
  double Sq[NumberOfqBins+1];
  double qi[NumberOfqBins+1];
  double qmax = 20;  
  int i,j;
  double dx,dy;
  double rij;
  FILE *fpxyz;
  FILE *fpoutput;
  double x[N_p+1],y[N_p+1];
  double theta;
  double sum_real,sum_complex,sq_mag; 
  printf("density %lf\n",rho);  
  dr = 0.5*L/NumberOfrBins;
  dq = (qmax-0.0)/NumberOfqBins;

 
 
  int nkbound=100;   
  int dkmultiplier; //increment dk multiplier 
  dkmultiplier = 1; //1
 
  double dkx,dky;
  double Lx,Ly;
  Lx=L;Ly=L;
  dkx = 2.0*M_PI/Lx*dkmultiplier;dky = 2.0*M_PI/Ly*dkmultiplier;

  int nk,nx,ny;
  typedef struct
  {
        double x;
        double y;
        
  } VECTOR;
  VECTOR Wavevector[MAX_Nk_wave];
  int Nk_wave;
  int z;

  nk = 0;
  for(nx=0;nx<=nkbound;nx++)
  for(ny=0;ny<=nkbound;ny++)
  
  {
   Wavevector[nk].x = nx*dkx;
   Wavevector[nk].y = ny*dky;
   nk++;
  }
  Nk_wave = nk;

  printf("number of k's = %d\n",Nk_wave);
 

 int itop,i1;
 double rxi,ryi;
 int change;
 itop = Nk_wave-1;
 do
 {
 change = 0;     
 for(i=0;i<itop;i++)
 {
  i1 = i+1;
  if(Wavevector[i].x*Wavevector[i].x + Wavevector[i].y*Wavevector[i].y  > Wavevector[i1].x*Wavevector[i1].x+Wavevector[i1].y*Wavevector[i1].y)
  {
   rxi =  Wavevector[i].x;
   ryi =  Wavevector[i].y;
   

   Wavevector[i].x = Wavevector[i1].x;
   Wavevector[i].y = Wavevector[i1].y;
   

   Wavevector[i1].x = rxi;
   Wavevector[i1].y = ryi;
   
   change = 1;   //true
  }
 }//end loop i
 itop--;
 }
 while(change==1 && itop > 0);  

  double kmax;
  int nkmax;
  nkmax = Nk_wave-1; printf("nkmax = %d\n",nkmax);
  kmax = sqrt(Wavevector[nkmax].x*Wavevector[nkmax].x+Wavevector[nkmax].y*Wavevector[nkmax].y);
  printf("maximum k = %lf\n",kmax);

  /*******************************************/ 

  double Sk[MAX_Nk_wave],Sk_cos[MAX_Nk_wave],Sk_sin[MAX_Nk_wave];
  VECTOR k_scale;

  SampleNumber = 0;
  for(nk=0;nk<Nk_wave;nk++)
   Sk[nk] = 0.0;

  SampleNumber = 0;
  
  fpxyz = fopen("dipole.xyz","r"); 
  fpoutput = fopen("sk.dat","w");

  for(snapshot=0;snapshot<t_config;snapshot++)
  {
   printf("snapshot %d\n",snapshot);
   fscanf(fpxyz,"%d\n%lf\t%lf\t%lf\t%lf\n",&N_p,&phi,&boxsize,&Fp,&times);
   for(line=0;line<N_p;line++) // N+2 lines
   {
     fscanf(fpxyz,"%c\t%lf\t%lf\t%d\t%lf\n",&c,&x[line],&y[line],&z,&theta);
   }
   if(snapshot >= t_steady)
   {
    SampleNumber++;
    for(nk=0;nk<Nk_wave;nk++)
    {
     k_scale.x = Wavevector[nk].x*L/L;k_scale.y = Wavevector[nk].y*L/L; 
     Sk_cos[nk] = 0.0;Sk_sin[nk] = 0.0;
     for(i=0;i<N_p;i++) 
     {
      Sk_cos[nk] += cos(k_scale.x*x[i]+k_scale.y*y[i]);
      Sk_sin[nk] += sin(k_scale.x*x[i]+k_scale.y*y[i]);
     }// end loop over particle i
     Sk_cos[nk] = Sk_cos[nk]*Sk_cos[nk];
     Sk_sin[nk] = Sk_sin[nk]*Sk_sin[nk];
     Sk[nk] += Sk_cos[nk]+Sk_sin[nk];
    } // end loop over nk
   } // end if after equilibration
  
  

 
  fprintf(fpoutput,"%lf\n",times);
  
  for(nk=0;nk<Nk_wave;nk++)
 	Sk[nk] /= (N_p);

  int kcount;
  double knorm,skmean;
  double kminus,kplus;
 
  kcount=0; knorm=0.0; skmean=0.0;
  for(nk=0;nk<Nk_wave;nk++)
  {
  kcount++;
  knorm += sqrt(Wavevector[nk].x*Wavevector[nk].x+Wavevector[nk].y*Wavevector[nk].y);
  skmean += Sk[nk];
  kplus = (Wavevector[nk+1].x*Wavevector[nk+1].x+Wavevector[nk+1].y*Wavevector[nk+1].y);
  kminus = (Wavevector[nk].x*Wavevector[nk].x+Wavevector[nk].y*Wavevector[nk].y);
  if( fabs(kplus-kminus) > 0.001 || nk+1 == Nk_wave )     // 0.001 clarify 
  {
   knorm /= kcount;  skmean /= kcount;
   fprintf(fpoutput,"%lf\t%lf\n",knorm,skmean);  //k,Sk
   kcount=0; knorm=0.0; skmean=0.0;
  }
 }
}//end loop over snapshot
  fclose(fpxyz);
  fclose(fpoutput);




 printf("*******************************************************\n");

 return 0;
}



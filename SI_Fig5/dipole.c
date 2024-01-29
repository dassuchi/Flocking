#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "param.c"

#define PI 3.14159265358979323846


typedef struct {    // structure for particle
    double x,y,theta;
} particle_t;

typedef struct {    // structure for interaction force
    double Fx,Fy;
} force_t;

typedef struct {    // structure for torque
    double Tz;
} torque_t;


double uniform(){   // uniform distribution between 0 and 1
    return rand()/(1.0*RAND_MAX);
}

/* ============================================================= */
/*  THIS PART OF THE PROGRAM GENERATES A GAUSSIAN RANDOM NUMBER  */
/* ============================================================= */

#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)

double ran2(long *idum){

    static int inext,inextp;
    static long ma[56];
    static int iff=0;
    long mj,mk;
    int i,ii,k;

    if (*idum < 0 || iff == 0) {

        iff=1;
        mj=MSEED-(*idum < 0 ? -*idum : *idum);
        mj %= MBIG;
        ma[55]=mj;
        mk=1;

        for (i=1;i<=54;i++){
            ii=(21*i) % 55;
            ma[ii]=mk;
            mk=mj-mk;
            if (mk < MZ) mk += MBIG;
            mj=ma[ii];
        }

        for (k=1;k<=4;k++)
            for (i=1;i<=55;i++) {
                ma[i] -= ma[1+(i+30) % 55];
                if (ma[i] < MZ) ma[i] += MBIG;
            }

        inext=0;
        inextp=31;
        *idum=1;

    }

    if (++inext == 56) inext=1;
    if (++inextp == 56) inextp=1;
    mj=ma[inext]-ma[inextp];
    if (mj < MZ) mj += MBIG;
    ma[inext]=mj;
    return mj*FAC;
}

#undef MBIG
#undef MSEED
#undef MZ
#undef FAC

/* ============================================================= */
/*  (C) Copr. 1986-92 Numerical Recipes Software ,o>29'?0>!+W.   */
/* ============================================================= */

double gasdev(long *idum) {

    static int iset=0;
    static double gset;
    double fac,r,v1,v2;
    double ran2();

    if  (iset == 0) {

        do {
            v1=2.0*ran2(idum)-1.0;
            v2=2.0*ran2(idum)-1.0;
            r=v1*v1+v2*v2;
        } while (r >= 1.0);

        fac=sqrt(-2.0*log(r)/r);
        gset=v1*fac;
        iset=1;
        return v2*fac;

    } else {

        iset=0;
        return gset;

    }

}



/* ============================================================= */
/*             THIS PART INITIALISES THE CONFIGURATION           */
/* ============================================================= */

 void initialpos(particle_t *particle, int N, int nx, double sep){

    int ip, ix = 0;
    double y = 0.0;

    FILE *fp= fopen("init_config.xyz","w");

    fprintf(fp," %d\n 0\n",N);

    for(ip=0;ip<N;ip++){

        particle[ip].x = ix*sep;
        particle[ip].y = y;
        particle[ip].theta = PI*(2*uniform()-1);

        ix++;

        if(ix>=nx){
            ix=0;
            y += sep;
        }

        fprintf(fp,"G\t%lf\t%lf\t%lf\n",particle[ip].x,particle[ip].y,particle[ip].theta);
    }
    fclose(fp);
}


/* ============================================================= */
/* THE ROUTINE IS CALLED EVERY TIMESTEP BEFORE THE FORCE ROUTINE */
/* ============================================================= */

void interparticle_int(particle_t *particle, force_t *f,
                       torque_t *T, int N, double boxsize_x,
                       double boxsize_y, double dipole_head,
                       double dipole_tail) {

    double rmag, xij, yij;
    double fmag, fxij, fyij;
    double factor;
    double tmag;

    int i,j;

    double epsilon = 1.0;
    double lambda = 40.0;
    double dip_length = 3.0/16.0;
    double torque_deno;
    
    for(i=0;i<N;i++){
        f[i].Fx=0.0;
        f[i].Fy=0.0;
        T[i].Tz=0.0;
        }



    for(i = 0; i < N; i++){
        for(j = i+1; j < N ; j++){          //avoids double counting of same particle

            xij = particle[j].x - particle[i].x;
            yij = particle[j].y - particle[i].y;

            if(xij >  (boxsize_x)*0.5) {xij -=(boxsize_x);}
            if(xij <=-(boxsize_x)*0.5) {xij +=(boxsize_x);}
            if(yij >  (boxsize_y)*0.5) {yij -=(boxsize_y);}
            if(yij <=-(boxsize_y)*0.5) {yij +=(boxsize_y);}   //PBC along x and y;

            // Calculate distance between particles
            rmag=pow(xij*xij+yij*yij,0.5);
 
            // Calculate common factor (common to force and torque)
            factor = 3*exp(-rmag/lambda)/(4*PI*epsilon*pow(rmag,5));

            // Calculation of translational force
            fmag = factor*(dipole_head+dipole_tail)*(dipole_head+dipole_tail);
            //printf("fmag = %lf\n",fmag);

            fxij=xij*fmag; // force on j due to i: x component
            fyij=yij*fmag; // force on j due to i: y component

            // add force to j-th particle
            f[j].Fx += fxij;
            f[j].Fy += fyij;

            // add oppositely directed force to i-th particle
            f[i].Fx -= fxij;
            f[i].Fy -= fyij;

            // Calculation of torque
            tmag = factor*dip_length*((dipole_head*dipole_head)-
                                      (dipole_tail*dipole_tail));
            //printf("tmag = %lf\n",tmag);

            // torque on j-th particle due to i-th particle
            T[j].Tz+= tmag*(yij*cos(particle[j].theta)-
                            xij*sin(particle[j].theta));

            // torque on i-th particle due to j-th particle
            T[i].Tz-= tmag*(yij*cos(particle[i].theta)-
                            xij*sin(particle[i].theta));

        }
    }
}

int main(int argc, char **argv){

    /* ============================================================= */
    /*                  DECLARATION OF VARIABLES                     */
    /* ============================================================= */

    // particle properties
    double sigma = 1.0;
    double dipole_head = -53.39;
    double dipole_tail = 112.39;
    double v_0;   
    double zeta_r = 3.0;
    double zeta_t = 9.0;

    // number and particles and seed for random noise
    int N=1024;
    long seed1;
    long seed2;
    long seed3;


    // box and particle positioning properties
    double phi;
    parameter(&phi, &v_0, &seed1, &seed2, &seed3);
    double boxsize_x = pow(((N*PI*sigma*sigma)/(4.0*phi)),0.5);
    double boxsize_y = pow(((N*PI*sigma*sigma)/(4.0*phi)),0.5);
    int nx = 32;                                                  // number of particles initially placed along x-axis
    double separation=((double)boxsize_x)/nx;

    // simulation variables
    double times;
    double h1 = 0.00001;
    int countloop;

    particle_t *particle;
    force_t *f;
    torque_t *T;
    double vx,vy;

    
    int m;
    FILE *fp2;
    char fn[60];


    double D=1.0/9.0,Dr=1.0;      


    /* ============================================================= */
    /*                     END OF DECLARATION                        */
    /* ============================================================= */

    sprintf(fn,"dipole.xyz");
    fp2 = fopen(fn,"w");


    /* ============================================================= */
    /*                    ALLOCATION OF MEMORY                       */
    /* ============================================================= */
    particle=(particle_t *)malloc((N)*sizeof(particle_t));
    f=(force_t *)malloc((N)*sizeof(force_t));
    T=(torque_t *)malloc((N)*sizeof(torque_t));

    /* ============================================================= */
    /*                SETUP INITIAL CONFIGURATION                    */
    /* ============================================================= */
    initialpos(particle, N, nx, separation);

    //vmd file
    fprintf(fp2," %d\n %lf\t %lf\t %lf\t %lf\n",N,phi,boxsize_x,v_0,times);


    /* ============================================================= */
    /*                          SIMULATION                           */
    /* ============================================================= */

    for(countloop=0;countloop<6000000;countloop++){

        times = countloop * h1;

        //if(countloop%1000==0){
        //    printf("t=%f\n",times);
        //}

        //calculate interparticle interaction
        interparticle_int(particle, f, T, N, boxsize_x, boxsize_y, dipole_head,
                          dipole_tail);

        for(m=0;m<N;m++){

            // Generate gaussian noise
            double a=gasdev(&seed1);
            double b=gasdev(&seed2);
            double c=gasdev(&seed3);

            //Calculate velocity of the particle
            vx=(f[m].Fx)/zeta_t+v_0*cos(particle[m].theta)+sqrt(2.0*D/h1)*a;
            vy=(f[m].Fy)/zeta_t+v_0*sin(particle[m].theta)+sqrt(2.0*D/h1)*b;

            //Update position of the particle
            particle[m].x+=h1*vx;
            particle[m].y+=h1*vy;

            //Update direction of the particle
            particle[m].theta+=((T[m].Tz*sqrt(h1)/zeta_r)+c*(pow((2.0*Dr),0.5)))*sqrt(h1);

            //Put boundary conditions
            if(particle[m].x<       0.0) {particle[m].x+=boxsize_x;}
            if(particle[m].x>=boxsize_x) {particle[m].x-=boxsize_x;}
            if(particle[m].y<       0.0) {particle[m].y+=boxsize_y;}
            if(particle[m].y>=boxsize_y) {particle[m].y-=boxsize_y;}

            //Print every 10000th iteration
            if(countloop%10000==0){

                fprintf(fp2,"G\t%lf\t%lf\t0\t%lf\n",particle[m].x,
                                    particle[m].y,particle[m].theta);

                fflush(fp2);

                if(m==N-1){
		  fprintf(fp2," %d\n %lf\t %lf\t %lf\t %lf\n",N,phi,boxsize_x,v_0,times);
                    fflush(fp2);
                }
            }
        }
    }

    fclose(fp2);
    free(particle);
    free(f);
    free(T);
}

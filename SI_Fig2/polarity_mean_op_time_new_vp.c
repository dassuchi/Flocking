#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
    int t, tmax=3358;
    double theta;
    int i;
    //FILE *fp0 = fopen("time_particle.dat","r");

    int tsteady = 0;
    int times, N;

    FILE *fp1 = fopen("particle_vel_orientation.dat","r");
    FILE *fp2 = fopen("polarity_op_vp.dat","w");

    double px_sum=0.0, py_sum=0.0, p_mag, p_mag_m=0.0, p_mag_sq_m=0.0, p_mag_stddev,p_av=0.0;
 
    for(t = 0; t < tmax; t++){ 
        fscanf(fp1,"%d\n%d\n",&times,&N);
        px_sum = 0.0;
        py_sum = 0.0;

        for(i = 0; i < N; i++){
            fscanf(fp1,"%lf\n",&theta);
            //printf("theta = %lf\n",theta);
            px_sum += cos(theta);
            py_sum += sin(theta);
        }

        p_mag = sqrt(px_sum*px_sum + py_sum*py_sum) / N;
        p_mag_m += p_mag;      // Accumulating sum of magnitude for mean
        p_mag_sq_m += p_mag*p_mag;  // Accumulating sum of squared magnitude for variance

       /* if(t > 0) {
            p_mag_stddev = sqrt((p_mag_sq_m/t) - (p_mag_m/t)*(p_mag_m/t));
        } else {
            p_mag_stddev = 0.0;
        }*/

        //fprintf(fp2, "%d\t%lf\t%lf\n", times, p_mag, p_mag_stddev); // Adding standard deviation in the third column
        fprintf(fp2, "%d\t%lf\n", times, p_mag);
        p_av+=p_mag;
    }
     p_av/=tmax;
    printf("p_av_sq = %lf\n",p_av*p_av);
    
    //fclose(fp0);
    fclose(fp1);
    fclose(fp2);
    return 0;
}


#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
    int t, tmax=93;
    double x, y, theta, add;
    int i;
    //FILE *fp0 = fopen("time_particle.dat","r");

    int tsteady = 0;
    int times, N, pid;
    int t_init = 52;

    FILE *fp1 = fopen("particle_orientation_PD.dat","r");
    FILE *fp2 = fopen("polarity_op_PD_or.dat","w");

    double px_sum=0.0, py_sum=0.0, p_mag, p_mag_m=0.0, p_mag_sq_m=0.0, p_mag_stddev;
 
    for(t = 0; t < tmax; t++){ 
        fscanf(fp1,"%d\n%d\n",&times,&N);
        px_sum = 0.0;
        py_sum = 0.0;

        for(i = 0; i < N; i++){
            fscanf(fp1,"%d\t%lf\t%lf\t%lf\n",&pid,&x,&y,&theta);
            printf("theta = %lf\n",theta);
            px_sum += cos(theta);
            py_sum += sin(theta);
        }

        p_mag = sqrt(px_sum*px_sum + py_sum*py_sum) / N;
        p_mag_m += p_mag;      // Accumulating sum of magnitude for mean
        p_mag_sq_m += p_mag*p_mag;  // Accumulating sum of squared magnitude for variance

        /*if(t > 0) {
            p_mag_stddev = sqrt((p_mag_sq_m/t) - (p_mag_m/t)*(p_mag_m/t));
        } else {
            p_mag_stddev = 0.0;
        }*/

       // fprintf(fp2, "%d\t%lf\t%lf\n", times, p_mag, p_mag_stddev); // Adding standard deviation in the third column
    	fprintf(fp2, "%d\t%lf\n", times+t_init, p_mag);
    }

    //fclose(fp0);
    fclose(fp1);
    fclose(fp2);
    return 0;
}


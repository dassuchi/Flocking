#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
    int t, tmax=93;
    double x, y, theta, add;
    int i,id;
    int tsteady = 0;
    int times1,times2,times3,N1,N2,N3;
    double vx,vy,v;

    FILE *fp1 = fopen("output_config_PD.dat","r");
    FILE *fp2 = fopen("vel_oc_PD.dat","r");
    FILE *fp3 = fopen("id_oc_PD.dat","r");
    FILE *fp4 = fopen("particle_orientation_PD.dat","w");
    FILE *fp5 = fopen("particle_vel_orientation_PD.dat","w");

  
    for(t = 0; t < tmax; t++){ 
        fscanf(fp1,"%d\n%d\n",&times1,&N1); printf("%d\t%d\n",times1,N1);
        fscanf(fp2,"%d\n%d\n",&times2,&N2);
        //if(N2==N1){fprintf(fp4,"%d\n%d\n",times1,N1);}
        //if(N2==N1){fprintf(fp5,"%d\n%d\n",times2,N2);}
        fprintf(fp4,"%d\n%d\n",times1,N1);
        fprintf(fp5,"%d\n%d\n",times2,N2);
        fscanf(fp3,"%d\n%d\n",&times3,&N3);
        //printf("%d\t%d\t%d\t%d\n",times1,N1,times2,N2);
        
        //if(N3!=N1){printf("N3 N1 not equal\n");}
        
        if(N2!=N1){printf("t = %d\t N2 N1 not equal\n",times1);}
        
        for(i = 0; i < N1; i++){
            fscanf(fp3,"%d\n",&id);
            //if(N2==N1){fprintf(fp4,"%d\t",id);}
            fprintf(fp4,"%d\t",id);
            fscanf(fp1,"%lf\t%lf\t%lf\t%lf\n",&x,&y,&theta,&add);
            //if(N2==N1){fprintf(fp4,"%lf\t%lf\t%lf\n",x,y,theta);}
            fprintf(fp4,"%lf\t%lf\t%lf\n",x,y,theta);
        }
  
        
        
         for(i = 0; i < N2; i++){
            fscanf(fp2,"%lf\t%lf\n",&vx,&vy);
            v = atan2(vy,vx);
            //if(N2==N1){fprintf(fp5,"%lf\n",v);}
            fprintf(fp5,"%lf\n",v);
        }
        
    }

    fclose(fp1);
    //fclose(fp2);
    //fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    return 0;
}


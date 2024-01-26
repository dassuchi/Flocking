#!/bin/bash

# Create an array to store the generated values
declare -a seed1_arr
declare -a seed2_arr
declare -a seed3_arr

# Generate 20 different values for seed1, seed2, and seed3
for ((i=1; i<=20; i++))
do
    seed1_arr[$i]=$RANDOM
    seed2_arr[$i]=$RANDOM
    seed3_arr[$i]=$RANDOM
done

# Generate the C files with the updated values
for ((i=1; i<=20; i++))
do
    # Create a new C file for each set of values
    filename="param${i}.c"
    echo "void parameter(double *phi, double *v_0, long *seed1, long *seed2, long *seed3)" >> $filename
    echo "{" >> $filename
    echo "*phi=0.20;" >> $filename
    echo "*v_0=11;" >> $filename
    echo "*seed1=${seed1_arr[$i]};" >> $filename
    echo "*seed2=${seed2_arr[$i]};" >> $filename
    echo "*seed3=${seed3_arr[$i]};" >> $filename
    echo "}" >> $filename
done

echo "C files with 20 different sets of values for seed1, seed2, and seed3 have been generated."


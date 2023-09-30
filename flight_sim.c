#include <stdio.h>
#include <math.h>

#include "grader.h"

#define ARR_DIM 10      // the arrays will contain the last 10 records

int counter;
int arr_index;
int i;
float acc;
float arr_acc_x[ARR_DIM];
float arr_acc_y[ARR_DIM];
float arr_acc_z[ARR_DIM];
float arr_gyro_x[ARR_DIM];
float arr_gyro_y[ARR_DIM];
float arr_gyro_z[ARR_DIM];
float arr_bar[ARR_DIM];

float avg_value(float* arr, int arr_dim);
float max_variation(float* arr, int arr_dim);


void init() {
    printf("Init!\n");
    counter = 0;
    arr_index = 0;
    acc = 0;
    for (i = 0; i < ARR_DIM; i++){
        arr_acc_x[i] = 0;
        arr_acc_y[i] = 0;
        arr_acc_z[i] = 0;
        arr_gyro_x[i] = 0;
        arr_gyro_y[i] = 0;
        arr_gyro_z[i] = 0;
        arr_bar[i] = 0;
    }
}


void update(float acc_x, float acc_y, float acc_z, float gyro_x, float gyro_y,
            float gyro_z, float baro){

    printf("Update: %f,%f,%f,%f,%f,%f,%f\n", acc_x, acc_y, acc_z, gyro_x,
           gyro_y, gyro_z, baro);

    // Updating Arrays
    arr_index = counter % ARR_DIM;  // form 0 to ARR_DIM - 1
    arr_acc_x[arr_index] = acc_x;
    arr_acc_y[arr_index] = acc_y;
    arr_acc_z[arr_index] = acc_z;
    arr_gyro_x[arr_index] = gyro_x;
    arr_gyro_y[arr_index] = gyro_y;
    arr_gyro_z[arr_index] = gyro_z;
    arr_bar[arr_index] = baro;

    // norm of the acceleration vector
    acc = sqrt(powf(avg_value(arr_acc_x, ARR_DIM), 2) + 
               powf(avg_value(arr_acc_y, ARR_DIM), 2) + 
               powf(avg_value(arr_acc_z, ARR_DIM), 2));

    if (counter >= ARR_DIM - 1){    // Starts with at least 10 record read
        if (acc > 25){
            liftoff();
        }
        else if (acc < 8){
            apogee();
        }
        else if (acc < 10 && acc > 9 && max_variation(arr_bar, ARR_DIM) < 100
                                && max_variation(arr_acc_x, ARR_DIM) < 0.2
                                && max_variation(arr_acc_y, ARR_DIM) < 0.2
                                && max_variation(arr_acc_z, ARR_DIM) < 0.2
                                && max_variation(arr_gyro_x, ARR_DIM) < 0.1
                                && max_variation(arr_gyro_x, ARR_DIM) < 0.1
                                && max_variation(arr_gyro_x, ARR_DIM) < 0.1){
            landed();    
        }
    }

    counter++;
}


float avg_value(float* arr, int arr_dim){
    float sum;
    float avg;
    int i;

    sum = 0;
    avg = 0;

    for(i = 0; i < arr_dim; i++){
        sum += arr[i];
    }
    avg = sum / arr_dim;

    return avg;
}


float max_variation(float* arr, int arr_dim){
    float max_value = 0;
    float min_value = 0;
    float max_var;
    int i;

    if (arr_dim > 0){
        max_value = arr[0];
        min_value = arr[0];
        
        for(i = 1; i < arr_dim; i++){
            if (arr[i] > max_value){
                max_value = arr[i];
            }
            if (arr[i] < min_value){
                min_value = arr[i];
            }
        }
    }

    max_var = abs(max_value - min_value);

    return max_var;
}

#ifndef KERNEL_H
#define KERNEL_H

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>


__global__ void kernel(float3* pos, unsigned int width, unsigned int height, float thetaStep, float phiStep);
__global__ void kernel2(unsigned int* i, unsigned int width, unsigned int height, float dx, float dy);
__global__ void kernel3(float2* tex, unsigned int width, unsigned int height);
//__global__ void kernel4(float3* pos, unsigned int width, unsigned int height);


void kernel_Pos(float3* pos, unsigned int width, unsigned int height, float thetaFac, float phiFac);



void kernel_Tex(float2* tex, unsigned int width, unsigned int height, float dx, float dy);



void kernel_Index(unsigned int* i, unsigned int width, unsigned int height);


//void kernel_Asteroid(float3* pos, unsigned int width, unsigned int height);


#endif
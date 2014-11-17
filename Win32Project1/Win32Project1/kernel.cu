#pragma once
#include "kernel.h"
#include <math.h>
#include <glm\glm.hpp>


#ifndef PI
#define PI 3.14159265358979323846f
#endif


using namespace std;

__global__ void kernel(float3* pos, unsigned int width, unsigned int height, float thetaStep, float phiStep) {
	//NOTE: threadIdx.x is row and blockIdx.x is col

	float x, y, z;
	//get index and current offset into memeory
	int index = width * threadIdx.y + threadIdx.x;

	// bottom row of mesh grid
	if(threadIdx.y == 0) {
		x = 0.0f;
		y = -1.0f;
		z = 0.0f;
	}
	// top row of mesh grid
	else if(threadIdx.y == height-1) {
		x = 0.0f;
		y = 1.0f;
		z = 0.0f;
	}else {
		//used to calculate postion
		float theta, phi;
		theta = (float)(PI - (threadIdx.y * thetaStep));
		phi = (float)(threadIdx.x * phiStep);

		//used to calculate spherical position
		x = 1.0f * sinf(theta) * cosf(phi);
		y = 1.0f * cosf(theta);
		z = 1.0f * sinf(theta) * sinf(phi);
	}
	
	//use index to get correct place in array
	pos[index] = make_float3(x, y, z);
}





__global__ void kernel2(unsigned int* i, unsigned int width, unsigned int height) {
	unsigned int index = width * threadIdx.y + threadIdx.x;

	unsigned int offset = index * 6; // every thread/index will compute 6 vertices/2 triangles

	i[offset] = index;
	i[offset+1] = index+1;
	i[offset+2] = index+width;

	i[offset+3] = index+1;
	i[offset+4] = index+1+width;
	i[offset+5] = index+width;
}



__global__ void kernel3(float2* tex, unsigned int width, unsigned int height, float dx, float dy) {
	int index = width * threadIdx.y + threadIdx.x;

	float u = dx * threadIdx.x; // texture coordinate for X left right
	float v = 1.0f - (dy * threadIdx.y); /* DevIL requires this to be flipped? */
	//float v = dy * threadIdx.y;

	tex[index] = make_float2(u, v);

}


//__global__ void kernel4(float3* pos, unsigned int width, unsigned int height) {
//	float x, y, z;
//	//get index and current offset into memeory
//	int index = width * threadIdx.y + threadIdx.x;
//
//	float3 position = pos[index];
//	glm::vec3 temp = glm::normalize(glm::vec3(pos->x, pos->y, pos->z));
//	temp = glm::normalize(temp);
//	/* Look in monte carlo sim example for using curand on GPU */
//
//	//use index to get correct place in array
//	pos[index] = make_float3(x, y, z);
//}



void kernel_Pos(float3* pos, unsigned int width, unsigned int height, float thetaFac, float phiFac) {
	dim3 block, grid; /* block describes threads within a block; grid describes how many blocks overall */

	kernel<<< dim3(1, 1, 1), dim3(width, height, 1)>>>(pos, width, height, thetaFac, phiFac);
}

void kernel_Tex(float2* tex, unsigned int width, unsigned int height, float dx, float dy) {
	dim3 block, grid; /* block describes threads within a block; grid describes how many blocks overall */

	kernel3<<< dim3(1, 1, 1), dim3(width, height, 1)>>>(tex, width, height, dx, dy);
}



void kernel_Index(unsigned int* i, unsigned int width, unsigned int height) {

	kernel2<<<dim3(1, 1, 1), dim3((width-1), (height-1), 1) >>>(i, width, height); /* width-1 */
}


//void kernel_Asteroid(float3* pos, unsigned int width, unsigned int height) {
//	kernel4<<< dim3(1, 1, 1), dim3(width, height, 1)>>>(pos, width, height);
//}
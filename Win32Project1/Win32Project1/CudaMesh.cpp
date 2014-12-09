#pragma once

#include "CudaMesh.h"
#include "kernel.h"

using namespace glm;

CudaMesh::CudaMesh(std::string name,  int width_, unsigned int height_) : Mesh(name), width(width_), height(height_){
	resPosition = nullptr;
	resNormal = nullptr;
	resTextureCoord = nullptr;
	resIndex = nullptr;
}
CudaMesh::~CudaMesh() {

}

void CudaMesh::MakeSphere() {
	/* Initialize memory on GPU to hold data */



	/* Position  ======================================================================================= */
	glGenBuffers(1, &vboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);

	/* Initialize size of buffer */
	size_t bytesPos = sizeof(vec3) * width * height;
	glBufferData(GL_ARRAY_BUFFER, bytesPos, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Register buffer object with CUDA */
	cudaGraphicsGLRegisterBuffer(&resPosition, vboPosition, cudaGraphicsMapFlagsWriteDiscard);

	/* Get pointer to data to pass to CUDA kernal and launch kernel to compute data for mesh */
	float3* posPtr;
	cudaGraphicsMapResources(1, &resPosition, 0);
	size_t num_bytes;
	cudaGraphicsResourceGetMappedPointer((void **)&posPtr, &num_bytes, resPosition);

	float thetaFac = (M_PI) / (height-1);
	float phiFac = (2.0f * M_PI) / (width-1);

	
	/* Position  ====================================================================================== */

	/* Normal   ======================================================================================= */
	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);

	/* Initialize size of buffer */
	size_t bytesNormal = sizeof(vec3) * width * height;
	glBufferData(GL_ARRAY_BUFFER, bytesNormal, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Register buffer object with CUDA */
	cudaGraphicsGLRegisterBuffer(&resNormal, vboNormal, cudaGraphicsMapFlagsWriteDiscard);

	/* Get pointer to data to pass to CUDA kernal and launch kernel to compute data for mesh */
	float3* normalPtr;
	cudaGraphicsMapResources(1, &resNormal, 0);
	size_t num_bytes2;
	cudaGraphicsResourceGetMappedPointer((void **)&normalPtr, &num_bytes2, resNormal);



	kernel_Pos_Normal(posPtr, normalPtr, width, height, thetaFac, phiFac);
	cudaDeviceSynchronize();
	cudaGraphicsUnmapResources(1, &resPosition, 0);
	cudaGraphicsUnmapResources(1, &resNormal, 0);
	/* Normal    ====================================================================================== */




	/* Texture Coordinates ============================================================================ */
	glGenBuffers(1, &vboTextureCoord);
	glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoord);

	/* Initialize size of buffer */
	size_t bytesTex = sizeof(vec2) * width * height;
	glBufferData(GL_ARRAY_BUFFER, bytesTex, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Register buffer object with CUDA */
	cudaGraphicsGLRegisterBuffer(&resTextureCoord, vboTextureCoord, cudaGraphicsMapFlagsWriteDiscard);

	/* Get pointer to data to pass to CUDA kernal and launch kernel to compute data for mesh */
	float2* texPtr;
	cudaGraphicsMapResources(1, &resTextureCoord, 0);
	num_bytes;
	cudaGraphicsResourceGetMappedPointer((void **)&texPtr, &num_bytes, resTextureCoord);

	float dx = 1.0f / (width-1);
	float dy = 1.0f / (height-1);

	kernel_Tex(texPtr, width, height, dx, dy);
	cudaDeviceSynchronize();
	cudaGraphicsUnmapResources(1, &resTextureCoord, 0); 
	/* Texture Coordinates ============================================================================ */



	/* Indices ======================================================================================== */
	//numElements = (width-1) * (height-1) * 6;
	numElements = (width) * (height-1) * 6;

	glGenBuffers(1, &vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);

	size_t bytesIndices = ( (width) * (height-1) * 6 ) * sizeof(GLuint); /* width-1 */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesIndices, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    //Unbind array element buffer

	cudaGraphicsGLRegisterBuffer(&resIndex, vboIndex, cudaGraphicsMapFlagsWriteDiscard);
	
	unsigned int* iptr;
	cudaGraphicsMapResources(1, &resIndex, 0);
	cudaGraphicsResourceGetMappedPointer((void **)&iptr, &num_bytes, resIndex);

	kernel_Index(iptr, width, height);
	cudaDeviceSynchronize();
	cudaGraphicsUnmapResources(1, &resIndex, 0);
	/* Indices ======================================================================================== */



	/* VAO ============================================================================================ */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoord);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
	
	glBindVertexArray(0);
	/* VAO ============================================================================================ */
}
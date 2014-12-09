#ifndef CUDAMESH_H
#define CUDAMESH_H


#include "Precompiled.h"
#include "Mesh.h"




class CudaMesh : public Mesh {
public:
	CudaMesh(std::string name,  int width_, unsigned int height_);
	~CudaMesh();



	void MakeSphere();


private:

	unsigned int width, height;

	/* Cuda Graphics Resource for mapping vbo and cuda in GPU memory */
	struct cudaGraphicsResource* resPosition;
	struct cudaGraphicsResource* resNormal;
	struct cudaGraphicsResource* resTextureCoord;
	struct cudaGraphicsResource* resIndex;



};


#endif
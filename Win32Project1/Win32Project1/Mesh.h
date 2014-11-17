#ifndef MESH_H
#define MESH_H

#include "Precompiled.h"

#include "Type.h"






class Vertex {
public:
	Vertex(const glm::vec3& p, const glm::vec3& n, const glm::vec2& t)
		: position(p), normal(n), texCoord(t) {}

public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

};


class Mesh {
public: 
	Mesh(const std::string name);
	Mesh(const std::string file, int type, unsigned int width, unsigned int height);
	~Mesh();


	void SetBuffers(); /* Activates this mesh's vao and vbo's */
	void ClearBuffers(); /* Deactivates this mesh's vao and vbo's */

	unsigned int GetIndexCount();
	const std::string& GetName()const;
	
private:

	//void CreateAsteroidSphere();



	/* Private helper functions for loading an .obj file for mesh. */
	bool Initialize();
	void InitializeBuffers(const std::vector<Vertex>& vertices, const std::vector<GLuint>& faces);
	void trimString(std::string & str);


	/* CUDA methods */
	void Sphere();


private:

	// File for mesh
	std::string fileName;

	unsigned int width, height;

	/* Vertex Array Object */
	GLuint vao;	


	///* Number of Inidices for mesh */
	GLsizei numElements;


	/* Attributes: Used for storing data read from file */
	std::vector<glm::vec3> positions; 
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<int> indices;



	/* Vertex Buffer Objects */
	GLuint vboPosition;				// Stores Vertex Positions 
	GLuint vboNormal;				// Stores Vertex Normals
	GLuint vboTextureCoord;			// Stores Vertex Tex Coords
	GLuint vboIndex;				// Stores Indices for DrawElements



	/* Cuda Graphics Resource for mapping vbo and cuda in GPU memory */
	
	struct cudaGraphicsResource* resPosition;
	struct cudaGraphicsResource* resNormal;
	struct cudaGraphicsResource* resTextureCoord;
	struct cudaGraphicsResource* resIndex;
	


	bool cuda; /* If we are using CUDA for GPGPU acceleration for computation of mesh properties */



	/* Can also have bool for using compute shader */



};

#endif
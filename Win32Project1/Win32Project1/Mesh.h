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
	Mesh(const std::string name_);
	~Mesh();


	// Base class
	void SetBuffers(); /* Activates this mesh's vao and vbo's */
	void ClearBuffers(); /* Deactivates this mesh's vao and vbo's */



	// Base class
	const unsigned int GetIndexCount()const;
	const std::string& GetName()const;


protected:

	// File for mesh
	std::string name;

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

};

#endif
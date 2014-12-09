#include "NormalMesh.h"


NormalMesh::NormalMesh(std::string name) : Mesh(name) {
	this->file.resize(0);
}

NormalMesh::NormalMesh(std::string name, std::string file) : Mesh(name) {
	this->file = file;
}

NormalMesh::~NormalMesh() {


}


void NormalMesh::MakeQuad() {

	const float positionData[] = {
		// Front
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,

	   -1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	   -1.0f,  1.0f, 0.0f,
	};

	const float textureData[] = {
		//front
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
	};

	const unsigned int indices[] = {
		//front
		0, 2, 1,
		3, 5, 4
	};

	// Generate buffers
	glGenBuffers(1, &vboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboTextureCoord);
	glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoord);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);

	glBindVertexArray(0);

	this->numElements = 6;

}

void NormalMesh::MakeBox() {

	const float positionData[] = {
		// Front
       -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
       -0.5f,  0.5f, 0.5f,
	   //back
	   -0.5f, -0.5f, -0.5f,
	    0.5f, -0.5f, -0.5f,
	    0.5f,  0.5f, -0.5f,
	   -0.5f,  0.5f, -0.5f,
	   //left
	   -0.5f, -0.5f, -0.5f,
	   -0.5f, -0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f, -0.5f,
	   //right
	   0.5f, -0.5f,  0.5f,
	   0.5f, -0.5f, -0.5f,
	   0.5f,  0.5f, -0.5f,
	   0.5f,  0.5f,  0.5f,
	   //top
	   -0.5f, 0.5f,  0.5f,
	    0.5f, 0.5f,  0.5f,
	    0.5f, 0.5f, -0.5f,
	   -0.5f, 0.5f, -0.5f,
	   //bottom
       -0.5f, -0.5f, 0.5f, 
        0.5f, -0.5f, 0.5f,
	    0.5f, -0.5f, -0.5f,
	   -0.5f, -0.5f, -0.5f,
	};

	const float normalData[] = {
		//front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		//back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		//left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		//right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		//top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		//bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
	};

	const float textureData[] = {
		//front
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//back
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		//left
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//right
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//top
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//bottom
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	const unsigned int indices[] = {
		//front
		0, 2, 1,
		0, 3, 2,
		//back
		4, 5, 6,
		4, 6, 7,
		//left
		8, 10, 9,
		8, 11, 10,
		//right
		12, 14, 13,
		12, 15, 14,
		//top
		16, 18, 17,
		16, 19, 18,
		//bottom
		20, 21, 22,
		20, 22, 23,
	};

	// Generate buffers
	glGenBuffers(1, &vboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalData), normalData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboTextureCoord);
	glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureData), textureData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

	this->numElements = 36;

}
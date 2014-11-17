#pragma once

#include "Mesh.h"
#include "kernel.h"


#define M_PI 3.14159265358979323846f
#define M_PI_2 1.57079632679489661923f
#define M_PI_4 0.785398163397448309616f



using namespace std;
using namespace glm;



Mesh::Mesh(const std::string name) {
	this->fileName = name;

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

	// I don't used these for cube mesh 
	cuda = false;
	width = height = 0;

}

Mesh::Mesh(const std::string file, int type, unsigned int width, unsigned int height) {

	this->fileName = file; /* Store name of mesh */
	this->height = height;
	this->width = width;

	switch(type) {
		case 1:
			/* Sphere used for planets, sun, stars */
			Sphere();
			break;
		case 2:
			/* Used for Asteroids */
			Sphere();
			// Asteroid function to modify sphere
			break;
		case 3:
			break;
		default:
			/* Error */
			break;
	}
	
}


Mesh::~Mesh() {
	glDeleteBuffers(1, &vboPosition);
	glDeleteBuffers(1, &vboNormal);
	glDeleteBuffers(1, &vboTextureCoord);
	glDeleteBuffers(1, &vboIndex);
}
const string& Mesh::GetName()const {
	return this->fileName;
}

void Mesh::SetBuffers() {
	glBindVertexArray(vao);
}

void Mesh::ClearBuffers() {
	glBindVertexArray(0);
}

unsigned int Mesh::GetIndexCount() {
	return numElements;
}


/* CUDA */
void Mesh::Sphere() {
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

	kernel_Pos(posPtr, width, height, thetaFac, phiFac);
	cudaDeviceSynchronize();
	cudaGraphicsUnmapResources(1, &resPosition, 0);
	/* Position  ====================================================================================== */




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
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndex);
	
	glBindVertexArray(0);
	/* VAO ============================================================================================ */
}






//void Mesh::CreateAsteroidSphere() {
//	float3* posPtr;
//	cudaGraphicsMapResources(1, &resPosition, 0);
//	size_t num_bytes;
//	cudaGraphicsResourceGetMappedPointer((void **)&posPtr, &num_bytes, resPosition);
//
//	float thetaFac = (M_PI) / (height-1);
//	float phiFac = (2.0f * M_PI) / (width-1);
//
//	kernel_Pos(posPtr, width, height, thetaFac, phiFac);
//	cudaDeviceSynchronize();
//	cudaGraphicsUnmapResources(1, &resPosition, 0);
//}



/* Used when loading mesh from .obj file! */
bool Mesh::Initialize() {
	std::filebuf fb;
	if(fb.open(fileName, std::ios::in)) {
		std::istream stream(&fb);
		vector<vec3> points;
		vector<vec3> normals;
		vector<vec2> texCoords;
		vector<Vertex> verts;

		int vertIndex = 0;
		vector<GLuint> faces;
		int nFaces = 0;

		string line, token;
		vector<ivec3> face;

		getline(stream, line);
		while(!stream.eof()) {
			trimString(line);
			if(line.length() > 0 && line.at(0) != '#') {
				istringstream lineStream(line);

				lineStream >> token;

				if(token == "v") {
					float x, y, z;
					lineStream >> x >> y >> z;
					points.push_back(vec3(x, y, z));
				}else if(token == "vt") {
					float u, v;
					lineStream >> u >> v;
					texCoords.push_back(vec2(u, v));
				}else if(token == "vn") {
					float x, y, z;
					lineStream >> x >> y >> z;
					normals.push_back(vec3(x, y, z));
				}else if(token == "f") {
					nFaces++;

					face.clear();
					size_t slash1, slash2;
					while(lineStream.good()) {
						string vertString;
						lineStream >> vertString;
						int pIndex = -1, nIndex = -1, tcIndex = -1;

						slash1 = vertString.find("/");
						if(slash1 == string::npos) {
							pIndex = atoi(vertString.c_str()) - 1;
						}else {
							slash2 = vertString.find("/", slash1 + 1);
							pIndex = atoi(vertString.substr(0, slash1).c_str()) - 1;
							if(slash2 > slash1 + 1) {
								tcIndex = atoi(vertString.substr(slash1 + 1, slash2).c_str()) - 1;
							}
							nIndex = atoi(vertString.substr(slash2 + 1, vertString.length()).c_str()) - 1;
						}
						face.push_back(ivec3(pIndex, tcIndex, nIndex));
					}

					ivec3 v0 = face[0];
					ivec3 v1 = face[1];
					ivec3 v2 = face[2];
					int baseIndex = verts.size();

					faces.push_back(baseIndex);
					verts.push_back(Vertex(points[v0.x], normals[v0.z], texCoords[v0.y]));
					faces.push_back(verts.size());
					verts.push_back(Vertex(points[v1.x], normals[v1.z], texCoords[v1.y]));
					faces.push_back(verts.size());
					verts.push_back(Vertex(points[v2.x], normals[v2.z], texCoords[v2.y]));

					for(unsigned int i = 3; i < face.size(); ++i) {
						v1 = v2;
						v2 = face[i];
						faces.push_back(baseIndex);
						faces.push_back(verts.size()-1);
						faces.push_back(verts.size());
						verts.push_back(Vertex(points[v2.x], normals[v2.z], texCoords[v2.y]));
					}
				}
			}
			getline(stream, line);
		}
		InitializeBuffers(verts, faces);
		fb.close();
		return true;
	}
	return false;
}









/* Used for loading mesh from .obj file! */
void Mesh::trimString(string & str) {
	const char * whiteSpace = " \t\n\r";
	size_t location;
	location = str.find_first_not_of(whiteSpace);
	str.erase(0, location);
	location = str.find_last_not_of(whiteSpace);
	str.erase(location + 1);
}

void Mesh::InitializeBuffers(const vector<Vertex>& vertices, const vector<GLuint>& faces) {
	/*numElements = faces.size();

	glGenBuffers(1, &attributeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, attributeBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(faces[0]), &faces[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, attributeBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLubyte*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLubyte*)(1*sizeof(vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLubyte*)(2*sizeof(vec3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	glBindVertexArray(0);*/
}
#pragma once

#include "Mesh.h"
#include "kernel.h"



Mesh::Mesh(const std::string name_) : name(name_) {

}


Mesh::~Mesh() {
	glDeleteBuffers(1, &vboPosition);
	glDeleteBuffers(1, &vboNormal);
	glDeleteBuffers(1, &vboTextureCoord);
	glDeleteBuffers(1, &vboIndex);
}


const std::string& Mesh::GetName()const {
	return this->name;
}

void Mesh::SetBuffers() {
	glBindVertexArray(vao);
}

void Mesh::ClearBuffers() {
	glBindVertexArray(0);
}

const unsigned int Mesh::GetIndexCount() const {
	return numElements;
}






/* Used when loading mesh from .obj file! */


//bool Mesh::Initialize() {
//	std::filebuf fb;
//	if(fb.open(fileName, std::ios::in)) {
//		std::istream stream(&fb);
//		vector<vec3> points;
//		vector<vec3> normals;
//		vector<vec2> texCoords;
//		vector<Vertex> verts;
//
//		int vertIndex = 0;
//		vector<GLuint> faces;
//		int nFaces = 0;
//
//		string line, token;
//		vector<ivec3> face;
//
//		getline(stream, line);
//		while(!stream.eof()) {
//			trimString(line);
//			if(line.length() > 0 && line.at(0) != '#') {
//				istringstream lineStream(line);
//
//				lineStream >> token;
//
//				if(token == "v") {
//					float x, y, z;
//					lineStream >> x >> y >> z;
//					points.push_back(vec3(x, y, z));
//				}else if(token == "vt") {
//					float u, v;
//					lineStream >> u >> v;
//					texCoords.push_back(vec2(u, v));
//				}else if(token == "vn") {
//					float x, y, z;
//					lineStream >> x >> y >> z;
//					normals.push_back(vec3(x, y, z));
//				}else if(token == "f") {
//					nFaces++;
//
//					face.clear();
//					size_t slash1, slash2;
//					while(lineStream.good()) {
//						string vertString;
//						lineStream >> vertString;
//						int pIndex = -1, nIndex = -1, tcIndex = -1;
//
//						slash1 = vertString.find("/");
//						if(slash1 == string::npos) {
//							pIndex = atoi(vertString.c_str()) - 1;
//						}else {
//							slash2 = vertString.find("/", slash1 + 1);
//							pIndex = atoi(vertString.substr(0, slash1).c_str()) - 1;
//							if(slash2 > slash1 + 1) {
//								tcIndex = atoi(vertString.substr(slash1 + 1, slash2).c_str()) - 1;
//							}
//							nIndex = atoi(vertString.substr(slash2 + 1, vertString.length()).c_str()) - 1;
//						}
//						face.push_back(ivec3(pIndex, tcIndex, nIndex));
//					}
//
//					ivec3 v0 = face[0];
//					ivec3 v1 = face[1];
//					ivec3 v2 = face[2];
//					int baseIndex = verts.size();
//
//					faces.push_back(baseIndex);
//					verts.push_back(Vertex(points[v0.x], normals[v0.z], texCoords[v0.y]));
//					faces.push_back(verts.size());
//					verts.push_back(Vertex(points[v1.x], normals[v1.z], texCoords[v1.y]));
//					faces.push_back(verts.size());
//					verts.push_back(Vertex(points[v2.x], normals[v2.z], texCoords[v2.y]));
//
//					for(unsigned int i = 3; i < face.size(); ++i) {
//						v1 = v2;
//						v2 = face[i];
//						faces.push_back(baseIndex);
//						faces.push_back(verts.size()-1);
//						faces.push_back(verts.size());
//						verts.push_back(Vertex(points[v2.x], normals[v2.z], texCoords[v2.y]));
//					}
//				}
//			}
//			getline(stream, line);
//		}
//		InitializeBuffers(verts, faces);
//		fb.close();
//		return true;
//	}
//	return false;
//}
//
//
//
//
//
//
//
//
//
///* Used for loading mesh from .obj file! */
//void Mesh::trimString(string & str) {
//	const char * whiteSpace = " \t\n\r";
//	size_t location;
//	location = str.find_first_not_of(whiteSpace);
//	str.erase(0, location);
//	location = str.find_last_not_of(whiteSpace);
//	str.erase(location + 1);
//}
//
//void Mesh::InitializeBuffers(const vector<Vertex>& vertices, const vector<GLuint>& faces) {
//	/*numElements = faces.size();
//
//	glGenBuffers(1, &attributeBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, attributeBuffer);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	glGenBuffers(1, &elementBuffer);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(faces[0]), &faces[0], GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, attributeBuffer);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLubyte*)0);
//
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLubyte*)(1*sizeof(vec3)));
//
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLubyte*)(2*sizeof(vec3)));
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
//
//	glBindVertexArray(0);*/
//}
#ifndef NORMALMESH_H
#define NORMALMESH_H


#include "Mesh.h"



class NormalMesh : public Mesh {
public:
	NormalMesh(std::string name);
	NormalMesh(std::string name, std::string file);
	~NormalMesh();



	void MakeBox();
	void MakeQuad();
	void MakeFromFile();



private:
	// private helper methods to load .obj file
	//bool Initialize();
	//void InitializeBuffers(const std::vector<Vertex>& vertices, const std::vector<GLuint>& faces);
	//void trimString(std::string & str);


private:
	
	std::string file;

};


#endif
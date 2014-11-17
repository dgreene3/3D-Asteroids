#ifndef MESHCONTAINER_H
#define MESHCONTAINER_H

#include "Precompiled.h"
#include "Mesh.h"





/* Class that contains all the mesh's */

/* Do memeory and resource allocation for mesh's */



class MeshContainter {
public:
	MeshContainter();
	~MeshContainter();

	void AddMesh(Mesh* mesh);
	Mesh* GetMeshPtr(const std::string name) const; /* Get a pointer to a mesh at index */
	Mesh* GetMeshPtr(unsigned int index)const;


private:
	std::vector<Mesh*> meshes;
	std::unordered_map<std::string, Mesh*> meshMap;
	/* Could use Hash Map to store <Name, Mesh*> so given a mesh name like Ship, or Earth, Marsh, I can get
		pointer to that mesh*/

};

#endif
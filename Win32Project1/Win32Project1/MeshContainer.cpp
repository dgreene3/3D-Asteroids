#pragma once


#include "MeshContainer.h"



MeshContainter::MeshContainter() {

}

MeshContainter::~MeshContainter() {
	// Go through all meshes and release memory
	for(auto it = meshMap.begin(); it != meshMap.end(); ++it) {
		Mesh* curr = it->second;
		delete curr;
	}

}

void MeshContainter::AddMesh(Mesh* mesh) {
	std::pair<std::string, Mesh*> newMesh(mesh->GetName(), mesh);
	meshMap.insert(newMesh);

	meshes.push_back(mesh);
}

Mesh* MeshContainter::GetMeshPtr(const std::string name)const {
	//std::unordered_map<std::string, Mesh*>::const_iterator element;
	std::unordered_map<std::string, Mesh*>::const_iterator it = meshMap.find(name);

	if(it == meshMap.end()) {
		return NULL;
	}
	return it->second;
}

Mesh* MeshContainter::GetMeshPtr(unsigned int index)const {
	return meshes.at(index);
}
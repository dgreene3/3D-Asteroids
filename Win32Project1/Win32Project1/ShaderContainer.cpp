#pragma once


#include "ShaderContainer.h"
#include "Shader.h"


ShaderContainer::ShaderContainer() {

}

ShaderContainer::~ShaderContainer() {
	// Go through all meshes and release memory
	for(auto it = shaderMap.begin(); it != shaderMap.end(); ++it) {
		Shader* curr = it->second;
		delete curr;
	}
}

void ShaderContainer::AddShaderProgram(Shader* shader, std::string name) {
	std::pair<std::string, Shader*> newShader(name, shader);
	shaderMap.insert(newShader);
}


Shader* ShaderContainer::GetShaderPtr(const std::string name)const {
	//std::unordered_map<std::string, Mesh*>::const_iterator element;
	std::unordered_map<std::string, Shader*>::const_iterator it = shaderMap.find(name);

	if (it == shaderMap.end()) {
		return NULL;
	}
	return it->second;
}


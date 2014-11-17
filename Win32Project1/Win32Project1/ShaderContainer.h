#ifndef SHADERCONTAINER_H
#define SHADERCONTAINER_H

#include "Precompiled.h"

#include "Shader.h"





class ShaderContainer {
public:
	ShaderContainer();
	~ShaderContainer();

	void AddShaderProgram(Shader* shader, std::string name);
	Shader* GetShaderPtr(unsigned int index) const;
	Shader* GetShaderPtr(const std::string name) const;

private:

	std::unordered_map<std::string, Shader*> shaderMap;

};

#endif
#ifndef SHADER_H
#define SHADER_H


#include "Precompiled.h"

#include "Uniform.h"




class Shader {
public:
	Shader();
	Shader(std::string vert, std::string frag);
	~Shader();

	void SetShader();
	void ClearShader();

	void AddUniform(const std::string name, UniformType type);



	/* Uniform set methods*/

	void SetTextureUniform(GLuint location, unsigned int texUnit);

	void SetUniform(const std::string name, glm::mat4& matrix);
	void SetUniform(const std::string name, unsigned int value);


private:
	bool Initialize();




private:
	GLuint shaderProgram;

	/* Uniforms */

	//std::vector<Uniform*> uniforms;
	std::unordered_map<std::string, Uniform*> uniforms;

	bool usingProgram;

	
};

#endif
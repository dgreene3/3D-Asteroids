#ifndef SHADER_H
#define SHADER_H


#include "Precompiled.h"

#include "Uniform.h"




class Shader {
public:
	Shader();
	Shader(std::string vert, std::string frag);
	~Shader();


	bool Initialize();

	void SetShader();
	void ClearShader();

	void AddUniform(const std::string name, UniformType type);



	/* Uniform set methods*/

	void SetTextureUniform(GLuint location, unsigned int texUnit);

	void SetUniform(const std::string name, glm::mat4& matrix);
	void SetUniform(const std::string name, unsigned int value);
	void SetUniform(const std::string name, glm::vec2& vector);

	GLuint GetHandle()const;

	

private:
	bool LoadShader(GLenum shaderType, const std::string& strFilename);
	bool CompileShader(GLenum shaderType, const std::string& shaderText);
	bool LinkProgram(GLuint program, GLuint vert, GLuint frag);
	bool CreateProgram(GLuint vert, GLuint frag);




private:
	// Files
	std::string vertex;
	std::string fragment;

	// Shaders
	GLuint vertShader;
	GLuint fragShader;


	GLuint shaderProgram;

	/* Uniforms */

	//std::vector<Uniform*> uniforms;
	std::unordered_map<std::string, Uniform*> uniforms;

	bool usingProgram;

	
};

#endif
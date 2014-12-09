#pragma once

#include "GraphicsGL.h"
#include "Shader.h"

Shader::Shader() {

	usingProgram = false;
}

Shader::Shader(std::string vert, std::string frag) {
	vertex = vert;
	fragment = frag;
}

Shader::~Shader() {

}


bool Shader::Initialize() {

	if(!LoadShader(GL_VERTEX_SHADER, vertex))
		return false;

	if(!LoadShader(GL_FRAGMENT_SHADER, fragment))
		return false;


	if(!CreateProgram(vertShader, fragShader))
		return false;

	usingProgram = false;

	return true;
}



void Shader::AddUniform(const std::string name, UniformType type) {
	GLuint uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());

	Uniform* newUniform = new Uniform(name, uniformLocation, type);

	uniforms.insert( std::make_pair(name, newUniform) );
}




void Shader::SetShader() {
	glUseProgram(shaderProgram);

	usingProgram = true;
}

void Shader::ClearShader() {
	glUseProgram(0);

	usingProgram = false;
}

GLuint Shader::GetHandle()const {
	return shaderProgram;
}


void Shader::SetUniform(const std::string name, glm::mat4& matrix) {
	std::unordered_map<std::string, Uniform*>::const_iterator it = uniforms.find(name);

	if (it != uniforms.end()) {
		it->second->SetUniform(matrix);
	}

}
void Shader::SetUniform(const std::string name, unsigned int value) {
	std::unordered_map<std::string, Uniform*>::const_iterator it = uniforms.find(name);

	if (it != uniforms.end()) {
		it->second->SetUniform(value);
	}
}

void Shader::SetUniform(const std::string name, glm::vec2& vector) {
	std::unordered_map<std::string, Uniform*>::const_iterator it = uniforms.find(name);

	if (it != uniforms.end()) {
		it->second->SetUniform(vector);
	}
}



void Shader::SetTextureUniform(GLuint location, unsigned int texUnit) {
	if(!usingProgram) {
		glUseProgram(shaderProgram);
	}

	glUniform1i(location, texUnit);
}


bool Shader::LoadShader(GLenum shaderType, const std::string& strFilename) {
	
	//open file
	std::ifstream shaderFile(strFilename.c_str());
	//check for errors in opening file
	if(shaderFile == NULL) {
		printf("Error opening file %s\n", strFilename);
		return false;
	}
	std::stringstream shaderData;
	shaderData << shaderFile.rdbuf();
	shaderFile.close();

	return CompileShader(shaderType,  shaderData.str());
}
bool Shader::CompileShader(GLenum shaderType, const std::string& shaderText) {

	GLuint shader = glCreateShader(shaderType);
	
	GLint textLength = (GLint)shaderText.size();
	const GLchar* pText = static_cast<const GLchar*>(shaderText.c_str());
	glShaderSource(shader, 1, &pText, &textLength);
	glCompileShader(shader);

	//check for errors
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char* strShaderType = NULL;
		switch(shaderType) {
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}
		printf("Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;

		return false; // Error
	}

	switch(shaderType) {
		case GL_VERTEX_SHADER:
			vertShader = shader;
			break;
		case GL_FRAGMENT_SHADER:
			fragShader = shader;
			break;
	}
	return true;;
}
bool Shader::LinkProgram(GLuint program, GLuint vert, GLuint frag) {
	glAttachShader(program, vert);
	glAttachShader(program, frag);

	glLinkProgram(program);

	//check for errors after linking
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar * strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		printf("Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;

		return false;
	}

	glDetachShader(program, vert);
	glDetachShader(program, frag);

	return true;;
}
bool Shader::CreateProgram(GLuint vert, GLuint frag) {
	shaderProgram = glCreateProgram();
	bool status = LinkProgram(shaderProgram, vert, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);

	return status;
}
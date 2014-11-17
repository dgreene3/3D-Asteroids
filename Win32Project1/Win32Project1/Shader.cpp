#pragma once

#include "GraphicsGL.h"
#include "Shader.h"

Shader::Shader() {

	usingProgram = false;
}

Shader::Shader(std::string vert, std::string frag) {
	GLuint vertShader, fragShader;

	vertShader = LoadShader(GL_VERTEX_SHADER, vert);
	fragShader = LoadShader(GL_FRAGMENT_SHADER, frag);

	shaderProgram = CreateProgram(vertShader, fragShader);

	usingProgram = false;
}

Shader::~Shader() {

}


bool Shader::Initialize() {

	return false;
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



void Shader::SetTextureUniform(GLuint location, unsigned int texUnit) {
	if(!usingProgram) {
		glUseProgram(shaderProgram);
	}

	glUniform1i(location, texUnit);
}
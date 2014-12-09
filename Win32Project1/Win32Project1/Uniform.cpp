#pragma once


#include "Uniform.h"


Uniform::Uniform(const std::string name, const GLuint uniform, UniformType type) {
	this->uniform = uniform;
	this->name = name;
	this->type = type;
}
Uniform::~Uniform() {

}

GLuint Uniform::GetLocation()const {
	return this->uniform;
}

void Uniform::SetUniform(unsigned int& integer) {
	glUniform1i(uniform, integer);
}
void Uniform::SetUniform(glm::mat4& matrix) {
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Uniform::SetUniform(glm::vec2& v) {
	glUniform2f(uniform, v.x, v.y);
}



#ifndef UNIFORM_H
#define UNIFORM_H

#include "Precompiled.h"




enum UniformType { INT_U, FLOAT_U, FLOAT2_U, FLOAT4_U, MATRIX4_U, SAMPLER2D_U };


class Uniform {
public:
	Uniform(const std::string name, const GLuint uniform, UniformType type);
	virtual ~Uniform();

	GLuint GetLocation()const;

	void SetUniform(unsigned int& integer);
	void SetUniform(glm::mat4& matrix);
	void SetUniform(glm::vec2& v);


protected:

	GLuint uniform;

	UniformType type;
	std::string name;
	
};


#endif
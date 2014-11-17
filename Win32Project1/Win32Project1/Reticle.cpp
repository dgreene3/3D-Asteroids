#pragma once

#include "Reticle.h"

using namespace glm;

Reticle::Reticle(Shader* shader) {

	this->shader = shader;

	const vec3 reticle[] = {
		vec3(0.05f, 0.05f, -1.0f),
		vec3(-0.05f, -0.05f, -1.0f),
		vec3(0.05f, -0.05f, -1.0f),
		vec3(-0.05f, 0.05f, -1.0f)
	};
	const unsigned int reticleI[] = {
		0, 1,
		2, 3
	};
	glGenBuffers(1, &reticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, reticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(reticle), reticle, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &reticleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reticleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(reticleI), reticleI, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &reticleVAO);
	glBindVertexArray(reticleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, reticleVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, reticleIndexVBO);

	glBindVertexArray(0);
}

Reticle::~Reticle() {

}

void Reticle::SetShader() {
	shader->SetShader();
}
void Reticle::ClearShader() {
	shader->ClearShader();
}

void Reticle::Render() {

	SetShader();

	glLineWidth(1.0f);

	glBindVertexArray(reticleVAO);

	glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);


	ClearShader();

}
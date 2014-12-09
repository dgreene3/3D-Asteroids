#pragma once

#include "Quad.h"


Quad::Quad(Shader* shader_, Texture* texture_, Mesh* mesh_) : shader(shader_), texture(texture_), mesh(mesh_) {
	
}

Quad::~Quad() {

}


void Quad::Initialize() {

}

void Quad::Render() {
	shader->SetShader();

	mesh->SetBuffers();


	texture->BindTexture(0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	mesh->ClearBuffers();

	shader->ClearShader();
}

void Quad::SetTexture(Texture* tex) {
	texture = tex;
}

#pragma once

#ifndef QUAD_H
#define QUAD_H

#include "Shader.h"
#include "Texture.h"
#include "NormalMesh.h"


class Quad {
public: 
	Quad(Shader* shader_, Texture* texture_, Mesh* mesh_);
	~Quad();

	void Initialize();
	void Render();

	void SetTexture(Texture* tex);

private:
	Shader* shader;
	Texture* texture;
	Mesh* mesh;

};





#endif
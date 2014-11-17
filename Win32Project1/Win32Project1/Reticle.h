#ifndef RETICLE_H
#define RETICLE_H

#include "Precompiled.h"

#include "Shader.h"

class Reticle {
public:
	Reticle(Shader* shader);
	~Reticle();

	void SetShader();
	void ClearShader();

	void Render();


private:

	GLuint reticleVAO;
	GLuint reticleVBO;
	GLuint reticleIndexVBO;

	Shader* shader;

};

#endif
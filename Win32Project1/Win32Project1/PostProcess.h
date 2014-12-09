#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Shader.h"
#include "FrameBufferObject.h"

class PostProcess {

public:
	PostProcess();
	~PostProcess();

	bool Initialize(glm::ivec2 _windowSize);

	void Bind();
	void UnBind();

	void Resize(glm::ivec2 _windowSize);
	void ChangeEffect();
	void Draw();


private:
	FrameBufferObject fbo;
	GLuint frameBufferVBO;
	GLuint frameBufferIBO;
	GLuint frameBufferVAO;

	std::vector<GLuint> postProcessSubroutines;

	Shader* postProcessShader;

	glm::ivec2 windowSize;
	std::vector<GLuint> effectSubroutines;

	int currentEffect;


};


#endif
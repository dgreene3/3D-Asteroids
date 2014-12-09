#pragma once

#include "PostProcess.h"

#include <iostream>

using namespace glm;

PostProcess::PostProcess() : currentEffect(0) {

}

PostProcess::~PostProcess() {
	delete postProcessShader;
}

bool PostProcess::Initialize(glm::ivec2 _windowSize) {
	this->windowSize = _windowSize;

	/* Initialize Frame Buffer Object */
	fbo.Initialize(windowSize);

	/* Initialize Shader */
	postProcessShader = new Shader("Shaders/PostProcess.vert", "Shaders/PostProcess.frag");
	if (!postProcessShader->Initialize())
		return false;
	/* Get Uniforms */
	postProcessShader->AddUniform("ColorMap", UniformType::SAMPLER2D_U);
	postProcessShader->AddUniform("size", UniformType::FLOAT2_U);


	
	/* Generate VBO and VAO */


	const float positionData[] = {
		// Front
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
	};


	const unsigned int indices[] = {
		//front
		0, 2, 1,
		3, 5, 4
	};

	// Generate buffers
	glGenBuffers(1, &frameBufferVBO);
	glBindBuffer(GL_ARRAY_BUFFER, frameBufferVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &frameBufferIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frameBufferIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &frameBufferVAO);
	glBindVertexArray(frameBufferVAO);

	glBindBuffer(GL_ARRAY_BUFFER, frameBufferVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frameBufferIBO);

	glBindVertexArray(0);





	/* Get Sub-routines */
	effectSubroutines.push_back(glGetSubroutineIndex(postProcessShader->GetHandle(), GL_FRAGMENT_SHADER, "drawTextureOnly"));
	effectSubroutines.push_back(glGetSubroutineIndex(postProcessShader->GetHandle(), GL_FRAGMENT_SHADER, "FXAA"));
	effectSubroutines.push_back(glGetSubroutineIndex(postProcessShader->GetHandle(), GL_FRAGMENT_SHADER, "drawPosterized"));
	effectSubroutines.push_back(glGetSubroutineIndex(postProcessShader->GetHandle(), GL_FRAGMENT_SHADER, "predatorVision"));
	effectSubroutines.push_back(glGetSubroutineIndex(postProcessShader->GetHandle(), GL_FRAGMENT_SHADER, "dreamVision"));
	effectSubroutines.push_back(glGetSubroutineIndex(postProcessShader->GetHandle(), GL_FRAGMENT_SHADER, "basicNoise"));
	effectSubroutines.push_back(glGetSubroutineIndex(postProcessShader->GetHandle(), GL_FRAGMENT_SHADER, "scanLinesChromatic"));

	return true;
}

void PostProcess::Bind() {
	fbo.Bind();
}


void PostProcess::UnBind() {
	fbo.UnBind();
}

void PostProcess::Resize(glm::ivec2 _windowSize) {
	fbo.TakeDown();
	fbo.Initialize(_windowSize);
}


void PostProcess::ChangeEffect() {
	currentEffect = (currentEffect + 1) % effectSubroutines.size(); // wrap around
}


void PostProcess::Draw() {
	//Clear screen
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set Viewport
	glViewport(0, 0, 800, 600);

	// Use Shader Program
	postProcessShader->SetShader();

	//Set current sub-routine
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &effectSubroutines[currentEffect]);

	// Update Uniforms
	vec2 windowSizeFloat = vec2(800, 600);
	postProcessShader->SetUniform("ColorMap", 0);
	postProcessShader->SetUniform("size", windowSizeFloat);

	// Activate Texture 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo.textures[0]);

	// Bind VAO
	glBindVertexArray(frameBufferVAO);
	// Draw 
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	// Clear context
	postProcessShader->ClearShader();
	glBindVertexArray(0);
	glDisable(GL_TEXTURE_2D);
}

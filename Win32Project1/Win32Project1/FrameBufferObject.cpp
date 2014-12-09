#pragma once

#include "FrameBufferObject.h"
#include <iostream>
#include <GL/freeglut.h>		// Used for error checking code

using namespace std;

// Forward declarations for helper functions
bool CheckGLErrors(const char* caller);
const char* FrameBufferError(GLint error_code);


FrameBufferObject::FrameBufferObject() {
	frameBuffer = GLuint(-1);
	depthBuffer = GLuint(-1);
	textures = NULL; // nullptr
}
FrameBufferObject::~FrameBufferObject() {
	this->TakeDown();
}

bool FrameBufferObject::Initialize(glm::ivec2 size, int numberColorAttatch, bool allocate_depth_buffer) {
	/* Set inputs */
	this->size = size;
	this->number_color_attatchments = numberColorAttatch;
	this->textures = new GLuint[numberColorAttatch];

	for(int i = 0; i < numberColorAttatch; ++i) 
		this->textures[i] = GLuint(-1);

	if( !CheckGLErrors("FrameBufferObject::Initialize - purging errors prior to initializing frame buffer object."))
		return false;

	/* Create and Bind frame buffer handle */
	glGenFramebuffers(1, &this->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

	if (!CheckGLErrors("FrameBufferObject::Initialize - purging errors prior to initializing textures."))
		return false;

	/* Generate textures for the frame buffer object */
	glGenTextures(numberColorAttatch, this->textures);
	for (int i = 0; i < numberColorAttatch; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->size.x, this->size.y, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->textures[i], 0);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "FrameBufferObject::Initialize failed to complete texture buffer " << FrameBufferError(status) << endl;
		return false;
	}

	/* Only create depth buffer if it was set to true */
	if (allocate_depth_buffer)
	{
		glGenRenderbuffers(1, &this->depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, this->depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->size.x, this->size.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "FrameBufferObject::Initialize failed to complete depth buffer " << FrameBufferError(status) << endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void FrameBufferObject::Bind(int color_attatchment_index) {
	/* Validate variables before we bind */
	assert(color_attatchment_index <= this->number_color_attatchments);
	assert(this->frameBuffer != GLuint(-1));

	//
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + color_attatchment_index);
}

void FrameBufferObject::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBufferObject::TakeDown() {
	/* Cleanup and release all resources associated with this frame buffer object */

	if( this->frameBuffer != GLuint(-1))
		glDeleteFramebuffers(1, &this->frameBuffer);

	if( this->depthBuffer != GLuint(-1))
		glDeleteFramebuffers(1, &this->depthBuffer);

	if(this->textures != NULL) {
		glDeleteTextures(this->number_color_attatchments, this->textures);
		delete [] this->textures;
	}
}







//////////////////////////////////////////////////////////////////////////////////////////////

//			Helper functions for checking OpenGL Errors										//

//////////////////////////////////////////////////////////////////////////////////////////////

bool CheckGLErrors(const char* caller) {
	GLenum e;
	bool r = true;
	
	while ((e = glGetError()) != GL_NO_ERROR)
	{
		r = false;
		cout << caller << " " << gluErrorString(e) << endl;
	}
	return r;
}
const char* FrameBufferError(GLint error_code) {
	switch (error_code)
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		return "GL_FRAMEBUFFER_UNDEFINED";

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";

	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";

	case GL_FRAMEBUFFER_COMPLETE:
		return "GL_FRAMEBUFFER_COMPLETE";

	case GL_FRAMEBUFFER_UNSUPPORTED:
		return "GL_FRAMEBUFFER_UNSUPPORTED";

	default:
		return "Unknown frame buffer error";
	}
}

#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H


#include <GL/glew.h>
#include <glm/glm.hpp>


class FrameBufferObject {

public:
	FrameBufferObject();
	~FrameBufferObject();

	bool Initialize(glm::ivec2 size, int numberColorAttatch = 1, bool allocate_depth_buffer = true);

	void Bind(int color_attatchment_index = 0);
	void UnBind();

	void TakeDown();

public:
	GLuint* textures;

private:
	glm::ivec2 size;
	int number_color_attatchments;

	GLuint frameBuffer;
	GLuint depthBuffer;
	//GLuint* textures; // Quick fix: make public so postProcess can access


};




#endif
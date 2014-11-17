#pragma once

#include "Texture.h"



Texture::Texture(const std::string name, const std::string imagePath) {
	this->name = name;
	this->file = imagePath;

	this->il_handle = BAD_IL_VALUE;
	this->il_texture_handle = BAD_GL_VALUE;
}
Texture::~Texture() {
	if (this->il_texture_handle != BAD_GL_VALUE)
		glDeleteTextures(1, &this->il_texture_handle);
	if (this->il_handle != BAD_IL_VALUE)
		ilDeleteImage(this->il_handle);

	this->il_handle = BAD_IL_VALUE;
	this->il_texture_handle = BAD_GL_VALUE;
}


bool Texture::Init() {
	// We are asserting that we have not initialized this object before.
	assert(this->il_handle == BAD_IL_VALUE);

	if ((this->il_handle = ilGenImage()) == BAD_IL_VALUE)
		return false;
	ilBindImage(this->il_handle);
	if (!ilLoadImage((const ILstring) file.c_str()))
		return false;

	glGenTextures(1, &this->il_texture_handle);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	this->width = ilGetInteger(IL_IMAGE_WIDTH);
	this->height = ilGetInteger(IL_IMAGE_HEIGHT);
	this->format = ilGetInteger(IL_IMAGE_FORMAT);	//6407 = 0x1907
	this->data = ilGetData();

	glTexImage2D(GL_TEXTURE_2D, 0, this->format, this->width,
		this->height, 0, this->format, GL_UNSIGNED_BYTE, this->data);

	return true;
}


const GLuint Texture::GetUniform()const {
	return il_texture_handle;
}

//void Texture::ActivateTexUnit(unsigned int texUnit) {
//	glActiveTexture(GL_TEXTURE0 + texUnit);
//}


void Texture::BindTexture(GLuint textureUnit) {

	// We are asserting that we have initialized this object before.
	assert(this->il_handle != BAD_IL_VALUE);

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, this->il_texture_handle);

}



/* Not used anymore */
GLuint Texture::LoadBMP(const std::string imagePath) {
	unsigned char* header = new unsigned char [54]; 
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char* data;

	std::ifstream imageFile(imagePath, std::ifstream::binary);

	if(imageFile == NULL) {
		printf("Image could not be opened\n");
		return 0;
	}

	imageFile.read((char*)header, 54);
		
	//Found from a resouce: either wikipedia or from some other source. 
	if( header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if(imageSize == 0) {
		imageSize = width*height*3; //24-bit BMP, so one byte for RGB
	}
	if(dataPos == 0) {
		dataPos = 54;
	}
	//

	data = new unsigned char[imageSize];

	imageFile.read((char*)data, imageSize);

	imageFile.close();

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	//free(data);
	delete data;

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


	return texture;
}


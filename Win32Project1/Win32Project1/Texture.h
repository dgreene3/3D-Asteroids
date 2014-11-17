#ifndef TEXTURE_H
#define TEXTURE_H

#include "Precompiled.h"





#define	BAD_IL_VALUE	IL_INVALID_VALUE
#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif // !BAD_GL_VALUE


class Texture {
public: 
	
	Texture(const std::string name, const std::string imageFile);
	~Texture();

	bool Init();
	const GLuint GetUniform()const;
	//void ActivateTexUnit(unsigned int texUnit);
	void BindTexture(GLuint textureUnit);


private:
	GLuint LoadBMP(const std::string imagePath);

	


private:
	std::string name;
	std::string file;

	/*GLuint texture;
	ILContainer* textureIL;*/

	ILint width;
	ILint height;
	ILint format;
	ILuint il_handle;
	GLuint il_texture_handle;
	GLvoid* data;

};

#endif
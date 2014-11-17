#ifndef TEXTURECONTAINER_H
#define TEXTURECONTAINER_H

#include "Precompiled.h"

#include "Texture.h"


class TextureContainer {
public:
	TextureContainer();
	~TextureContainer();


	void AddTexture(Texture* texture, std::string name);
	Texture* GetTexPtr(const std::string name)const;
	//Texture* GetTexPtr(unsigned int index)const;


private:
	std::unordered_map<std::string, Texture*> textureMap;

};

#endif
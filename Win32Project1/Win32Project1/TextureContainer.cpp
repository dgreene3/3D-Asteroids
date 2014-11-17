#pragma once

#include "TextureContainer.h"


TextureContainer::TextureContainer() {

}

TextureContainer::~TextureContainer() {
	for(auto it = textureMap.begin(); it != textureMap.end(); ++it) {
		Texture* curr = it->second;
		delete curr;
	}
}


void TextureContainer::AddTexture(Texture* texture, std::string name) {
	std::pair<std::string, Texture*> newTex(name, texture);
	textureMap.insert(newTex);

}

Texture* TextureContainer::GetTexPtr(const std::string name)const {
	std::unordered_map<std::string, Texture*>::const_iterator it = textureMap.find(name);

	if (it == textureMap.end()) {
		return NULL;
	}
	return it->second;
}




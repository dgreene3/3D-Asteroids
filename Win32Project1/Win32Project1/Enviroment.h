#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include "Precompiled.h"

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Object.h"

class Enviroment : public Object {
public:
	Enviroment();
	~Enviroment();

	void Update(float dt); 
	void Render();

	float GetRadius()const;
	ObjectType GetType()const;

	void Collide(Object* other);
};

#endif
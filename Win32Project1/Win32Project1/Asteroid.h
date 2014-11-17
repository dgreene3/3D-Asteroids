#ifndef ASTEROID_H
#define ASTEROID_H

#include "Precompiled.h"

#include "Object.h"


class Asteroid : public Object {
public:
	Asteroid(std::string file, Mesh* mesh, Shader* shader);
	~Asteroid();

	bool Initialize();

	void Update(float dt);


	float GetRadius();
	bool Hit();


protected:

	/* Velocity Direction */
	glm::vec3 vDir;
	float radius;

	int hitPoints;
};

#endif
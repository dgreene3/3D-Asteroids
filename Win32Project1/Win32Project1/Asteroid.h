#ifndef ASTEROID_H
#define ASTEROID_H

#include "Precompiled.h"
#include "Time.h"
#include "Object.h"


class Asteroid : public Object {
public:
	Asteroid(std::string file, Mesh* mesh, Shader* shader);
	~Asteroid();

	bool Initialize();

	void Update(float dt);


	float GetRadius()const;
	glm::vec3 GetPos()const;
	ObjectType GetType()const;

	void Collide(Object* other);


	bool Hit();
	void Reflect(); // version 2 will take other asteroids data to reflect


protected:

	/* Velocity Direction */
	glm::vec3 vDir;
	float radius;

	int hitPoints;

	bool delayedDelete;
	float timeLeft;
	Time* timer;
	
};

#endif
#ifndef LASER_H
#define LASER_H

#include "Precompiled.h"
#include "Object.h"

class Laser : public Object {
public:
	Laser(Mesh* mesh, Shader* shader);
	~Laser();

	bool Initialize(glm::vec3 pos, glm::vec3 vDir, float theta, float phi);

	void Update(float dt);

	float GetRadius()const;
	glm::vec3 GetPos()const;
	ObjectType GetType()const;

	void Collide(Object* other);

private:

	glm::vec3 vDir;
	float length, width, height;
	float radius;
	glm::mat4 rotationMatrix;
	float lifeSpan;
};

#endif



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

private:

	glm::vec3 vDir;
	float length, width, height;
	float radius;
	glm::mat4 rotationMatrix;
};

#endif





#ifndef POWERUP_H
#define POWERUP_H


#include "Object.h"
#include "Player.h"

class PowerUp : public Object {
public:
	PowerUp(Mesh* mesh, Shader* shader);
	~PowerUp();

	/* Methods */


	/* Overrides */
	bool Initialize(glm::vec3 pos_);
	void Update(float dt);
	void Collide(Object* other);
	ObjectType GetType()const;
	float GetRadius()const;

private:



	float radius;




};


#endif
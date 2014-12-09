#pragma once


#ifndef PLAYER_H
#define PLAYER_H


#include "Precompiled.h"

#include "Camera.h"
#include "Laser.h"







class Player : public Object {
public:
	Player();
	~Player();

	void Initialize(glm::vec3 pos, glm::vec3 dir);

	void Update(glm::vec3& pos, glm::vec3& dir);

	void Hit();
	bool CheckHit();
	bool Alive();
	void Reset(glm::vec3& pos, glm::vec3& dir);

	bool isPowerUpActive()const;


	glm::vec3 GetPos();
	float GetRadius()const;
	ObjectType GetType()const;

	void Collide(Object* other);

	void Fire(std::vector<Object*> * bullets, Mesh* mesh, Shader* shader, Camera* cam);


private:

	glm::vec3 PosW; // Position of player in world space
	glm::vec3 lookDir; 

	unsigned int livesRemaining;

	// Data for physics collision detection
	float radius;

	bool tookHit; 
	bool isAlive;

	bool PowerUpActive;
};

#endif
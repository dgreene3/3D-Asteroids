#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "Laser.h"

#include "Precompiled.h"





class Player {
public:
	Player();
	~Player();

	void Initialize(glm::vec3 pos, glm::vec3 dir);

	void Update(glm::vec3& pos, glm::vec3& dir);

	void Hit();
	bool CheckHit();
	bool Alive();
	void Reset(glm::vec3& pos, glm::vec3& dir);


	glm::vec3 GetPos();
	float GetRadius();


private:

	glm::vec3 PosW; // Position of player in world space
	glm::vec3 lookDir; 

	unsigned int livesRemaining;

	// Data for physics collision detection
	float radius;

	bool tookHit; 
	bool isAlive;
};

#endif
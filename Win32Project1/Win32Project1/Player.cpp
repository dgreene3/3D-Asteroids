
#pragma once

#include "Player.h"
#include "PowerUp.h"

#define PLAYER_SIZE 2.0f
#define LIVES 2;
#define BULLET_SPEED 600.0f		// Bullet speed


using namespace glm;


Player::Player() {

}
Player::~Player() {

}

void Player::Initialize(glm::vec3 pos, glm::vec3 dir) {
	
	this->PosW = pos;	// set player starting position
	this->lookDir = dir;	// direction the player is looking

	this->radius = PLAYER_SIZE;
	this->livesRemaining = LIVES;

	this->isAlive = true;

	this->tookHit = false;

	this->PowerUpActive = false;
}

void Player::Reset(glm::vec3& pos, glm::vec3& dir) {
	this->PosW = pos;	
	this->lookDir = dir;

	this->tookHit = false;
	this->isAlive = true;
}

void Player::Update(glm::vec3& pos, glm::vec3& dir) {
	this->PosW = pos;

	this->lookDir = dir;


}

void Player::Hit() {
	--livesRemaining;
	if(livesRemaining == 0) {
		this->isAlive = false;
	}
	tookHit = true;
}
bool Player::CheckHit() {
	return tookHit;
}
bool Player::Alive() {
	return isAlive;
}

glm::vec3 Player::GetPos() {
	return PosW;
}
float Player::GetRadius()const {
	return radius;
}

ObjectType Player::GetType()const {
	return ObjectType::PLAYER;
}


void Player::Collide(Object* other) {

	switch(other->GetType()) {
		case ObjectType::ASTEROID:
			this->Hit();
			this->PowerUpActive = false;
			break;
		case ObjectType::PLANET:
			this->Hit();
			this->PowerUpActive = false;
			break;
		case ObjectType::BULLET:
			this->PowerUpActive = false;
			break;
		case ObjectType::LEVEL:
			this->Hit();
			this->PowerUpActive = false;
			break;
		case ObjectType::POWER_UP:
			this->PowerUpActive = true;
			break;
	}
}

void Player::Fire(std::vector<Object*> * bullets, Mesh* mesh, Shader* shader, Camera* cam) {
	Laser* newBullet = new Laser(mesh, shader);

	// Get ship position "cam position"
	vec3 pos = cam->GetPosition();
	vec3 right = cam->GetRightDir();
	vec3 bulletPos = pos + (0.5f * right);
	bulletPos.y -= 0.5f;

	vec3 lookDir = cam->GetForwardDir();
	vec3 target = lookDir * 100.0f + pos; 
	vec3 targetVector = target - bulletPos;
	targetVector = glm::normalize(targetVector);
	targetVector *= BULLET_SPEED;

	float theta = cam->GetTheta();
	float phi = cam->GetPhi();

	newBullet->Initialize(bulletPos, targetVector, theta, phi); // pos, target vector, theta, phi
	bullets->push_back(newBullet);



	// Fire second bullet if powerup is active
	if(PowerUpActive) {
		newBullet = new Laser(mesh, shader);

		bulletPos = pos - (0.5f * right);
		bulletPos.y -= 0.5f;

		newBullet->Initialize(bulletPos, targetVector, theta, phi);
		bullets->push_back(newBullet);
	}


}

bool Player::isPowerUpActive()const {
	return PowerUpActive;
}


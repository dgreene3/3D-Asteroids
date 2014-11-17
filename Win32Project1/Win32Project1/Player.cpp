
#pragma once

#include "Player.h"

#define PLAYER_SIZE 2.0f
#define LIVES 2;


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
float Player::GetRadius() {
	return radius;
}


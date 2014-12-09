#pragma once

#include "Asteroid.h"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

using namespace std;


Asteroid::Asteroid(string file, Mesh* mesh, Shader* shader) : Object(mesh, shader, true) {
	this->objFile = file;

	isActive = true;
	vDir = glm::vec3(0.0f);

	hitPoints = 5;

	delayedDelete = false;
	timeLeft = 3.0f;
	timer = new Time();
	timer->Reset();
}


bool Asteroid::Initialize() {
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> theta(0.0f, 180.0f);
	uniform_real_distribution<> phi(0.0f, 360.0f);
	uniform_real_distribution<> size(5.0f, 30.0f);
	uniform_real_distribution<> velocity(1.0f, 50.0f);
	uniform_real_distribution<> dist(100.0f, 1500.0f);


	float x, y, z;
	float randomTheta, randomPhi;
	/* Initialize starting position */
	
	// Get random number in degrees, then convert to radians
	randomTheta = (float)( theta(gen) * (PI / 180.0f) ); // [0, pi]
	randomPhi = (float)( phi(gen) * (PI / 180.0f) );	  // [0, 2pi]

	
	float distance = (float)dist(gen);
	// Get random vector on unit sphere
	x = distance * sinf(randomTheta) * cosf(randomPhi);
	y = distance * cosf(randomTheta);
	z = distance * sinf(randomTheta) * sinf(randomPhi);
	
	this->PosW = glm::vec3(x, y, z);



	vDir = glm::vec3(y, z, x) - PosW; // direction towards origin of world, which is sun
	vDir = glm::normalize(vDir);
	vDir *= velocity(gen); // Init velocity between 1.0f and 50.0f

	/* Size of asteroid */
	radius = (float)(size(gen)); // Init size of asteroid between 1.0f and 50.0f


	return true;
}


void Asteroid::Update(float dt) {
	/* Update position based on velocity */
	glm::vec3 step = this->vDir * dt;
	PosW += step;

	/* Create and set transformation matrix */
	glm::mat4 translateMatrix(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(PosW.x, PosW.y, PosW.z));
	glm::mat4 scaleMatrix(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(radius));
	
	SetTransformation(translateMatrix * scaleMatrix);


	// If we exit game world, delayed destroy so that the asteroid
	// goes outside the game world and we don't see it just disappear
	if(delayedDelete) {
		timer->Tick();
		float delta = timer->DeltaTime();
		timeLeft -= delta;
		if(timeLeft <= 0) {
			this->FlagToDelete();
		}
	}

}


glm::vec3 Asteroid::GetPos()const {
	return PosW;
}

float Asteroid::GetRadius()const {
	return radius;
}

ObjectType Asteroid::GetType()const {
	return ObjectType::ASTEROID;
}


bool Asteroid::Hit() {
	--hitPoints;

	if(hitPoints > 0) {
		return false; // not destoryed yet
	}else {
		return true; // destory asteroid
	}
}


void Asteroid::Reflect() {
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> velocity(1.0f, 50.0f);

	float x, y, z;
	x = (float)velocity(gen);
	y = (float)velocity(gen);
	z = (float)velocity(gen);

	vDir = glm::vec3(x, y, z);
	vDir = glm::normalize(vDir);
	vDir *= velocity(gen); // Init velocity between 1.0f and 50.0f

	// offset asteroid by size
	//PosW = (radius * vDir) + PosW;
}


void Asteroid::Collide(Object* other) {

	switch(other->GetType()) {
		case ObjectType::ASTEROID:
			if(other->GetRadius() > this->GetRadius()) {
				this->FlagToDelete(); // won't be deleted until we finish all events
			}else {
				this->Reflect();
			}
			break;
		case ObjectType::PLANET:
			//this->FlagToDelete();
			this->Reflect();
			break;
		case ObjectType::BULLET:
			if(this->Hit()) {
				this->FlagToDelete();
			}
			break;
		case ObjectType::LEVEL:
			delayedDelete = true;
			timer->Start();
			break;
		case ObjectType::PLAYER:
			//this->FlagToDelete();
			//this->Reflect();
			break;
	}
	
}
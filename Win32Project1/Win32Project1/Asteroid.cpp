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
}


bool Asteroid::Initialize() {
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> theta(0.0f, 180.0f);
	uniform_real_distribution<> phi(0.0f, 360.0f);
	uniform_real_distribution<> size(1.0f, 30.0f);
	uniform_real_distribution<> velocity(1.0f, 50.0f);


	float x, y, z;
	float randomTheta, randomPhi;
	/* Initialize starting position */
	
	// Get random number in degrees, then convert to radians
	randomTheta = (float)( theta(gen) * (PI / 180.0f) ); // [0, pi]
	randomPhi = (float)( phi(gen) * (PI / 180.0f) );	  // [0, 2pi]

	
	// Get random vector on unit sphere
	x = 1000.0f * sinf(randomTheta) * cosf(randomPhi);
	y = 1000.0f * cosf(randomTheta);
	z = 1000.0f * sinf(randomTheta) * sinf(randomPhi);
	
	this->PosW = glm::vec3(x, y, z);
	//this->PosW = glm::vec3(0.0f, 0.0f, 100.0f);
	vDir = glm::vec3(0.0f) - PosW; // direction towards origin of world, which is sun
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

}



float Asteroid::GetRadius() {
	return radius;
}


bool Asteroid::Hit() {
	--hitPoints;

	if(hitPoints > 0) {
		return false; // not destoryed yet
	}else {
		return true; // destory asteroid
	}
}
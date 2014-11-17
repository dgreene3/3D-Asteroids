#pragma once

#include "Laser.h"



using namespace glm;

#define WIDTH_SCALE 0.10f
#define HEIGHT_SCALE 0.10f
#define LENGTH_SCALE 0.10f
#define M_PI 3.14159265358979323846f

Laser::Laser(Mesh* mesh, Shader* shader) : Object(mesh, shader, false) {

	this->texture = NULL; // no texture for this object
	length = width = 0;
	
}

Laser::~Laser() {

}


bool Laser::Initialize(vec3 pos, vec3 vDir, float theta, float phi) {
	width = 1.0f * WIDTH_SCALE;  // x dimension
	height = 1.0f * HEIGHT_SCALE; // y dimension
	length = 1.0f * LENGTH_SCALE; // z dimension

	radius = glm::max(glm::max(width, height), length);


	this->PosW = pos;
	this->vDir = vDir;

	// Calculate rotation matrix
	mat4 rot(1.0f);
	// angle in degrees
	float phiDeg = phi * (180.0f / M_PI);
	rot = glm::rotate(rot, phi, vec3(0.0f, 1.0f, 0.0f)); 
	rotationMatrix = rot;

	return true;
}


void Laser::Update(float dt) {
	vec3 step = this->vDir * dt;
	PosW += step;

	mat4 translateMatrix(1.0f);
	translateMatrix = glm::translate(translateMatrix, vec3(PosW.x, PosW.y, PosW.z));
	mat4 scaleMatrix(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, vec3(width, height, length));
	

	SetTransformation(translateMatrix * rotationMatrix * scaleMatrix);
}

float Laser::GetRadius()const {
	return radius;
}
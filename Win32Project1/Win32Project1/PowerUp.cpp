#pragma once


#include "PowerUp.h"

#define POWER_UP_SIZE 50.0f


PowerUp::PowerUp(Mesh* mesh, Shader* shader) : Object(mesh, shader, false) {

}

PowerUp::~PowerUp() {

}

void PowerUp::Update(float dt) {
	
}

bool PowerUp::Initialize(glm::vec3 pos_) {
	// Set Position
	this->PosW = pos_;

	this->radius = POWER_UP_SIZE;

	glm::mat4 translateMatrix(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(PosW.x, PosW.y, PosW.z));

	glm::mat4 scaleMatrix(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(radius));

	SetTransformation(translateMatrix * scaleMatrix);

	return true;
}
void PowerUp::Collide(Object* other) {
	// always delete on collision
	this->FlagToDelete();
}
ObjectType PowerUp::GetType()const {
	return ObjectType::POWER_UP;
}
float PowerUp::GetRadius()const {
	return radius;
}
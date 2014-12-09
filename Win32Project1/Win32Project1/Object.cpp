#pragma once


#include "Object.h"



Object::Object() {

}

Object::Object(Mesh* m, Shader* s, bool textureEnabled_) {
	shaderProgram = s;
	mesh = m;
	textureEnabled = textureEnabled_;
}


Object::~Object() {

}

bool Object::Initialize() {

	return false; /* Temp to get to compile */
}



void Object::SetTransformation(glm::mat4& transform) {
	this->transformMatrix = transform;
}

void Object::SetMesh(Mesh* meshPtr) {
	this->mesh = meshPtr;
}
void Object::EnableMesh() {
	mesh->SetBuffers();
}
void Object::DisableMesh() {
	mesh->ClearBuffers();
}

void Object::SetShader(Shader* shaderPtr) {
	this->shaderProgram = shaderPtr;
}
void Object::EnableShader() {
	shaderProgram->SetShader();
}
void Object::DisableShader() {
	shaderProgram->ClearShader();
}

void Object::SetTexture(Texture* tex) {
	this->texture = tex;
}



void Object::Update(float dt) {
	// Every object in the game derives from class Object and overrides this
	// method based on each objects own behavior
}


/* Set any uniforms specific to object in here */
void Object::Render() {

	unsigned int indexCount = mesh->GetIndexCount();


	/* Bind VAO */
	EnableMesh();

	/* Set Texture */
	if(this->hasTexture()) {
		texture->BindTexture(0);
	}else {
		// unbind textures
	}

	/* Set any uniforms */
	shaderProgram->SetUniform("worldMatrix", transformMatrix);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);


	DisableMesh();
}



glm::vec3 Object::GetPos()const {
	return PosW;
}


bool Object::GetActive() {
	return isActive;
}

bool Object::hasTexture() {
	return textureEnabled;
}

void Object::FlagToDelete() {
	this->isActive = false;
}


const std::string& Object::GetTextureName()const {
	return texName;
}

void Object::Collide(Object* other) {

}
















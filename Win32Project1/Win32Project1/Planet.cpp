#pragma once

#include "Planet.h"


#define PI 3.141592654
#define PLANET_ORBIT_SCALE 160.0f
#define PLANET_SIZE_SCALE 6.0f


using namespace std;




Planet::Planet(string file, Mesh* mesh, Shader* shader) : Object(mesh, shader, true){
	this->objFile = file;

	planetRadius = 0.0f;
	orbitRadius = 0.0f;
	orbitSpeed = 0.0f;
	rotationSpeed = 0.0f;

	currRadiansRotation = 0.0f;
	currRadiansOrbit = 0.0f;
}



Planet::~Planet() {

}

bool Planet::Initialize() {
	/* Open object file for reading data */
	ifstream stream;
	string currLine;

	stream.open(objFile);

	if(!stream.is_open()) {
		return false;
	}

	getline(stream, currLine);
	istringstream lineStream(currLine);

	lineStream >> meshName;			// Mesh type: Sphere
	lineStream >> texName;			// texture file
	lineStream >> planetRadius;		//
	lineStream >> orbitRadius;		//
	lineStream >> orbitSpeed;		//
	lineStream >> rotationSpeed;	//

	orbitRadius *= PLANET_ORBIT_SCALE;
	planetRadius *= PLANET_SIZE_SCALE;

	//orbitSpeed = 0.0f; // testing purposes so planets dont' move

	/* Initialize starting position */
	float random = (float)(rand() / 32767.0f);
	float rad = (float)( random * 2.0f * PI );
	currRadiansOrbit = rad; // so planets don't start in a line
		
	stream.close();

	return true;
}

void Planet::Update(float dt) {
	/* Calculate position */
	float radians = orbitSpeed * dt;
	currRadiansOrbit += radians;
	if(currRadiansOrbit >= 2.0f * PI) { currRadiansOrbit = 0.0f; }

	float x, z;
	x = this->orbitRadius * cosf(currRadiansOrbit);
	z = this->orbitRadius * sinf(currRadiansOrbit);
	PosW = glm::vec3(x, 0.0f, z);
	glm::mat4 translateMatrix(1.0f);
	translateMatrix = glm::translate(translateMatrix, glm::vec3(PosW.x, PosW.y, PosW.z));

	/* Calculate rotation */
	glm::mat4 rotationMatrix = glm::mat4(1.0f);

	radians = rotationSpeed * dt;
	currRadiansRotation  += radians;

	if(currRadiansRotation >= (2.0f * PI)) { currRadiansRotation = 0.0f; }

	float degree = (float)( currRadiansRotation * ( 180.0f / PI ) );

	rotationMatrix = glm::rotate(rotationMatrix, degree, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 scaleMatrix(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(planetRadius, planetRadius, planetRadius));

	glm::mat4 tMatrix(1.0f);
	tMatrix = translateMatrix * rotationMatrix * scaleMatrix;

	/* Set Transformation matrix which is the World Matrix */
	SetTransformation(tMatrix);
}


glm::vec3 Planet::GetPos()const {
	return PosW;
}

float Planet::GetRadius()const {
	return planetRadius;
}

ObjectType Planet::GetType()const {
	return ObjectType::PLANET;
}



void Planet::Collide(Object* other) {


	
}

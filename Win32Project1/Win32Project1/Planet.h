#ifndef PLANET_H
#define PLANET_H

#include "Precompiled.h"

#include "Object.h"


class Planet : public Object {
public:
	Planet(std::string file, Mesh* mesh, Shader* shader);
	~Planet();


	/* when planet object is created, read in file, and then do all planet */
	/* stuff and then I can also call Object::Init() to init any generic object stuff*/

	bool Initialize(); 

	void Update(float dt); 


	float GetRadius();
	
private:

	float planetRadius;
	float orbitRadius;
	float orbitSpeed; 
	float rotationSpeed;

	float currRadiansRotation;
	float currRadiansOrbit;

};


#endif
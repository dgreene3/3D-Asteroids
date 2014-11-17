#pragma once


#include "Physics.h"
#include "Asteroid.h"
#include "Planet.h"

using namespace glm;


bool StaticSphereToSphere(vec3 Center1, float R1, vec3 Center2, float R2) {
	float sumR = R1 + R2; // Sum of the two radii

	float dist = distance(Center1, Center2); // distance between two spheres

	if(dist <= sumR) {
		return true; // Intersection
	}else {
		return false; // No Intersection
	}
}


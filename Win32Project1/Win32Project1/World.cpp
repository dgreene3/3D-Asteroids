#pragma once

#include "World.h"
#include "Physics.h"



using namespace glm;

World::World() {
	
}
World::~World() {
	// free all objects
}

void World::Initialize(glm::vec3 pos, glm::vec3 dir) {
	/* Initialize player */

	player = new Player();
	player->Initialize(pos, dir);

}

void World::UpdatePlayer(glm::vec3 pos, glm::vec3 dir) {
	player->Update(pos, dir);
}



void World::AddObject(Object* obj) {
	
}
void World::RemoveObject() {
	
}

void World::Update(float dt) {
	

	/* Update objects */
	
	/* Update Static Objects */
	for(unsigned int i = 0; i < staticObjects.size(); ++i) {
		staticObjects.at(i)->Update(dt);
	}
	/* Update Dynamic Objects */
	for(unsigned int i = 0; i < asteroids.size(); ++i) {
		asteroids.at(i)->Update(dt);
	}
	for(unsigned int i = 0; i < bullets.size(); ++i) {
		bullets.at(i)->Update(dt);
	}


	/* Physics */

	/* ================= Check for collisions ========================== */

	/* Check for collisions between Asteroids and Planets and Sun and bullets */
	for(unsigned int i = 0; i < asteroids.size(); ++i) {
		Asteroid* currA = dynamic_cast<Asteroid*>(asteroids.at(i));
		vec3 currPos = currA->GetPos();
		float radius = currA->GetRadius();
		// Go through each static object for each asteroid 
		for(unsigned int j = 0; j < staticObjects.size(); ++j) {
			// Asteroid 1, Planet, Sun 2
			Planet* currP = dynamic_cast<Planet*>(staticObjects.at(j));
			float radiusP = currP->GetRadius();
			if( StaticSphereToSphere(currPos, radius, currP->GetPos(), radiusP) ) {
				currA->FlagToDelete();
				freeAsteroidList.push_back(i);
			}
		}
		// Go through all bullets 
		for(unsigned int j = 0; j < bullets.size(); ++j) {
			Laser* currLaser = dynamic_cast<Laser*>(bullets.at(j));
			vec3 laserPos = currLaser->GetPos();
			float r = currLaser->GetRadius();

			if( StaticSphereToSphere(currPos, radius, laserPos, r) ) {
				if(currA->Hit()) {
					// destroy asteroid after so many hits
					currA->FlagToDelete();
					freeAsteroidList.push_back(i);

					// destory bullet
					currLaser->FlagToDelete();
					freeBulletList.push_back(j);
				}else {
					// destroy only bullet
					currLaser->FlagToDelete();
					freeBulletList.push_back(j);
				}
			}
		}
	}


	// BUGS HERE 
	/* Check for collision between two Asteroids */
	if(asteroids.size() >= 2) {
		for(unsigned int i = 0; i < asteroids.size()-1; ++i) {
			Asteroid* a1 = dynamic_cast<Asteroid*>(asteroids.at(i));
			for(unsigned int j = i+1; j < asteroids.size(); ++j) {
				Asteroid* a2 = dynamic_cast<Asteroid*>(asteroids.at(j));

				if( StaticSphereToSphere(a1->GetPos(), a1->GetRadius(), a2->GetPos(), a2->GetRadius()) ) {
					a1->FlagToDelete();
					a2->FlagToDelete();

					freeAsteroidList.push_back(i);
					freeAsteroidList.push_back(j);
				}
			}
		}
	}
	

	/* Check for collision between bullet and planet/sun */
	for(unsigned int i = 0; i < bullets.size(); ++i) {
		Laser* currLaser = dynamic_cast<Laser*>(bullets.at(i));
		vec3 laserPos = currLaser->GetPos();
		float r = currLaser->GetRadius();

		/* Planets and sun */
		for(unsigned int j = 0; j < staticObjects.size(); ++j) {
			Planet* currPlanet = dynamic_cast<Planet*>(staticObjects.at(j));

			if( StaticSphereToSphere(laserPos, r, currPlanet->GetPos(), currPlanet->GetRadius()) ) {
				currLaser->FlagToDelete();
				freeBulletList.push_back(i);
				break;
			}
		}
	}

	/* Check for collision between Player and [Asteroids, Planets, and Sun] */
	vec3 playerPos = player->GetPos();
	float playerR = player->GetRadius();
	bool playerHit = false;

	for(unsigned int i = 0; i < asteroids.size(); ++i) {
		Asteroid* currA = dynamic_cast<Asteroid*>(asteroids.at(i));

		if( StaticSphereToSphere(playerPos, playerR, currA->GetPos(), currA->GetRadius()) ) {
			// player hit
			player->Hit();
			playerHit = true; // if true, we don't check second for loop

			currA->FlagToDelete();
			freeAsteroidList.push_back(i);

			break;

		}
	}
	// only check if we are not hit yet, if we are hit, no need to do more checking
	if(!playerHit) {
		for(unsigned int i = 0; i < staticObjects.size(); ++i) {
			Planet* currP = dynamic_cast<Planet*>(staticObjects.at(i));

			if( StaticSphereToSphere(playerPos, playerR, currP->GetPos(), currP->GetRadius()) ) {
				// player hit
				player->Hit();
				playerHit = true;

				break;
			}
		}
	}
	



	/* Check if bullet or asteroid is outside of stars, if radius is more than stars radius */




	/* Dynamically spawn more asteroids every n seconds outside of world */


	/* Release all in-active objects */
	// BUGS HERE TOO
	auto it = asteroids.begin();
	for(unsigned int i = 0; i < freeAsteroidList.size(); ++i) {
		int index = freeAsteroidList.at(i); // index of asteroid in vector 
		delete asteroids.at(index);			// delete object release memory
		asteroids.erase(it+index);			// remove asteroid from vector of asteroids
	}
	freeAsteroidList.resize(0);

	it = bullets.begin();
	for(unsigned int i = 0; i < freeBulletList.size(); ++i) {
		int index = freeBulletList.at(i); // index of asteroid in vector 
		delete bullets.at(index);			// delete object release memory
		bullets.erase(it+index);			// remove asteroid from vector of asteroids
	}
	freeBulletList.resize(0);
}

void World::Render() {


	/* Draw all static objects like planets */
	for(unsigned int i = 0; i < staticObjects.size(); ++i) {
		staticObjects.at(i)->Render();
	}
	/* Draw enviroment level */
	levelEnv->Render();
	

	/* Draw all dynamic objects like asteroids */
	for(unsigned int i = 0; i < asteroids.size(); ++i) {
		asteroids.at(i)->Render();
	}


	int i = 0;
	currShader->SetUniform("test", i);
	/* Draw all bullets */
	for(unsigned int i = 0; i < bullets.size(); ++i) {
		bullets.at(i)->Render();
	}

}



void World::AddStaticObject(Object* obj) {
	staticObjects.push_back(obj);
}
void World::AddAsteroidObject(Object* obj) {
	asteroids.push_back(obj);
}
void World::AddBulletObject(Object* obj) {
	bullets.push_back(obj);
}
void World::AddLevelObject(Object* obj) {
	levelEnv = obj;
}

void World::SetCurrShader(Shader* shader) {
	currShader = shader;
}


bool World::CheckPlayerHit() {
	return player->CheckHit();
}
bool World::CheckPlayerAlive() {
	return player->Alive();
}
void World::ResetPlayer(glm::vec3 pos, glm::vec3 dir) {
	player->Reset(pos, dir);
}
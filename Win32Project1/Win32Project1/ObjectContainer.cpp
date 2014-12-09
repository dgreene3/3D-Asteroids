//#pragma once
//
//#include "Precompiled.h"
//
//#include "ObjectContainer.h"
//
//#include "Physics.h"
//#include "Asteroid.h"
//#include "Planet.h"
//#include "Laser.h"
//
//#include "CollisionEvent.h"
//
//
//using namespace glm;
//
//
//
//
//ObjectContainer::ObjectContainer() {
//
//
//
//}
//
//ObjectContainer::~ObjectContainer() {
//
//
//}
//
//
//void ObjectContainer::AddObject(Object* obj, ObjectType type) {
//
//	switch (type) {
//		case ObjectType::ASTEROID:
//			asteroids.push_back(obj);
//			break;
//		case ObjectType::BULLET:
//			bullets.push_back(obj);
//			break;
//		case ObjectType::PLANET:
//			staticObjects.push_back(obj);
//			break;
//		case ObjectType::LEVEL:
//			levelEnv = obj;
//			break;
//	}
//}
//
//void ObjectContainer::Update(float dt, Player* player) {
//
//
//	/*						Update objects								 */
//
//	/* Update Static Objects */
//	for (unsigned int i = 0; i < staticObjects.size(); ++i) {
//		staticObjects.at(i)->Update(dt);
//	}
//	/* Update Dynamic Objects */
//	for (unsigned int i = 0; i < asteroids.size(); ++i) {
//		asteroids.at(i)->Update(dt);
//	}
//	for (unsigned int i = 0; i < bullets.size(); ++i) {
//		bullets.at(i)->Update(dt);
//	}
//
//
//	/*							Physics									 */
//
//	/* ================= Check for collisions ========================== */
//
//	/* Check for collisions between Asteroids and Planets and Sun and bullets */
//	for (unsigned int i = 0; i < asteroids.size(); ++i) {
//		Asteroid* currA = dynamic_cast<Asteroid*>(asteroids.at(i));
//		vec3 currPos = currA->GetPos();
//		float radius = currA->GetRadius();
//
//
//
//
//		// Check if Asteroid hits Planet/Sun
//		for (unsigned int j = 0; j < staticObjects.size(); ++j) {
//			// Asteroid 1, Planet, Sun 2
//			Planet* currP = dynamic_cast<Planet*>(staticObjects.at(j));
//
//
//			if (StaticSphereToSphere(currPos, radius, currP->GetPos(), currP->GetRadius())) {
//				ObjectDestroyedEvent* E = new ObjectDestroyedEvent(currA, EventObjectType::ASTEROID, i);
//				events.push_back(E);
//			}
//		}
//
//
//
//
//		// Go through all bullets 
//		for (unsigned int j = 0; j < bullets.size(); ++j) {
//			Laser* currLaser = dynamic_cast<Laser*>(bullets.at(j));
//			vec3 laserPos = currLaser->GetPos();
//			float r = currLaser->GetRadius();
//
//			if (StaticSphereToSphere(currPos, radius, laserPos, r)) {
//				if (currA->Hit()) {
//					// destroy asteroid after so many hits
//					//currA->FlagToDelete();
//					freeAsteroidList.push_back(i);
//
//
//					// destory bullet
//					//currLaser->FlagToDelete();
//					freeBulletList.push_back(j);
//
//				}
//				else {
//					// destroy only bullet
//					//currLaser->FlagToDelete();
//					freeBulletList.push_back(j);
//
//				}
//			}
//		}
//
//
//
//
//
//
//	}
//
//
//	// BUGS HERE 
//	/* Check for collision between two Asteroids */
//	if (asteroids.size() >= 2) {
//		for (unsigned int i = 0; i < asteroids.size() - 1; ++i) {
//			Asteroid* a1 = dynamic_cast<Asteroid*>(asteroids.at(i));
//			for (unsigned int j = i + 1; j < asteroids.size(); ++j) {
//				Asteroid* a2 = dynamic_cast<Asteroid*>(asteroids.at(j));
//
//				if (StaticSphereToSphere(a1->GetPos(), a1->GetRadius(), a2->GetPos(), a2->GetRadius())) {
//
//
//				}
//			}
//		}
//	}
//
//
//	/* Check for collision between bullet and planet/sun */
//	for (unsigned int i = 0; i < bullets.size(); ++i) {
//		Laser* currLaser = dynamic_cast<Laser*>(bullets.at(i));
//		vec3 laserPos = currLaser->GetPos();
//		float r = currLaser->GetRadius();
//
//		/* Planets and sun */
//		for (unsigned int j = 0; j < staticObjects.size(); ++j) {
//			Planet* currPlanet = dynamic_cast<Planet*>(staticObjects.at(j));
//
//			if (StaticSphereToSphere(laserPos, r, currPlanet->GetPos(), currPlanet->GetRadius())) {
//				currLaser->FlagToDelete();
//				freeBulletList.push_back(i);
//				break;
//			}
//		}
//	}
//
//	/* Check for collision between Player and [Asteroids, Planets, and Sun] */
//	vec3 playerPos = player->GetPos();
//	float playerR = player->GetRadius();
//	bool playerHit = false;
//
//	for (unsigned int i = 0; i < asteroids.size(); ++i) {
//		Asteroid* currA = dynamic_cast<Asteroid*>(asteroids.at(i));
//
//		if (StaticSphereToSphere(playerPos, playerR, currA->GetPos(), currA->GetRadius())) {
//			// player hit
//			player->Hit();
//			playerHit = true; // if true, we don't check second for loop
//
//			currA->FlagToDelete();
//			freeAsteroidList.push_back(i);
//
//			break;
//
//		}
//	}
//	// only check if we are not hit yet, if we are hit, no need to do more checking
//	if (!playerHit) {
//		for (unsigned int i = 0; i < staticObjects.size(); ++i) {
//			Planet* currP = dynamic_cast<Planet*>(staticObjects.at(i));
//
//			if (StaticSphereToSphere(playerPos, playerR, currP->GetPos(), currP->GetRadius())) {
//				// player hit
//				player->Hit();
//				playerHit = true;
//
//				break;
//			}
//		}
//	}
//
//
//
//}
//
//
//void ObjectContainer::Render(Shader* shader) {
//
//	/* Draw all static objects like planets */
//	for (unsigned int i = 0; i < staticObjects.size(); ++i) {
//		staticObjects.at(i)->Render();
//	}
//	/* Draw enviroment level */
//	levelEnv->Render();
//
//
//	/* Draw all dynamic objects like asteroids */
//	for (unsigned int i = 0; i < asteroids.size(); ++i) {
//		asteroids.at(i)->Render();
//	}
//
//
//	int i = 0;
//	shader->SetUniform("test", i);
//	/* Draw all bullets */
//	for (unsigned int i = 0; i < bullets.size(); ++i) {
//		bullets.at(i)->Render();
//	}
//}
#pragma once

#include "Precompiled.h"
#include "World.h"
#include "Enviroment.h"
#include "Physics.h"
#include "PowerUp.h"



using namespace std;
using namespace glm;

World::World(MeshContainter* meshes, ShaderContainer* shaders, TextureContainer* textures) {
	// Store pointers to containers so world can access them
	meshContainer_ = meshes;
	shaderContainer_ = shaders;
	textureContainer_ = textures;
}
World::~World() {
	// free all objects
	
	delete player;
	delete levelEnv;
	
	for(unsigned int i = 0; i < staticObjects.size(); ++i) {
		delete staticObjects.at(i);
	}
	for(unsigned int i = 0; i < asteroids.size(); ++i) {
		delete asteroids.at(i);
	}
	for(unsigned int i = 0; i < bullets.size(); ++i) {
		delete bullets.at(i);
	}
	for(unsigned int i = 0; i < powerups.size(); ++i) {
		delete powerups.at(i);
	}
	for(unsigned int i = 0; i < events.size(); ++i) {
		delete events.at(i);
	}
}

bool World::Initialize(glm::vec3 pos, glm::vec3 dir) {
	/* Initialize player */

	player = new Player();
	player->Initialize(pos, dir);


	/* Mesh used for Planets */
	Mesh* sphereMesh = meshContainer_->GetMeshPtr("Sphere");
	/* Basic shader program */
	Shader* basicShader = shaderContainer_->GetShaderPtr("Basic");

	/* Load world Objects */
	ifstream stream;
	string currLine;

	stream.open("Objects/Level.txt");
	if(!stream.is_open()) {
		return false;
	}

	getline(stream, currLine);
	while(stream.good()) {
		istringstream lineStream(currLine);

		string type, file;
		lineStream >> type;
		lineStream >> file;
		// Get type of object: S - Static, 
		switch(type.at(0)) {
			case 'S':
				Object* currObj = new Planet(file.c_str(), sphereMesh, basicShader);
				currObj->Initialize();
				string texName = currObj->GetTextureName();
				Texture* tex = textureContainer_->GetTexPtr(texName);
				currObj->SetTexture(tex);

				AddStaticObject(currObj);
				//AddObject(currObj);
				break;
		}
		getline(stream, currLine);
	}

	/* Create enviroment object for the level: Stars background */
	Object* level = new Enviroment();
	level->SetMesh(sphereMesh);
	level->SetShader(basicShader);
	level->SetTexture(textureContainer_->GetTexPtr("stars"));

	AddLevelObject(level);

	/* Create Asteroid objects */
	//Texture* tex = textureContainer_->GetTexPtr("asteroid1");
	for(unsigned int i = 0; i < 15; ++i) {
		SpawnAsteroid();
	}
	


	/* Create Power-up Box */
	Mesh* mesh = meshContainer_->GetMeshPtr("Rectangle");
	//Shader* basicShader = shaderContainer_->GetShaderPtr("Basic");

	vec3 powerUpPos = vec3(1000.0f, -800.0f, -450.0f);
	

	PowerUp* powerUp = new PowerUp(mesh, basicShader);
	powerUp->Initialize(powerUpPos);

	AddPowerUpObject(powerUp);
	


	return true;
}

void World::UpdatePlayer(glm::vec3 pos, glm::vec3 dir) {
	player->Update(pos, dir);
}

void World::SpawnAsteroid() {
	/* Mesh used for Planets */
	Mesh* sphereMesh = meshContainer_->GetMeshPtr("Sphere");
	/* Basic shader program */
	Shader* basicShader = shaderContainer_->GetShaderPtr("Basic");

	/* Create Asteroid objects */
	Texture* tex = textureContainer_->GetTexPtr("asteroid1");
	
	Asteroid* currAsteroid = new Asteroid("Objects/Asteroids.txt", sphereMesh, basicShader);
	currAsteroid->Initialize();
	currAsteroid->SetTexture(tex);

	AddAsteroidObject(currAsteroid);
	
}

unsigned World::AsteroidCount() {
	return asteroids.size();
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
	for(unsigned int i = 0; i < powerups.size(); ++i) {
		powerups.at(i)->Update(dt);
	}


	/* Physics */
	vec3 playerPos = player->GetPos();
	float playerR = player->GetRadius();



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
				CollisionEvent* E = new CollisionEvent(currA, currP, i, j);
				events.push_back(E);
			}
		}
		// Go through all bullets 
		for(unsigned int j = 0; j < bullets.size(); ++j) {
			Laser* currLaser = dynamic_cast<Laser*>(bullets.at(j));
			vec3 laserPos = currLaser->GetPos();
			float r = currLaser->GetRadius();

			if( StaticSphereToSphere(currPos, radius, laserPos, r) ) {
				CollisionEvent* E = new CollisionEvent(currA, currLaser, i, j);
				events.push_back(E);
			}
		}

		// Check for collision between player and asteroids
		if( StaticSphereToSphere(playerPos, playerR, currPos, radius) ) {
			CollisionEvent* E = new CollisionEvent(currA, player);
			events.push_back(E);
			break;
		}

		// Check for collision between asteroid and level
		float dist = glm::length(currA->GetPos());
		if(dist >= levelEnv->GetRadius()) {
			CollisionEvent* E = new CollisionEvent(currA, levelEnv, 0, 0); // last two args not used
			events.push_back(E);
			break;
		}

	}


	/* Check for collision between two Asteroids */
	if(asteroids.size() >= 2) {
		for(unsigned int i = 0; i < asteroids.size()-1; ++i) {
			Asteroid* a1 = dynamic_cast<Asteroid*>(asteroids.at(i));
			for(unsigned int j = i+1; j < asteroids.size(); ++j) {
				Asteroid* a2 = dynamic_cast<Asteroid*>(asteroids.at(j));

				if( StaticSphereToSphere(a1->GetPos(), a1->GetRadius(), a2->GetPos(), a2->GetRadius()) ) {
					CollisionEvent* E = new CollisionEvent(a1, a2, i, j);
					events.push_back(E);
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
				CollisionEvent* E = new CollisionEvent(currLaser, currPlanet, i, j);
				events.push_back(E);
				break;
			}
		}
	}

	
	// Check for collision between player and planets
	for(unsigned int i = 0; i < staticObjects.size(); ++i) {
		Planet* currP = dynamic_cast<Planet*>(staticObjects.at(i));

		if( StaticSphereToSphere(playerPos, playerR, currP->GetPos(), currP->GetRadius()) ) {
			CollisionEvent* E = new CollisionEvent(currP, player);
			events.push_back(E);

			break;
		}
	}

	// Check for collision between powerups and player
	for(unsigned int i = 0; i < powerups.size(); ++i) {
		PowerUp* curr = dynamic_cast<PowerUp*>(powerups.at(i));

		if(StaticSphereToSphere(curr->GetPos(), curr->GetRadius(), playerPos, playerR)) {
			CollisionEvent* E = new CollisionEvent(curr, player);
			events.push_back(E);
		}
	}
	
	

	/* Update Events */
	for(unsigned int i = 0; i < events.size(); ++i) {
		events.at(i)->Update();
	}
	events.resize(0); // empty for next frame

	/* Check if bullet or asteroid is outside of stars, if radius is more than stars radius */




	/* Dynamically spawn more asteroids every n seconds outside of world */


	/* Release all in-active objects */
	auto it = asteroids.begin();
	for(unsigned int i = 0; i < asteroids.size(); ++i) {
		if(!asteroids.at(i)->GetActive()) {
			delete asteroids.at(i);
			asteroids.erase(it+i);
		}
	}
	

	it = bullets.begin();
	for(unsigned int i = 0; i < bullets.size(); ++i) {
		if(!bullets.at(i)->GetActive()) {
			delete bullets.at(i);
			bullets.erase(it+i);
			it = bullets.begin();
		}
	}

	it = powerups.begin();
	for(unsigned int i = 0; i < powerups.size(); ++i) {
		if(!powerups.at(i)->GetActive()) {
			delete powerups.at(i);
			powerups.erase(it+i);
		}
	}
	
}

void World::Render() {

	int drawOption;


	drawOption = 4;
	currShader->SetUniform("test", drawOption);

	/* Draw all static objects like planets */
	for(unsigned int i = 0; i < staticObjects.size(); ++i) {
		// draw sun with just texture, no lighting
		if(i == 6) {
			drawOption = 1;
			currShader->SetUniform("test", drawOption);
		}
		staticObjects.at(i)->Render();
	}
	

	drawOption = 4;
	currShader->SetUniform("test", drawOption);
	/* Draw all dynamic objects like asteroids */
	for(unsigned int i = 0; i < asteroids.size(); ++i) {
		asteroids.at(i)->Render();
	}



	drawOption = 1;
	currShader->SetUniform("test", drawOption);
	/* Draw enviroment level */
	levelEnv->Render();


	
	if(player->isPowerUpActive()) {
		drawOption = 3;
	}else {
		drawOption = 0;
	}
	currShader->SetUniform("test", drawOption);
	/* Draw all bullets */
	for(unsigned int i = 0; i < bullets.size(); ++i) {
		bullets.at(i)->Render();
	}
	

	drawOption = 2;
	currShader->SetUniform("test", drawOption);
	for(unsigned int i = 0; i < powerups.size(); ++i) {
		powerups.at(i)->Render();
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
void World::AddPowerUpObject(Object* obj) {
	powerups.push_back(obj);
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

void World::PlayerFire(Camera* cam) {
	Mesh* mesh = meshContainer_->GetMeshPtr("Rectangle");
	Shader* basicShader = shaderContainer_->GetShaderPtr("Basic");

	player->Fire(&bullets, mesh, basicShader, cam); // Fire() will add bullets to the world bullet container

}
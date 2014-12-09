#pragma once

#ifndef WORLD_H
#define WORLD_H

#include "Object.h"
#include "Asteroid.h"
#include "Laser.h"
#include "Planet.h"
#include "Player.h"
#include "MeshContainer.h"
#include "ShaderContainer.h"
#include "TextureContainer.h"

#include "CollisionEvent.h"


#include "Precompiled.h"


class World {
public:

	World(MeshContainter* meshes, ShaderContainer* shaders, TextureContainer* textures);
	~World();
	
	/* Initialize Game World */
	bool Initialize(glm::vec3 pos, glm::vec3 dir);

	/* Standard Game Loop Methods */
	void Update(float dt);
	void Render();


	/* Player Methods */
	void UpdatePlayer(glm::vec3 pos, glm::vec3 dir);
	bool CheckPlayerHit();
	bool CheckPlayerAlive();
	void ResetPlayer(glm::vec3 pos, glm::vec3 dir);
	void PlayerFire(Camera* cam);

	unsigned AsteroidCount();

	/* Add Object Methods */
	void AddStaticObject(Object* obj);
	void AddAsteroidObject(Object* obj);
	void AddBulletObject(Object* obj);
	void AddLevelObject(Object* obj);
	void AddPowerUpObject(Object* obj);
	void SpawnAsteroid();

	/* Sets the shader to draw with */
	void SetCurrShader(Shader* shader);


private:

	/* Asset Containers */
	MeshContainter* meshContainer_;
	ShaderContainer* shaderContainer_;
	TextureContainer* textureContainer_;


	/* Objects */
	Object* levelEnv;						/* Stars */

	std::vector<Object*> staticObjects;		/* Planets			*/
	std::vector<Object*> asteroids;			/* Aseteroids		*/
	std::vector<Object*> bullets;			/* Laser bullets	*/
	std::vector<Object*> powerups;			/* Power-Ups		*/

	/* Events */
	std::vector<CollisionEvent*> events;

	/* Player */
	Player* player;

	/* Current shader to render with */
	Shader* currShader;

};

#endif
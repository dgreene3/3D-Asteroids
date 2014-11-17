#ifndef WORLD_H
#define WORLD_H

#include "Object.h"
#include "Asteroid.h"
#include "Laser.h"
#include "Planet.h"
#include "Player.h"

#include "Precompiled.h"


class World {
public:
	World();
	~World();
	
	void Initialize(glm::vec3 pos, glm::vec3 dir);

	void AddObject(Object* obj);
	void RemoveObject();
	void Update(float dt);
	void Render();

	void UpdatePlayer(glm::vec3 pos, glm::vec3 dir);
	bool CheckPlayerHit();
	bool CheckPlayerAlive();
	void ResetPlayer(glm::vec3 pos, glm::vec3 dir);

	void AddStaticObject(Object* obj);
	void AddAsteroidObject(Object* obj);
	void AddBulletObject(Object* obj);
	void AddLevelObject(Object* obj);

	void SetCurrShader(Shader* shader);


private:

	//std::vector<Object*> objects;


	/* Objects */
	Object* objInst; // player object
	std::vector<Object*> staticObjects; /* Planets and Sun*/
	Object* levelEnv;					/* Stars */
	std::vector<Object*> asteroids;		/* Aseteroids */
	std::vector<Object*> bullets;



	std::vector<int> freeAsteroidList;	/* holds object pointers ready to be freed */
	std::vector<int> freeBulletList; 


	/* Player */
	Player* player;


	Shader* currShader;

};

#endif
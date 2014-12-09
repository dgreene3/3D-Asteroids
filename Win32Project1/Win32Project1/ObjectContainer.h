#ifndef OBJECTCONTAINER_H
#define OBJECTCONTAINER_H


#include "Object.h"
#include "Shader.h"
#include "Player.h"

#include "CollisionEvent.h"



class ObjectContainer {
public:

	ObjectContainer();
	~ObjectContainer();


	void AddObject(Object* obj, ObjectType type);
	void Update(float dt, Player* player);
	void Render(Shader* shader);

private:

	/* Objects */
	std::vector<Object*> staticObjects;		/* Planets and Sun*/
	Object* levelEnv;						/* Stars map level */
	std::vector<Object*> asteroids;			/* Aseteroids */
	std::vector<Object*> bullets;			/* Bullets */


	/* Events */
	std::vector<CollisionEvent*> events;


	/* Freelists */
	std::vector<int> freeAsteroidList;	/* holds object pointers ready to be freed */
	std::vector<int> freeBulletList;

};


#endif

#pragma once

#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "Object.h"
#include "Player.h"



class CollisionEvent {
public:
	CollisionEvent(Object* object_, Player* player_);
	CollisionEvent(Object* object1_, Object* object2_, unsigned int id1_, unsigned int id2_);
	~CollisionEvent();

	void Update();

protected:
	Object* object1;
	Object* object2;
	Player* player;


	unsigned int id1;
	unsigned int id2;

};







#endif
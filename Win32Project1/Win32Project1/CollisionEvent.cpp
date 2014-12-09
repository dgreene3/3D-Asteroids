#include "CollisionEvent.h"





CollisionEvent::CollisionEvent(Object* object_, Player* player_)
	: object1(0), object2(0), player(0), id1(0), id2(0) {
	this->object1 = object_;
	this->player = player_;
}

CollisionEvent::CollisionEvent(Object* object1_, Object* object2_, unsigned int id1_, unsigned int id2_)
	: object1(0), object2(0), player(0), id1(0), id2(0) {
	this->object1 = object1_;
	this->object2 = object2_;

	this->id1 = id1_;
	this->id2 = id2_;
}

CollisionEvent::~CollisionEvent() {

}

void CollisionEvent::Update() {

	if(player) {
		object1->Collide(player);
		player->Collide(object1);
	}else {
		object1->Collide(object2);
		object2->Collide(object1);
	}
}
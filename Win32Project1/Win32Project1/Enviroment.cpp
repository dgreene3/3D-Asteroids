#include "Enviroment.h"

#define SIZE 1800.0f

Enviroment::Enviroment() {
	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(SIZE, SIZE, SIZE) );
	SetTransformation(scale);
}

Enviroment::~Enviroment() {

}

void Enviroment::Update(float dt) {
	/* Do nothing */
}

/* Since we are inside the sphere, if we don't disable culling, we 
   won't see anything since the winding order will be opposite of normal
   situations */
void Enviroment::Render() {
	glDisable(GL_CULL_FACE); 

	Object::Render();

	glEnable(GL_CULL_FACE);
}


float Enviroment::GetRadius()const {
	return SIZE;
}

ObjectType Enviroment::GetType()const {
	return ObjectType::LEVEL;
}


void Enviroment::Collide(Object* other) {
	// don't do anything on any collision right now
}
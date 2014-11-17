#include "Enviroment.h"


Enviroment::Enviroment() {
	glm::mat4 scale(1.0f);
	scale = glm::scale(scale, glm::vec3(1800.0f, 1800.0f, 1800.0f));
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
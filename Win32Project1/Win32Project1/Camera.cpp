#include "Camera.h"

#define TURNRADIANS 0.1963495f
#define CAM_STEP_DELTA 4.10f

using namespace glm;

const float turnRadians = (float)(3.14159 / 16); //how much to turn/rotate by, in radians.




Camera::Camera() {
	camPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	camTarget = glm::vec3(0.0f, 0.0f, -1.0f);
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	forwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	rightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	upDirection = glm::vec3(0.0f, 1.0f, 0.0f);


}

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
	camPosition = pos;
	camTarget = target;
	camUp = up;
	//add params for these too
	forwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	rightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	//init to (0, 0, -1) in world space
	theta = (float)(PI / 2);
	phi = (float)(3.0f * PI / 2);
}

Camera::~Camera() {
}

void Camera::Update() {
	// Update view matrix
	view = glm::lookAt(camPosition, camTarget, camUp);
}
mat4& Camera::GetViewMatrix() {
	return view;
}


void Camera::MoveForward() {
	camPosition = camPosition + (CAM_STEP_DELTA * forwardDirection);
	camTarget = camPosition + forwardDirection;
}
void Camera::MoveBackward() {
	camPosition = camPosition - (CAM_STEP_DELTA * forwardDirection);
	camTarget = camPosition + forwardDirection;
}

void Camera::MoveRight() {
	camPosition = camPosition + (CAM_STEP_DELTA * rightDirection);
	camTarget = camPosition + forwardDirection;
}

void Camera::MoveLeft() {
	camPosition = camPosition - (CAM_STEP_DELTA * rightDirection);
	camTarget = camPosition + forwardDirection;
}



void Camera::MoveUp() {
	camPosition = camPosition + (CAM_STEP_DELTA * glm::vec3(0.0f, 1.0f, 0.0f));
	camTarget = camPosition + forwardDirection;
}

void Camera::MoveDown() {
	camPosition = camPosition + (CAM_STEP_DELTA * glm::vec3(0.0f, -1.0f, 0.0f));
	camTarget = camPosition + forwardDirection;
}



void Camera::mouseLook(float dTheta, float dPhi) {
	theta += dTheta;
	phi += dPhi;
	if (theta <= 0.3f) { theta = 0.3f; }
	if (theta >= (PI - 0.3f)) { theta = (float)(PI - 0.3f); }


	float x, y, z;

	x = 1.0f * sinf(theta) * cosf(phi);
	y = 1.0f * cosf(theta);
	z = 1.0f * sinf(theta) * sinf(phi);

	//have new look direction vector
	//get look at target one unit in front of cam position
	forwardDirection = glm::normalize(glm::vec3(x, y, z));
	camTarget = camPosition + forwardDirection;


	rightDirection = glm::normalize(glm::cross(forwardDirection, camUp));
	upDirection = glm::normalize(glm::cross(rightDirection, forwardDirection));


}



glm::vec3 Camera::GetPosition()const {
	return camPosition;
}

glm::vec3 Camera::GetForwardDir()const {
	return forwardDirection;
}
glm::vec3 Camera::GetRightDir()const {
	return rightDirection;
}
float Camera::GetTheta()const {
	return theta;
}
float Camera::GetPhi()const {
	return phi;
}


void Camera::Reset(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
	camPosition = pos;
	camTarget = target;
	camUp = up;
	//add params for these too
	forwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	rightDirection = glm::vec3(1.0f, 0.0f, 0.0f);
	upDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	//init to (0, 0, -1) in world space
	theta = (float)(PI / 2);
	phi = (float)(3.0f * PI / 2);
}
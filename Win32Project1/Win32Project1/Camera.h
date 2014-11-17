#ifndef CAMERA_H
#define CAMERA_H

#include "Precompiled.h"


#ifndef PI
#define PI 3.141592654
#endif





class Camera {
public:
	Camera();
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);
	~Camera();

	void Reset(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

	// Update function to update view matrix 
	void Update();
	glm::mat4& GetViewMatrix();

	//Free fly functions
	void MoveForward();
	void MoveBackward();

	void MoveRight();
	void MoveLeft();


	void MoveUp();
	void MoveDown();

	
	void mouseLook(float dTheta, float dPhi); 

	glm::vec3 GetPosition()const;
	glm::vec3 GetForwardDir()const;
	glm::vec3 GetRightDir()const;
	float GetTheta()const;
	float GetPhi()const;


private:

	// view matrix
	glm::mat4 view;


	//needed for calculating view matrix
	glm::vec3 camPosition;
	glm::vec3 camTarget;
	glm::vec3 camUp;

	//needed to keep track of view direction: forward and right
	glm::vec3 forwardDirection;
	glm::vec3 rightDirection;
	glm::vec3 upDirection;


	//used for looking around, look direction vector based on spherical coords
	float theta, phi; // theta up down Y, phi left right on xz plane

};



#endif
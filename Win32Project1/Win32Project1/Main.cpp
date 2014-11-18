#pragma once
#include "Engine.h"
#include "Object.h"
#include "Physics.h"
#include "Asteroid.h"
#include "Planet.h"
#include "Laser.h"
#include "Reticle.h"

#define BULLET_SPEED 200.0f
#define PER_PIXEL_TURN_RAD 0.0025f		// Used for mouse movement

using namespace glm;

class GameEngine : public Engine {
public :
	GameEngine(HINSTANCE hInstance, int Width, int Height);
	~GameEngine();

	/* Overrides */
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	/* Mouse function overrides */
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseFire(WPARAM btnState, int x, int y);

private:
	
	POINT prevMousePos;

	Reticle* reticle;
};




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {

	GameEngine gameApp(hInstance, 800, 600);
	
	if( !gameApp.Init() )
		return 0;
	
	return gameApp.Run();
}






GameEngine::GameEngine(HINSTANCE hInstance, int Width, int Height) 
	: Engine(hInstance, Width, Height) {

	/* Init variables, matrices ... */
}
GameEngine::~GameEngine() {

}

bool GameEngine::Init() {

	if( !Engine::Init() )
		return false;

	/* Initialize all game objects */

	// Init reticle for ship
	Shader* shader = shaderContainer_.GetShaderPtr("Reticle");
	reticle = new Reticle(shader);

	/* Set any other variables like mouse position ... */



	/* Configure OpenGL options */
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	return true;
}

void GameEngine::OnResize() {
	if(!shaderReady) 
		return; // some systems call resize before shader has been loaded

	Engine::OnResize();
}

void GameEngine::UpdateScene(float dt) {

	/* Control the Camera */
	if (GetAsyncKeyState('W') & 0x8000) {
		cam->MoveForward();
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		cam->MoveBackward();
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		cam->MoveLeft();
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		cam->MoveRight();
	}
	if (GetAsyncKeyState('Q') & 0x8000) {
		cam->MoveUp();
	}
	if (GetAsyncKeyState('E') & 0x8000) {
		cam->MoveDown();
	}


	/* Mouse Movement */
	if(fpsMode) {
		POINT currMousePos;
		GetCursorPos(&currMousePos);
		ScreenToClient(MainWnd_, &currMousePos);

		// Make each pixel correspond to a quarter of a degree.
		float dx = (float) ((currMousePos.x - 400) * PER_PIXEL_TURN_RAD );
		float dy = dy = (float) ((currMousePos.y - 300) * PER_PIXEL_TURN_RAD * 1.0f);

		cam->mouseLook(dy, dx);

		// Set mouse back to middle of screen
		currMousePos.x = 400;
		currMousePos.y = 300;
		ClientToScreen(MainWnd_, &currMousePos);
		SetCursorPos(currMousePos.x, currMousePos.y);
	}
	
	/* Update player */
	world->UpdatePlayer(cam->GetPosition(), cam->GetForwardDir());



	/* Update camera and view matrix */
	cam->Update();



	/* Update world objects */
	world->Update(dt);


	/* Check for Game Over */
	bool playerHitStatus = world->CheckPlayerHit();

	/* If the player took a hit, reset position */
	if(playerHitStatus) {
		bool playerAlive = world->CheckPlayerAlive();
		if(playerAlive) {
			// Reset camera
			glm::vec3 pos(0.0f, 0.0f, 700.0f);
			glm::vec3 target(0.0f, 0.0f, 0.0f);
			glm::vec3 up(0.0f, 1.0f, 0.0f);
			cam->Reset(pos, target, up);
			// Reset player back to starting position
			world->ResetPlayer(cam->GetPosition(), cam->GetForwardDir());
		}else {
			// Game Over: could do red screen for a few sec, then quit
			PostQuitMessage(0);
		}
	}
}


void GameEngine::DrawScene() {
	/* Clear screen */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	/* Enable Shader */
	currShaderProgram = shaderContainer_.GetShaderPtr("Basic");
	currShaderProgram->SetShader();


	/* Set any uniforms that are per frame */
	int i = 1;
	currShaderProgram->SetUniform("test", i);

	/* Set view uniform */
	glm::mat4 viewMatrix = cam->GetViewMatrix();
	currShaderProgram->SetUniform("viewMatrix", viewMatrix);
	

	// DRAW OBJECTS 
	world->SetCurrShader(currShaderProgram);
	world->Render();


	currShaderProgram->ClearShader();






	// Draw reticle on screen
	reticle->Render();
	
	/* Game loop will call SwapBuffers() */
}







/* MOUSE MOVEMENT */
void GameEngine::OnMouseDown(WPARAM btnState, int x, int y) {
	prevMousePos.x = x;
	prevMousePos.y = y;

	SetCapture(MainWnd_);
}
void GameEngine::OnMouseUp(WPARAM btnState, int x, int y) {
	ReleaseCapture();
}
void GameEngine::OnMouseMove(WPARAM btnState, int x, int y) {
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = (float) ((x - prevMousePos.x) * PER_PIXEL_TURN_RAD );

		float dy = dy = (float) ((y - prevMousePos.y) * PER_PIXEL_TURN_RAD * 1.0f);

		cam->mouseLook(dy, dx);
	}

	prevMousePos.x = x;
	prevMousePos.y = y;
}

void GameEngine::OnMouseFire(WPARAM btnState, int x, int y) {
	Mesh* mesh = meshContainer_.GetMeshPtr("Rectangle");
	Shader* basicShader = shaderContainer_.GetShaderPtr("Basic");

	// Fire bullet/laser

	// Get ship position "cam position"
	vec3 pos = cam->GetPosition();
	vec3 right = cam->GetRightDir();
	vec3 bulletPos = pos + (0.5f * right);
	bulletPos.y -= 0.5f;

	vec3 lookDir = cam->GetForwardDir();
	vec3 target = lookDir * 100.0f + pos; 
	vec3 targetVector = target - bulletPos;
	targetVector = glm::normalize(targetVector);
	targetVector *= BULLET_SPEED;

	float theta = cam->GetTheta();
	float phi = cam->GetPhi();

	Laser* newBullet = new Laser(mesh, basicShader);
	newBullet->Initialize(bulletPos, targetVector, theta, phi);

	world->AddBulletObject(newBullet);
}




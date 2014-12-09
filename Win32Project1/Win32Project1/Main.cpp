/* Start Header -------------------------------------------------------

File Name: Main.cpp
Purpose: Contains the entry point for the program.
Creates GameEngine, initializes and starts the game loop.

Language: <C++ Visual Studio >
Platform: Windows 7/8. Requires Nvidia GPU and CUDA

Project: <d.greene, CS 529, Final Game Project. 

Author: <Dustin Greene, d.greene, 60002414>

Creation date: October 2, 2014

- End Header --------------------------------------------------------*/




#pragma once
#include "Engine.h"
#include "Object.h"
#include "Physics.h"
#include "Asteroid.h"
#include "Planet.h"
#include "Laser.h"
#include "Reticle.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>


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
	void DrawWorld();

	/* Mouse function overrides */
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseFire(WPARAM btnState, int x, int y);

	void DebugMode();
	void MenuMode();
	void ChangeEffect();

private:
	
	POINT prevMousePos;
	Reticle* reticle;

	bool debugMode;
	bool menuMode;
	bool gameOver;
	bool gameWin;

	bool shaderLoaded;
};




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {


#ifdef _DEBUG
	/* Allocate Console for Debug */
	AllocConsole();
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;
	printf("3D Asteroids Game by Dustin Greene.\n");

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
#endif



	/* Create Game Engine */
	GameEngine gameApp(hInstance, 800, 600);
	
	/* Initialize Game Engine*/
	if (!gameApp.Init()) {
		printf("Error: Press enter to continue.\n");
		getchar();
		return 0;
	}
	
	/* Call Game Loop */
	return gameApp.Run();
}






GameEngine::GameEngine(HINSTANCE hInstance, int Width, int Height) 
	: Engine(hInstance, Width, Height) {

	/* Init variables, matrices ... */
		debugMode = false;
		menuMode = false;
		gameOver = false;
		gameWin = false;
		shaderLoaded = false;
}
GameEngine::~GameEngine() {
	delete reticle;
}


bool GameEngine::Init() {

	if (!Engine::Init()) {
		return false;
	}
	shaderLoaded = true;	// At this point, shaders will all be loaded


	/* Initialize all game objects */

	// Init targeting reticle
	Shader* shader = shaderContainer_.GetShaderPtr("Reticle");
	reticle = new Reticle(shader);


	/* Set any other variables like mouse position ... */



	/* Configure OpenGL options */
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	return true;
}

void GameEngine::OnResize() {
	if(!shaderLoaded) 
		return; // Win 8 laptop crashes without this check for some reason

	Engine::OnResize();
}


void GameEngine::DebugMode() {
	/* Debug Mode */
	debugMode = !debugMode;
	if(debugMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void GameEngine::MenuMode() {
	menuMode = !menuMode;
	if(menuMode) {
		timer.Stop();
	}else {
		timer.Start();
	}
}


void GameEngine::UpdateScene(float dt) {
	if(Paused) {
		POINT setMouseToMiddle;
		setMouseToMiddle.x = 400;
		setMouseToMiddle.y = 300;
		ClientToScreen(MainWnd_, &setMouseToMiddle);
		SetCursorPos(setMouseToMiddle.x, setMouseToMiddle.y);
		return; // don't do any game updates
	}

	/* Control the Camera */
	if (GetAsyncKeyState('W') & 0x8000) {
		cam->MoveForward();
	}



	/*	Allows only forward movement, more like a ship.
		Uncomment this section if you want to test by moving around 
		freely in the world.
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
	*/



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

	/* Update camera and view matrix */
	cam->Update();


	
	/* Update player */
	world->UpdatePlayer(cam->GetPosition(), cam->GetForwardDir());


	/* Update world objects */
	world->Update(dt);


	/* Check for Game won condition */
	if (world->AsteroidCount() == 0) {
		gameWin = true;
	}


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
			menuMode = false;
			gameOver = true;
			//PostQuitMessage(0);
		}
	}
}

void GameEngine::DrawWorld() {
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
}


void GameEngine::DrawScene() {

	if (menuMode) {
		quad->SetTexture(textureContainer_.GetTexPtr("Menu"));
		quad->Render();
	}
	else if (debugMode) {
		DrawWorld();
	}
	else if (gameOver) {
		menuMode = false;
		quad->SetTexture(textureContainer_.GetTexPtr("GameOver"));
		quad->Render();
	}
	else if (gameWin) {
		quad->SetTexture(textureContainer_.GetTexPtr("GameWin"));
		quad->Render();
	}
	else {
		/* Bind FBO before drawing sceen */
		postProcess->Bind();

		/* Draw Game World to FBO */
		DrawWorld();

		/* Unbind FBO */
		postProcess->UnBind();

		/* Post Process Draw */
		postProcess->Draw();
	}

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

	// Fire bullet
	world->PlayerFire(cam);

}


void GameEngine::ChangeEffect() {
	postProcess->ChangeEffect();
}

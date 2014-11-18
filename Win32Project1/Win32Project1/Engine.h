#ifndef ENGINE_H
#define ENGINE_H

#include "Precompiled.h"
#include "GraphicsGL.h"

#include "Object.h"
#include "MeshContainer.h"
#include "ShaderContainer.h"
#include "TextureContainer.h"
#include "Camera.h"
#include "Time.h"
#include "Enviroment.h"
#include "World.h"
#include "Player.h"



/* The engine manages all the systems in the game: Graphics, Audio, Physics, AI.
** It initializes, updates, sends messages between systems and releases all 
** resources */



class Engine {
public:
	Engine(HINSTANCE hInstance, int Width, int Height);
	virtual ~Engine();

	HINSTANCE AppInst()const;
	HWND MainWnd()const;
	float AspectRatio()const;

	int Run();

	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0; // pure virtual function
	virtual void DrawScene() = 0;



	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);




	virtual void OnMouseDown(WPARAM btnState, int x, int y){ }
	virtual void OnMouseUp(WPARAM btnState, int x, int y)  { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y){ }
	virtual void OnMouseFire(WPARAM btnState, int x, int y) { }

		
protected:
	bool InitMainWindow();
	bool InitOpenGL();

	bool InitShaders(); /* this will take a file name */
	bool InitMeshes();
	bool InitTextures();
	bool InitObjectInsts();

	void CalculateFPS();

protected:
	HINSTANCE AppInst_;
	HWND MainWnd_;
	bool Paused;
	bool WindowMin;
	bool WindowMax;
	bool WindowResizing;

	POINT middleScreen;

	// Timer
	Time timer;

	// OpenGL data
	HDC dc; /* Device Context */
	HGLRC rc;

	// Window data
	unsigned int WindowWidth;
	unsigned int WindowHeight;

	/* Current Shader to use for Rendering */
	Shader* currShaderProgram;


	/* Assets */
	MeshContainter meshContainer_;
	ShaderContainer shaderContainer_;
	TextureContainer textureContainer_;


	/* Objects */
	World* world;


	/* Camera */
	Camera* cam;
	bool fpsMode;



	/* View Matrix */
	glm::mat4 viewMatrix;

	/* Proj Matrix */
	glm::mat4 projMatrix;
	float nearZ;
	float farZ;
	float fFrustumScale;

	bool shaderReady;
};




#endif

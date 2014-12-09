#pragma once

#include "Precompiled.h"

#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "Planet.h"
#include "Engine.h"
#include "Asteroid.h"
#include "Laser.h"
#include "CudaMesh.h"
#include "NormalMesh.h"

using namespace std;


extern Engine* engine = 0;




LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return engine->MsgProc(hWnd, msg, wParam, lParam);
}

Engine::Engine(HINSTANCE hInstance, int Width, int Height) : WindowWidth(Width), WindowHeight(Height), AppInst_(hInstance) {
		engine = this;
}
Engine::~Engine() {
	/* Release objects */

	/* Release World */
	delete world;

	/* Release camera*/
	delete cam;


	delete quad;

}

float Engine::AspectRatio()const {
	return static_cast<float>(WindowWidth / WindowHeight);
}



bool Engine::Init() {
	printf("Initializing Game.\n");
	bool quit = false;

	/* Init Camera */
	glm::vec3 pos(0.0f, 0.0f, 1600.0f);
	glm::vec3 target(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	cam = new Camera(pos, target, up);


	/* Initialize Systems and Resources */
	if(!InitMainWindow())
		quit = true;
	if(!InitOpenGL())
		quit = true;
	if(!InitShaders()) 
		quit = true;
	if(!InitMeshes())
		quit = true;
	if(!InitTextures())
		quit = true;
	if(!InitObjectInsts())
		quit = true;



	/* Init Mouse for FPS movement with no clicking */
	/* This is used for debugging */
	fpsMode = true;
	if(fpsMode) {
		middleScreen.x = 400;
		middleScreen.y = 300;
		ClientToScreen(MainWnd_, &middleScreen);
		SetCursorPos(middleScreen.x, middleScreen.y);
		ShowCursor(false);
	}
	if(quit) 
		return false;
	else
		return true;
}

bool Engine::InitMainWindow() {
	WNDCLASS cls;

	ZeroMemory(&cls, sizeof(cls));
	cls.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	cls.lpfnWndProc = MainWndProc;
	cls.hInstance = ::GetModuleHandle(NULL);
	cls.lpszClassName = TEXT("OPENGL");

	RegisterClass(&cls);

	MainWnd_ = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
									TEXT("OPENGL"),
									TEXT("CS 529 Game Engine OpenGL"),
									WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
									0, 0,
								    WindowWidth, WindowHeight,
								    NULL,
									NULL,
									AppInst_,
									NULL);

	dc = ::GetDC(MainWnd_);

	PIXELFORMATDESCRIPTOR pfd;

	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize               = sizeof(pfd);
	pfd.nVersion            = 1;
	pfd.dwFlags             = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
	pfd.iPixelType          = PFD_TYPE_RGBA;
	pfd.cColorBits          = 24;
	pfd.cRedBits            = 8;
	pfd.cGreenBits          = 8;
	pfd.cBlueBits           = 8;
	pfd.cDepthBits          = 32;
	pfd.cStencilBits		= 8; // stencil buffer 

	int iPixelFormat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, iPixelFormat, &pfd);

	rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	return true;
}

bool Engine::InitOpenGL() {
	//initialzie glew
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
		return false;
	}
	return true;
}

bool Engine::InitTextures() {

	/* Initialize DevIL */
	ilInit();

	/* Load Sun Texture */
	Texture* tex = new Texture("sun", "Textures/sun.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "sun");

	/* Load Planet Textures */
	tex = new Texture("mercury", "Textures/mercury.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "mercury");

	tex = new Texture("venus", "Textures/venus.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "venus");

	tex = new Texture("earth", "Textures/earth.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "earth");

	tex = new Texture("mars", "Textures/mars.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "mars");

	tex = new Texture("jupiter", "Textures/jupiter.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "jupiter");

	tex = new Texture("saturn", "Textures/saturn.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "saturn");

	/* Load Level texture used for the world */
	tex = new Texture("stars", "Textures/stars.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "stars");


	/* Load Asteroid texture */
	tex = new Texture("asteroid1", "Textures/asteroid1.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "asteroid1");

	/*tex = new Texture("asteroid2", "Textures/asteroid2.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "asteroid2");*/

	/* Load Menu Texture */
	tex = new Texture("Menu", "Textures/menu.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "Menu");

	/* Load Game Over Screen Texture */
	tex = new Texture("GameOver", "Textures/gameover.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "GameOver");

	/* Load Game Win Screen Texture */
	tex = new Texture("GameWin", "Textures/gamewin.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "GameWin");

	
	return true;
}

bool Engine::InitShaders() {
	/* Initialize Post Process */
	glm::ivec2 windowSize(800, 600); // Bug in window size
	postProcess = new PostProcess();
	if (!postProcess->Initialize(windowSize))
		return false;



	/* Create shader */
	Shader* shader = new Shader("Shaders/basic.vert", "Shaders/basic.frag");
	if(!shader->Initialize()) {
		return false;
	}
	/* Set Proj Uniform after shader is created. Only changes on window resize */
	shader->AddUniform("projMatrix", UniformType::MATRIX4_U);

	/* Use shader to set uniforms */
	shader->SetShader();

	/* Init Proj Matrix */
	nearZ = 0.1f;
	farZ = 5000.0f;
	fFrustumScale = CalcFrustumScale(45.0f);
	projMatrix = glm::mat4(0.0f);
	projMatrix = glm::perspective(45.0f, (float)(WindowWidth / WindowHeight), nearZ, farZ);
	projMatrix[0].x = fFrustumScale * (WindowHeight / (float)WindowWidth);
	projMatrix[1].y = fFrustumScale;
	shader->SetUniform("projMatrix", projMatrix);

	/* Init texture uniform */
	shader->AddUniform("tex", UniformType::SAMPLER2D_U);
	shader->SetUniform("tex", 0);

	/* Stop using shader after setting uniforms */
	shader->ClearShader();
	


	/* Add uniforms */
	shader->AddUniform("test", UniformType::INT_U);
	shader->AddUniform("worldMatrix", UniformType::MATRIX4_U);
	shader->AddUniform("viewMatrix", UniformType::MATRIX4_U);


	/* Add shader to container */
	shaderContainer_.AddShaderProgram(shader, "Basic");


	shader = new Shader("Shaders/Reticle.vert", "Shaders/Reticle.frag");
	if(!shader->Initialize()) {
		return false;
	}

	shaderContainer_.AddShaderProgram(shader, "Reticle");


	shader = new Shader("Shaders/Menu.vert", "Shaders/Menu.frag");
	if(!shader->Initialize()) {
		return false;
	}

	shader->AddUniform("tex", UniformType::SAMPLER2D_U);

	// Do the set/clear inside setUniform 
	shader->SetShader();
	shader->SetUniform("tex", 0);
	shader->ClearShader();

	shaderContainer_.AddShaderProgram(shader, "Menu");


	return true;
}

bool Engine::InitMeshes() {
	// Create one Sphere mesh that all planets will use
	CudaMesh* mesh = new CudaMesh("Sphere", 30, 30); /* Max thread limit per block is 1024 */
	mesh->MakeSphere();
	meshContainer_.AddMesh(mesh);


	//mesh = new Mesh("Asteroid", 2, 30, 30);
	/* Modify sphere to be asteroid like */
	//meshContainer_.AddMesh(mesh);


	/* Create laser mesh Rectangular cube */
	NormalMesh* mesh2 = new NormalMesh("Rectangle");
	mesh2->MakeBox();
	meshContainer_.AddMesh(mesh2);

	mesh2 = new NormalMesh("Quad");
	mesh2->MakeQuad();
	meshContainer_.AddMesh(mesh2);


	return true;
}

bool Engine::InitObjectInsts() {

	/* Create world and initialize with objects */
	world = new World(&meshContainer_, &shaderContainer_, &textureContainer_);
	if( !world->Initialize(cam->GetPosition(), cam->GetForwardDir()) ) {
		return false;
	}

	/* Create quad */
	Mesh* mesh = meshContainer_.GetMeshPtr("Quad");
	Shader* shader = shaderContainer_.GetShaderPtr("Menu");
	Texture* texture = textureContainer_.GetTexPtr("Menu");
	quad = new Quad(shader, texture, mesh);
	quad->Initialize();


	return true;
}



int Engine::Run() {
	MSG msg = {0};

	/* Timer reset */
	timer.Reset();
	Paused = false;

	while(msg.message != WM_QUIT) {
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else {
		
			/* Timer tick */
			timer.Tick();
			float dt = timer.DeltaTime(); /* Get delta time from last frame */
			
			CalculateFPS();
			UpdateScene(dt);
			DrawScene();

			SwapBuffers(dc);
		
		}

	}
	return true;
}
	

LRESULT Engine::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
		case WM_SIZE:
			WindowWidth = LOWORD(lParam);
			WindowHeight = HIWORD(lParam);
			OnResize();
			return 0;
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE) {
				PostQuitMessage(0);
				return 0;
			}else if(wParam == 'P') {
				Paused = !Paused;
				if(Paused) {
					timer.Stop();
				}else {
					timer.Start();
				}
			}else if(wParam == 'N') {
				DebugMode();
			}else if(wParam == 'M') {
				MenuMode();
			}
			else if (wParam == 'O') {
				ChangeEffect();
			}
			break;

		case WM_LBUTTONDOWN:
			OnMouseFire(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			break;

		/*case WM_RBUTTONDOWN:
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			break;
		case WM_RBUTTONUP:
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			break;*/


		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
		case WM_QUIT:
			PostQuitMessage(0);
			return 0;
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Engine::OnResize() {

	/* Resize any frame buffer objects */

	/* Update projection matrix */
	projMatrix[0].x = fFrustumScale * (WindowHeight / (float)WindowWidth);
	projMatrix[1].y = fFrustumScale;

	/* Update every shaders proj matrix uniform */
	currShaderProgram->SetShader();
	currShaderProgram->SetUniform("projMatrix", projMatrix);	// Only one shader right now for testing, would
	currShaderProgram->ClearShader();							// have to loop through all and make change

	glViewport(0, 0, 800, 600);

	postProcess->Resize(glm::ivec2(800, 600));
}


void Engine::CalculateFPS() {
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (timer.TotalTime() - timeElapsed) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		//this is how we write text to the window
		std::wostringstream outs;   
		outs.precision(6);
		outs << "CS 529 Game Engine OpenGL" << L"    "
			 << L"FPS: " << fps << L"    " 
			 << L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(MainWnd_, outs.str().c_str());
		
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

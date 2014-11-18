#pragma once

#include "Precompiled.h"

#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "Planet.h"
#include "Engine.h"
#include "Asteroid.h"
#include "Laser.h"

using namespace std;


extern Engine* engine = 0;




LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return engine->MsgProc(hWnd, msg, wParam, lParam);
}

Engine::Engine(HINSTANCE hInstance, int Width, int Height) : 
	WindowWidth(Width), WindowHeight(Height), AppInst_(hInstance) {
		engine = this;
		shaderReady = false;
}
Engine::~Engine() {
	/* Release objects */
	/*for(unsigned int i = 0; i < staticObjects.size(); ++i) {
		Object* curr = staticObjects[i];
		delete curr;
	}*/

	/* Release World */
	delete world;

	/* Release camera*/
	delete cam;
}

float Engine::AspectRatio()const {
	return static_cast<float>(WindowWidth / WindowHeight);
}



bool Engine::Init() {
	/* Init Camera */
	glm::vec3 pos(0.0f, 0.0f, 1700.0f);
	glm::vec3 target(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	cam = new Camera(pos, target, up);


	/* Initialize Systems and Resources */
	if(!InitMainWindow())
		return false;
	if(!InitOpenGL())
		return false;
	if(!InitShaders()) 
		return false;
	if(!InitMeshes())
		return false;
	if(!InitTextures())
		return false;
	if(!InitObjectInsts())
		return false;

	shaderReady = true;

	/* Init Mouse for FPS movement with no clicking */
	fpsMode = true;
	if(fpsMode) {
		middleScreen.x = 400;
		middleScreen.y = 300;
		ClientToScreen(MainWnd_, &middleScreen);
		SetCursorPos(middleScreen.x, middleScreen.y);
		ShowCursor(false);
	}
	
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

	tex = new Texture("asteroid2", "Textures/asteroid2.jpg");
	tex->Init();
	textureContainer_.AddTexture(tex, "asteroid2");

	
	return true;
}

bool Engine::InitShaders() {

	/* Create shader */
	Shader* shader = new Shader("Shaders/basic.vert", "Shaders/basic.frag");

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
	shaderContainer_.AddShaderProgram(shader, "Reticle");


	return true;
}

bool Engine::InitMeshes() {
	// Create one Sphere mesh that all planets will use
	Mesh* mesh = new Mesh("Sphere", 1, 30, 30); /* Max thread limit per block is 1024 */
	meshContainer_.AddMesh(mesh);


	//mesh = new Mesh("Asteroid", 2, 30, 30);
	/* Modify sphere to be asteroid like */
	//meshContainer_.AddMesh(mesh);


	/* Create laser mesh Rectangular cube */
	mesh = new Mesh("Rectangle");
	meshContainer_.AddMesh(mesh);


	return true;
}

bool Engine::InitObjectInsts() {

	world = new World();
	world->Initialize(cam->GetPosition(), cam->GetForwardDir());


	/* Mesh used for Planets */
	Mesh* sphereMesh = meshContainer_.GetMeshPtr("Sphere");
	/* Basic shader program */
	Shader* basicShader = shaderContainer_.GetShaderPtr("Basic");

	
	/* Create Planet Objects */
	std::vector<string> planets, textures;
	planets.push_back("Objects/Mercury.txt");
	planets.push_back("Objects/Venus.txt");
	planets.push_back("Objects/Earth.txt");
	planets.push_back("Objects/Mars.txt");
	planets.push_back("Objects/Jupiter.txt");
	planets.push_back("Objects/Saturn.txt");
	planets.push_back("Objects/Sun.txt");

	textures.push_back("mercury");
	textures.push_back("venus");
	textures.push_back("earth");
	textures.push_back("mars");
	textures.push_back("jupiter");
	textures.push_back("saturn");
	textures.push_back("sun");

	/* Create planets: serialize object based on object file */
	for(unsigned int i = 0; i < planets.size(); ++i) {
		Object* currObj = new Planet(planets.at(i), sphereMesh, basicShader);
		currObj->Initialize();
		Texture* tex = textureContainer_.GetTexPtr(textures.at(i));
		currObj->SetTexture(tex);

		world->AddStaticObject(currObj);

	
	}

	

	/* Create enviroment object for the level: Stars background */
	Object* level = new Enviroment();
	level->SetMesh(sphereMesh);
	level->SetShader(basicShader);
	level->SetTexture(textureContainer_.GetTexPtr("stars"));

	world->AddLevelObject(level);


	/* Create Asteroid objects */
	Texture* tex = textureContainer_.GetTexPtr("asteroid1");
	for(unsigned int i = 0; i < 10; ++i) {
		Asteroid* currAsteroid = new Asteroid("Objects/Asteroids.txt", sphereMesh, basicShader);
		currAsteroid->Initialize();
		currAsteroid->SetTexture(tex);

		world->AddAsteroidObject(currAsteroid);
	}






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
			}else if(wParam == VK_F1) {
				Paused = !Paused; // Turn pause on/off
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			break;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			break;

		case WM_RBUTTONDOWN:
			OnMouseFire(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
			break;
		case WM_RBUTTONUP:
			return 0;
			break;


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

	glViewport(0, 0, WindowWidth, WindowHeight);
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

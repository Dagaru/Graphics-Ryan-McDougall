#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "SkyBox.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void linkFogShader();
	void linkRimLighting();
	void linkGeo();
	void linkReflect();
	void linkCustomShader();

	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	bool resetTimer;

	Display _gameDisplay;
	GameState _gameState;

	Mesh objectMesh1;
	Mesh objectMesh2;
	Mesh objectMesh3;

	Camera myCamera;

	Shader fogShader;
	Shader shaderCustom;
	Shader rimShader;
	Shader geoShader;
	Shader shaderSkybox;
	Shader reflect;

	Skybox skybox;

	vector<std::string> faces;
	
	float objSpeed;
	float timeDeltaTime;
};


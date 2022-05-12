#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transformModel1;
Transform transformModel2;
Transform transfromModel3;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;

	//creates a new display that will be used here.
	Display* _gameDisplay = new Display();

	Shader fogShader();
	Shader shaderCustom();
	Shader rimShader();
	Shader geoShader();
	Shader reflect();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	//if the game is running it will intilize the systems method and run the game loop.
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	//intializes the game display.
	_gameDisplay.initDisplay(); 

	resetTimer = false;

	//sets default timer for objspeed & timeDeltaTime
	objSpeed = 1.0f;
	timeDeltaTime = 1.0f;
	
	//grabs the three different models and assigns them to the lablled object meshes.
	objectMesh1.loadModel("..\\res\\lamp.obj");
	objectMesh2.loadModel("..\\res\\monkey3.obj");
	objectMesh3.loadModel("..\\res\\Sword.obj");

	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); 
	shaderCustom.init("..\\res\\shaderCustom.vert", "..\\res\\shaderCustom.frag");
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	reflect.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");

	geoShader.initGeo();

	//sets the initial position, fov, width/height(Aspect Ratio), and the near/far clip.
	myCamera.initCamera(glm::vec3(0, 0, -5), 71, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1010.0f);

	//using a vector called faces to grab each of the images used for the skybox.
	vector<std::string> faces
	{
		"..\\res\\skybox\\posx.jpg",
		"..\\res\\skybox\\negx.jpg",
		"..\\res\\skybox\\posy.jpg",
		"..\\res\\skybox\\negy.jpg",
		"..\\res\\skybox\\posz.jpg",
		"..\\res\\skybox\\negz.jpg"
	};

	skybox.init(faces);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();

		//checks for sphere collision on each of the models during game loop.
		collision(objectMesh1.getSpherePos(), objectMesh1.getSphereRadius(), objectMesh1.getSpherePos(), objectMesh1.getSphereRadius());
		collision(objectMesh2.getSpherePos(), objectMesh2.getSphereRadius(), objectMesh2.getSpherePos(), objectMesh2.getSphereRadius());
		collision(objectMesh3.getSpherePos(), objectMesh3.getSphereRadius(), objectMesh3.getSpherePos(), objectMesh3.getSphereRadius());
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}

//this checks if the object has collided with an other by using distance aswell as setting the value of that distance.
bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// this link fog shader sets the data in terms of fog max/min distance.
void MainGame::linkFogShader()
{
	//Maximium Distane which how far the fog will end from the camera.
	fogShader.setFloat("maxDist", 26.0f);
	//Minimium Distance which is how close the fog starts from the camera.
	fogShader.setFloat("minDist", 0.1f);
	//Colour of the fog using vectors.
	fogShader.setVec3("fogColor", glm::vec3(1.0f, 1.0f, 1.0f));
}


void MainGame::linkReflect()
{
	reflect.setMat4("model", transformModel1.GetModel());
	reflect.setMat4("view", myCamera.getView());
	reflect.setMat4("projection", myCamera.getProjection());

	reflect.setInt("skybox", 0);
	reflect.setVec3("cameraPos", myCamera.getPos());
}

// this links all the uniforms for the vert/frag shader.
void MainGame::linkRimLighting()
{
	glm::vec3 camDir;

	camDir = objectMesh1.getSpherePos() - myCamera.getPos();
	camDir = glm::normalize(camDir);

	rimShader.setMat4("u_pm", myCamera.getProjection());
	rimShader.setMat4("u_vm", myCamera.getView());

	rimShader.setMat4("model", transformModel1.GetModel());
	rimShader.setMat4("model", transformModel2.GetModel());
	rimShader.setMat4("model", transfromModel3.GetModel());

	rimShader.setMat4("view", myCamera.getView());
	rimShader.setVec3("lightDir", glm::vec3(1.1f, 1.1f, 1.1f));
}

//links the uniforms from the vert/frag files to the Main Game class.
void MainGame::linkCustomShader()
{
	// assigns the right data to the MVP uniforms for the vert file.
	shaderCustom.setMat4("model", transfromModel3.GetModel());
	shaderCustom.setMat4("view", myCamera.getView());
	shaderCustom.setMat4("projection", myCamera.getProjection());

	// assigns the appropriate data to the objec/light color asweel as the veiw and light position.
	shaderCustom.setVec3("ambientColor", 0.8f, 0.4f, 1.0f);
	shaderCustom.setVec3("lightColor", timeDeltaTime, 0.5f, 0.5f);

	shaderCustom.setVec3("lightPos", *transfromModel3.GetPos() + glm::vec3(0.1f, -0.45f, 0.25f));
	shaderCustom.setVec3("viewPos", myCamera.getPos());
}

void MainGame::linkGeo()
{
	//this detemins how fast the shader will exapand back and forth from it's orginal state.
	geoShader.setFloat("time", objSpeed);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour

	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture


	// the logic below is just a timer that increases and decreases based on a min/max value.
	if (timeDeltaTime >= 2.0f)
	{
		resetTimer = true;
		std::cout << timeDeltaTime << std::endl;
	}

	if (timeDeltaTime <= 0)
	{
		resetTimer = false;
		std::cout << timeDeltaTime << std::endl;
	}

	if (!resetTimer)
	{
	    timeDeltaTime = timeDeltaTime + 0.01f;
	}
	else
	{
		timeDeltaTime = timeDeltaTime - 0.01f;
	}


	objSpeed = objSpeed + 0.01f / 2.0f;
	
	//Fisrt Object =====================================================
	//These three lines declare the position, rotation and scale of the object.
	transformModel1.SetPos(glm::vec3(0, 1,  -2));
	transformModel1.SetRot(glm::vec3(0, objSpeed * 4, 0.2));
	transformModel1.SetScale(glm::vec3(0.3, 0.3, 0.3));

	//these link & bind the reflect shader to the declared object.
	reflect.Bind();
	linkReflect();
	reflect.Update(transformModel1, myCamera);

	objectMesh1.draw();
	objectMesh1.updateSphereData(*transformModel1.GetPos(), 0.62f);

	//Second Object =====================================================
	//These three lines declare the position, rotation and scale of the object.
	transformModel2.SetPos(glm::vec3(sinf(objSpeed), -1.0, -2));
	transformModel2.SetRot(glm::vec3(0.2, objSpeed * 3, 0.2));
	transformModel2.SetScale(glm::vec3(0.7, 0.7, 0.7));

	//These link & Bind the geo shader to the declared object. 
	geoShader.Bind();
	linkGeo();
	geoShader.Update(transformModel2, myCamera);
	texture1.Bind(0);

	objectMesh2.draw();
	objectMesh2.updateSphereData(*transformModel2.GetPos(), 0.65f);

	//Third Object =====================================================
	//These three lines declare the position, rotation and scale of the object.
	transfromModel3.SetPos(glm::vec3(-3, 0.1, -2));
	transfromModel3.SetRot(glm::vec3(3, -objSpeed * 3, 0.2));
	transfromModel3.SetScale(glm::vec3(0.03f, 0.03f , 0.03f));

	//These link and bind the customShader to the declared object.
	shaderCustom.Bind();
	linkCustomShader();
	shaderCustom.Update(transfromModel3, myCamera);

	objectMesh3.draw();
	objectMesh3.updateSphereData(*transfromModel3.GetPos(), 0.60f);

	skybox.draw(&myCamera);	

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();


	_gameDisplay.swapBuffer();	
} 
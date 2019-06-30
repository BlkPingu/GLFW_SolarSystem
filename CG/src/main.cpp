#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <MMSystem.h>
// GLEW

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <gl/freeglut.h>
// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Planet.h"


// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <soil.h>
#include <math.h>
#include <list>
#include "Texture.h"


// Properties
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 300.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat UniverseSpeed = 1.0f;


int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders

	Shader Textshader("./res/shaders/modelOrbit.vs", "./res/shaders/modelOrbit.frag");
	Shader shader("./res/shaders/cube.vs", "./res/shaders/cube.frag");
	Shader skyboxShader("./res/shaders/skybox.vs", "./res/shaders/skybox.frag");
  Shader orbitShader("./res/shaders/modelOrbit.vs", "./res/shaders/modelOrbit.frag");
  Shader starShader("./res/shaders/modelStar.vs", "./res/shaders/modelStar.frag");
  Shader planetShader("./res/shaders/modelLoading.vs", "./res/shaders/modelLoading.frag");


	// Load models
	//Model ourModel( "../res/models/nanosuit/nanosuit.obj");
	GLfloat cubeVertices[] =
	{
		// Positions          // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// Setup cube VAO
	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);



	// Load textures
	GLuint cubeTexture = TextureLoading::LoadTexture("../res/textures/skybox/right.jpg");

	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("../res/textures/skybox/right.jpg");
	faces.push_back("../res/textures/skybox/left.jpg");
	faces.push_back("../res/textures/skybox/top.jpg");
	faces.push_back("../res/textures/skybox/bottom.jpg");
	faces.push_back("../res/textures/skybox/back.jpg");
	faces.push_back("../res/textures/skybox/front.jpg");
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

    Model orbitModel("../res/models/planets/orbit2.obj");

	Model sunText(		"../res/Texts/Sonne.obj");
	Model mercuryText(	"../res/Texts/mercury.obj");
	Model venusText(	"../res/Texts/Venus.obj");
	Model earthText(	"../res/Texts/earth.obj");
	Model MarsText(		"../res/Texts/Mars.obj");
	Model jupityerText(	"../res/Texts/Jupyter.obj");
	Model neptunText(	"../res/Texts/Neptun.obj");
	Model uranusText(	"../res/Texts/Uranus.obj");
	Model saturnText(	"../res/Texts/Saturn.obj");
	Model lunaText(		"../res/Texts/Luna.obj");
    
    //planets
	Model earthModel("../res/models/planets/earth/earth.obj");
	Model jupiterModel("../res/models/planets/jupiter/jupiter1.obj");
	Model marsModel("../res/models/planets/mars/Mars_normal-strong.obj");
	Model mercuryModel("../res/models/planets/mercury/mercury.obj");
	Model neptuneModel("../res/models/planets/neptune/neptune.obj");
	Model saturnModel("../res/models/planets/saturn/saturn3.obj");
	Model uranusModel("../res/models/planets/uranus/uranus.obj");
	Model venusModel("../res/models/planets/venus/venus.obj");
    
    //sun
    Model sunModel("../res/models/planets/Sun/sun2.obj");
    
    //moons
    Model moonModel("../res/models/planets/moon/moon1.obj");


	

	//Model sunMoons[] = {earthModel, jupiterModel, marsModel,mercuryModel, moonModel, neptuneModel, saturnModel, sunModel, uranusModel, venusModel};

	//view distance 500
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 500.0f);

    
	std::list<Planet> noMoons;
	std::list<Planet> earthMoons;
	std::list<Planet> planets;
	

	////Luna
    //radius = 10.0f
    //Angle 0.005f
	Planet earthMoon = Planet(planetShader,orbitShader, moonModel,orbitModel,lunaText, 10.0f, 0.005f, 8.0f * UniverseSpeed, 10000.0f * UniverseSpeed, 0.005f, noMoons, 1.0f, false,true);
    earthMoons.push_back(earthMoon);
    
	////Mercury
	//radius = 70.0f
	//Angle 0.008f
	Planet mercury = Planet(planetShader,orbitShader, mercuryModel,orbitModel,mercuryText, 70.0f, 0.1f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.008f, noMoons,7.0f, false,false);
	////Venus
	//radius = 80.0f
	//angle = 0.007f
	Planet venus =Planet(planetShader,orbitShader, venusModel,orbitModel,venusText, 80.0f, 0.1f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.007f, noMoons,8.0f, false, false);
	////earth
	//radius = 90.0f
	//angle = 0.006f
	Planet earth =Planet(planetShader,orbitShader, earthModel,orbitModel,earthText, 90.0f, 0.008f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.006f, earthMoons,9.0f, true, false);
	////Mars
	//radius = 100.0f
	//angle = 0.005f
	Planet mars = Planet(planetShader,orbitShader, marsModel,orbitModel,MarsText, 100.0f, 0.1f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.005f, noMoons,10.0f, true, false);
	//// JUPITER
	//radius = 120.0f
	//angle = 0.0045f
	Planet jupiter = Planet(planetShader,orbitShader, jupiterModel,orbitModel,jupityerText, 120.0f, 0.1f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.0045f, noMoons,12.0f, true, false);
	////Uranus
	//radius = 190.0f
	//angle = 0.0035f
	Planet uranus = Planet(planetShader,orbitShader, uranusModel,orbitModel,uranusText, 190.0f, 0.1f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.0035f, noMoons,19.0f, false, false);
	////SATURN
	//radius = 160.0f
	//angle = 0.0040f
	Planet saturn = Planet(planetShader,orbitShader, saturnModel,orbitModel,saturnText, 160.0f, 0.1f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.004f, noMoons,16.0f, false, false);
	//radius = 220.0f
	//angle = 0.003f
	Planet neptune = Planet(planetShader,orbitShader, neptuneModel,orbitModel,neptunText, 220.0f, 0.1f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.003f, noMoons,22.0f, false, false);

	////Sun
	//radius = 0.0f
	//Angle 0.006f
    
    planets.push_back(mercury);
    planets.push_back(venus);
    planets.push_back(earth);
    planets.push_back(mars);
    planets.push_back(jupiter);
    planets.push_back(uranus);
    planets.push_back(saturn);
    planets.push_back(neptune);
    
  
  Planet sun= Planet(planetShader,orbitShader , sunModel, orbitModel, sunText, 0.0f, 0.1f, 16.0f* UniverseSpeed, 0.0f, 0.006f, planets,0.0f, false, false);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
        
    
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();
		

		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
        
        starShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(starShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(starShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        sun.drawPlanet(glm::vec2(0.0f, 0.0f), UniverseSpeed, view, projection);

        
        orbitShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(orbitShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(orbitShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        sun.drawOrbit(glm::vec2(0.0f, 0.0f), UniverseSpeed);




		glm::mat4 model = glm::mat4(1.0f);
		// Draw our first triangle
		shader.Use();

		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glUniform1i(glGetUniformLocation(shader.Program, "texture1"), 0);

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(cubeVAO);

		// Calculate the model matrix for each object and pass it to shader before drawing
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}




// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (GLFW_KEY_KP_ADD== key ) {
		
		UniverseSpeed = UniverseSpeed * 1.25;
		std::cout << UniverseSpeed << std::endl;
		
	}
	else if(GLFW_KEY_KP_SUBTRACT == key){

		UniverseSpeed = UniverseSpeed / 1.25;
		std::cout << UniverseSpeed << std::endl;
		
	}
		



	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}


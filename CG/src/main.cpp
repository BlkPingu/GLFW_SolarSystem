#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// GLEW

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

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



//Math
const double PI = 3.141592653589793238463;



// Properties
const GLuint WIDTH = 1600, HEIGHT = 1200;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 100.0f, 0.0f));
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
	Shader shader("./res/shaders/modelLoading.vs", "./res/shaders/modelLoading.frag");

	// Load models
	//Model ourModel( "./res/models/nanosuit/nanosuit.obj");
    
    //planets
	Model earthModel("./res/models/planets/earth/earth.obj");
	Model jupiterModel("./res/models/planets/jupiter/jupiter1.obj");
	Model marsModel("./res/models/planets/mars/Mars_normal-strong.obj");
	Model mercuryModel("./res/models/planets/mercury/mercury.obj");
	Model neptuneModel("./res/models/planets/neptune/neptune.obj");
	Model saturnModel("./res/models/planets/saturn/saturn3.obj");
	Model uranusModel("./res/models/planets/uranus/uranus.obj");
	Model venusModel("./res/models/planets/venus/venus.obj");
    
    //sun
    Model sunModel("./res/models/planets/Sun/sun2.obj");
    
    //moons
    Model moonModel("./res/models/planets/moon/moon1.obj");

	//Model sunMoons[] = {earthModel, jupiterModel, marsModel,mercuryModel, moonModel, neptuneModel, saturnModel, sunModel, uranusModel, venusModel};

	//view distance 300
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 300.0f);

    
	std::list<Planet> noMoons;
	std::list<Planet> earthMoons;
	std::list<Planet> planets;


	////Luna
    //radius = 10.0f
    //Angle 0.005f
	Planet earthMoon = Planet(shader, moonModel, 10.0f, 0.005f, 8.0f * UniverseSpeed, 10000.0f * UniverseSpeed, 0.005f, noMoons);
    earthMoons.push_back(earthMoon);

	////Mercury
	//radius = 70.0f
	//Angle 0.008f
	Planet mercury = Planet(shader, mercuryModel, 70.0f, 0.01f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.008f, earthMoons);
	////Venus
	//radius = 80.0f
	//angle = 0.007f
	Planet venus =Planet(shader, venusModel, 80.0f, 0.01f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.007f, noMoons);
	////earth
	//radius = 90.0f
	//angle = 0.006f
	Planet earth =Planet(shader, earthModel, 90.0f, 0.01f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.006f, noMoons);
	////Mars
	//radius = 100.0f
	//angle = 0.005f
	Planet mars = Planet(shader, marsModel, 100.0f, 0.01f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.005f, earthMoons);
	//// JUPITER
	//radius = 120.0f
	//angle = 0.0045f
	Planet jupiter = Planet(shader, jupiterModel, 120.0f, 0.01f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.0045f, noMoons);
	////Uranus
	//radius = 190.0f
	//angle = 0.0035f
	Planet uranus = Planet(shader, uranusModel, 190.0f, 0.01f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.0035f, noMoons);
	////SATURN
	//radius = 160.0f
	//angle = 0.0040f
	Planet saturn = Planet(shader, saturnModel, 160.0f, 0.01f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.004f, noMoons);
	//radius = 220.0f
	//angle = 0.003f
	Planet neptune = Planet(shader, neptuneModel, 220.0f, 0.01f, 8.0f * UniverseSpeed, 1000.0f * UniverseSpeed, 0.003f, noMoons);

	////Sun
	//radius = 0.0f
	//Angle 0.006f
	Planet sun= Planet(shader, sunModel, 0.0f, 0.01f, 16.0f* UniverseSpeed, 0.0f, 0.006f, noMoons);



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
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));







		sun.drawPlanet(UniverseSpeed);
		mercury.drawPlanet(UniverseSpeed);
		venus.drawPlanet(UniverseSpeed);
		earth.drawPlanet(UniverseSpeed);
		mars.drawPlanet(UniverseSpeed);
		jupiter.drawPlanet(UniverseSpeed);
		uranus.drawPlanet(UniverseSpeed);
		saturn.drawPlanet(UniverseSpeed);
		neptune.drawPlanet(UniverseSpeed);








		//DrawPlanet(shader, neptuneModel, glm::vec3( 0.0f, -10.0f, 0.0f ), glm::vec3( 0.01f, 0.01f, 0.01f ), 8.0f);



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

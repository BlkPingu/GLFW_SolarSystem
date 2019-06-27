#pragma once
#include <stdio.h>
// Std. Includes
#include <string>
#include <list>

// GLEW

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <math.h>




class Planet {

public:
	double length;
	Shader shader;
	Model planetModel;
	GLfloat orbitDistance;
	GLfloat planetaryScale, rotationSpeed, orbitSpeed, orbitAngle;
	std::list<Planet> listOfPlanets;
	GLfloat UniverseSpeed;
    void renderOrbit(glm::vec2 currentPosition);



public:
	void drawPlanet(glm::vec2 planetPosition, GLfloat UniverseSpeed);
	glm::vec2 calculatePos(glm::vec2 relativePlanetPosition, GLfloat UniverseSpeed);
    void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);


	Planet(Shader shader, Model planetModel, GLfloat orbitDistance, GLfloat planetaryScale, GLfloat rotationSpeed, GLfloat orbitSpeed, GLfloat orbitAngle, std::list<Planet> listOfPlanets);

	~Planet();

};

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

private:
	double length;
	Shader planetShader;
    Shader orbitShader;
	Model planetModel;
    Model orbitModel;
	GLfloat orbitDistance;
	GLfloat planetaryScale, rotationSpeed, orbitSpeed, orbitAngle;
	std::list<Planet> listOfPlanets;
	GLfloat UniverseSpeed;
    GLfloat orbitScale;
    bool hasMoon;

public:
	void drawPlanet(glm::vec2 planetPosition, GLfloat UniverseSpeed);
	glm::vec2 calculatePos(glm::vec2 relativePlanetPosition, GLfloat UniverseSpeed);
    void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
    void drawOrbit(glm::vec2 orbitCenter, GLfloat UniverseSpeed);
    bool getHasMoon();


	Planet(Shader planetShader, Shader orbitShader, Model planetModel, Model orbitModel,  GLfloat orbitDistance, GLfloat planetaryScale, GLfloat rotationSpeed, GLfloat orbitSpeed, GLfloat orbitAngle, std::list<Planet> listOfPlanets, GLfloat orbitScale, bool hasMoon);

	~Planet();

};

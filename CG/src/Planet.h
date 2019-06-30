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
	Model textModel;
	GLfloat orbitDistance;
	GLfloat planetaryScale, rotationSpeed, orbitSpeed, orbitAngle;
	std::list<Planet> listOfPlanets;
	GLfloat UniverseSpeed;
    GLfloat orbitScale;
    bool hasMoon;
	bool isMoon;
public:
	void drawPlanet(glm::vec2 planetPosition, GLfloat UniverseSpeed, glm::mat4 view, glm::mat4 projection);
	glm::vec2 calculatePos(glm::vec2 relativePlanetPosition, GLfloat UniverseSpeed);
    void drawCircle( GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides);
    void drawOrbit(glm::vec2 orbitCenter, GLfloat UniverseSpeed);
	void drawName(glm::vec2 orbitCenter, GLfloat UniverseSpeed,bool isMoon);
    bool getHasMoon();
	bool getisMoon();


	Planet(Shader planetShader, Shader orbitShader, Model planetModel, Model orbitModel, Model textModel,  GLfloat orbitDistance, GLfloat planetaryScale, GLfloat rotationSpeed, GLfloat orbitSpeed, GLfloat orbitAngle, std::list<Planet> listOfPlanets, GLfloat orbitScale, bool hasMoon,bool isMoon);

	~Planet();

};

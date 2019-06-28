#include <stdio.h>
#include "Planet.h"
#include "Model.h"
float xk = 0;
float yk = 0;
Shader shader;
Model planetModel;
GLfloat orbitDistance;
GLfloat planetaryScale;
GLfloat rotationSpeed;
GLfloat orbitSpeed;
GLfloat orbitAngle;
std::list<Planet> listOfPlanetsmoons;
glm::vec3 orbitScale;



Planet::Planet(Shader planetShader, Shader orbitShader, Model planetModel, Model orbitModel, GLfloat orbitDistance, GLfloat planetaryScale, GLfloat rotationSpeed, GLfloat orbitSpeed, GLfloat orbitAngle, std::list<Planet> listOfPlanetsmoons, GLfloat orbitScale) {
	this->planetShader = planetShader;
    this->orbitShader = orbitShader;
	this->planetModel = planetModel;
    this->orbitModel = orbitModel;
	this->orbitDistance = orbitDistance;
	this->planetaryScale = planetaryScale;
	this->rotationSpeed = rotationSpeed;
	this->orbitSpeed = orbitSpeed;
	this->orbitAngle = orbitAngle;
	this->listOfPlanets = listOfPlanetsmoons;
    this->orbitScale = orbitScale;
}

Planet::~Planet() {
}


/*
// render this planets orbit circle
void Planet::renderOrbit(glm::vec2 currentPlanetPosition)
{
    // draw a line strip
    glBegin(GL_LINE_STRIP);
    // loop round from 0 to 2*PI and draw around the radius of the orbit using trigonometry
    for (float angle = 0.0f; angle < 6.283185307f; angle += 0.05f)
    {
        glVertex3f(sin(angle) * orbitDistance, cos(angle) * orbitDistance, 0.0f);
    }
    glVertex3f(0.0f, orbitDistance, 0.0f);
    glEnd();
    
    //render the moons' orbit
    glPushMatrix();
    // translate to the center of this planet to draw the moon orbit around it
    glTranslatef(currentPlanetPosition[0], currentPlanetPosition[1], 0.0f);
    // draw moon orbits
    glPopMatrix();
}
*/

glm::vec2 Planet::calculatePos(glm::vec2 relativePlanetPosition, GLfloat UniverseSpeed) {
	const double PI = 3.141592653589793238463;
	GLfloat angle, radius, x, y;

	//Orbit Calculations
	angle = orbitAngle * (GLfloat)glfwGetTime() * orbitSpeed * UniverseSpeed;
	radius = orbitDistance;
	x = radius * sin(PI * 2 * angle / 360) + relativePlanetPosition.x;
	y = radius * cos(PI * 2 * angle / 360) + relativePlanetPosition.y;


	return glm::vec2(x, y);
}


void Planet::drawPlanet(glm::vec2 planetPosition, GLfloat UniverseSpeed) {
	planetShader.Use();

	//Drawing
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec2 currentPlanetPosition = calculatePos(planetPosition, UniverseSpeed);
	//Relative Position
	model = glm::translate(model, glm::vec3(currentPlanetPosition.x, 0.0f,currentPlanetPosition.y));

	//Rotation
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * rotationSpeed * UniverseSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

	//Scaling
	model = glm::scale(model, glm::vec3(planetaryScale, planetaryScale, planetaryScale));
	glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	planetModel.Draw(planetShader);

	for (Planet moon : listOfPlanets) {
        moon.drawPlanet(currentPlanetPosition, UniverseSpeed);
        //moon.renderOrbit(currentPlanetPosition);
	}
}


void Planet::drawOrbit(glm::vec2 planetPosition, GLfloat UniverseSpeed, glm::vec2 orbitCenter) {
    orbitShader.Use();
    
    //Drawing
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 currentPlanetPosition = calculatePos(planetPosition, UniverseSpeed);
    //Relative Position
    //model = glm::translate(model, glm::vec3(currentPlanetPosition.x, 0.0f,currentPlanetPosition.y));
    model = glm::translate(model, glm::vec3(orbitCenter.x, orbitCenter.y,0.0f));
    
    //Scaling
    model = glm::scale(model, glm::vec3(orbitScale, orbitScale, orbitScale));
    glUniformMatrix4fv(glGetUniformLocation(orbitShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    orbitModel.Draw(orbitShader);
    
    for (Planet moon : listOfPlanets) {
        moon.drawOrbit(currentPlanetPosition, UniverseSpeed, currentPlanetPosition);
    }
}

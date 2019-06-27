#pragma once

#include <stdio.h>
#include "Planet.h"
#include "Model.h"
float xk = 0;
float yk = 0;
Shader shader;
Model planetModel;
GLfloat orbitdistance;
GLfloat planetaryScale;
GLfloat rotationSpeed;
GLfloat orbitSpeed;
GLfloat orbitAngle;
std::list<Planet> listOfPlanetsmoons;



Planet::Planet(Shader shader, Model planetModel, GLfloat orbitDistance, GLfloat planetaryScale, GLfloat rotationSpeed, GLfloat orbitSpeed, GLfloat orbitAngle, std::list<Planet> listOfPlanetsmoons) {
	this->shader = shader;
	this->planetModel = planetModel;
	this->orbitDistance = orbitDistance;
	this->planetaryScale = planetaryScale;
	this->rotationSpeed = rotationSpeed;
	this->orbitSpeed = orbitSpeed;
	this->orbitAngle = orbitAngle;
	this->listOfPlanets = listOfPlanetsmoons;
}

Planet::~Planet() {
}

// render this planets orbit circle
void Planet::renderOrbit(float distanceFromSun, glm::vec2 positions)
{
    // draw a line strip
    glBegin(GL_LINE_STRIP);
    
    // loop round from 0 to 2*PI and draw around the radius of the orbit using trigonometry
    for (float angle = 0.0f; angle < 6.283185307f; angle += 0.05f)
    {
        glVertex3f(sin(angle) * distanceFromSun, cos(angle) * distanceFromSun, 0.0f);
    }
    glVertex3f(0.0f, distanceFromSun, 0.0f);
    
    glEnd();
    
    // render the moons' orbit
    glPushMatrix();
    // translate to the center of this planet to draw the moon orbit around it
    glTranslatef(positions[0], positions[1], 0.0f);
    // draw all moon orbits
    glPopMatrix();
}

glm::vec2 Planet::calculatePos(glm::vec2 relativePlanetPosition, GLfloat UniverseSpeed) {
	const double PI = 3.141592653589793238463;
	GLfloat angle, radius, x, y;

	//Orbit Calculations
	angle = orbitAngle * (GLfloat)glfwGetTime() * orbitSpeed * UniverseSpeed;
	radius = orbitDistance;
	x = radius * sin(PI * 2 * angle / 360);
	y = radius * cos(PI * 2 * angle / 360);
    x = x + relativePlanetPosition.x;
    y = y + relativePlanetPosition.y;

	return glm::vec2(x, y);
}

void Planet::drawPlanet(glm::vec2 planetPosition, GLfloat UniverseSpeed) {
	shader.Use();
	//Drawing
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec2 currentPlanetPosition = calculatePos(planetPosition, UniverseSpeed);
	//Relative Position
	model = glm::translate(model, glm::vec3(currentPlanetPosition.x, 0.0f,currentPlanetPosition.y));

	//Rotation
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * rotationSpeed * UniverseSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

	//Scaling
	model = glm::scale(model, glm::vec3(planetaryScale, planetaryScale, planetaryScale));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	planetModel.Draw(shader);

	for (Planet moon : listOfPlanets) {
		//moon.drawMoon(xy,UniverseSpeed);
        moon.drawPlanet(currentPlanetPosition, UniverseSpeed);
	}
    renderOrbit(orbitDistance, currentPlanetPosition);
}

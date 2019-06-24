#pragma once
#include <stdio.h>
#include "planet.h"
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


void Planet::drawMoon(glm::vec2 positions) {
	shader.Use();
	//Drawing
	glm::mat4 model = glm::mat4(1.0f);

	//Relative Position
	model = glm::translate(model, glm::vec3(positions.x, 0.0f, positions.y));

	//Rotation
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

	//Scaling
	model = glm::scale(model, glm::vec3(planetaryScale, planetaryScale, planetaryScale));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	planetModel.Draw(shader);


}

glm::vec2 Planet::calculatePos() {
	const double PI = 3.141592653589793238463;
	GLfloat angle, radius, x, y;

	//Orbit Calculations
	angle = orbitAngle * (GLfloat)glfwGetTime() * orbitSpeed;
	radius = orbitDistance;
	x = radius * sin(PI * 2 * angle / 360);
	y = radius * cos(PI * 2 * angle / 360);

	std::cout << "angle " << angle << "\n"
		<< "radius" << radius << "\n"
		<< "x " << x << "\n"
		<< "y " << y << "\n"
		<< "time" << (GLfloat)glfwGetTime() << std::endl;

	return glm::vec2(x, y);
}

void Planet::drawPlanet() {
	shader.Use();
	//Drawing
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec2 xy = calculatePos();
	//Relative Position
	model = glm::translate(model, glm::vec3(xy.x, 0.0f,xy.y));

	//Rotation
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

	//Scaling
	model = glm::scale(model, glm::vec3(planetaryScale, planetaryScale, planetaryScale));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	planetModel.Draw(shader);

	for (Planet moon : listOfPlanets) {
		moon.drawMoon(calculatePos());
	}
}

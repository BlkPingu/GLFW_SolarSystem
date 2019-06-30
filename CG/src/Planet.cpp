#include <stdio.h>
#include "Planet.h"
#include "Model.h"
float xk = 0;
float yk = 0;
Shader shader;
Model planetModel;
Model textModel;
GLfloat orbitDistance;
GLfloat planetaryScale;
GLfloat rotationSpeed;
GLfloat orbitSpeed;
GLfloat orbitAngle;
std::list<Planet> listOfPlanetsmoons;
glm::vec3 orbitScale;
bool hasMoon;
bool isMoon;


Planet::Planet(Shader planetShader, Shader orbitShader, Model planetModel, Model orbitModel,Model textModel, GLfloat orbitDistance, GLfloat planetaryScale, GLfloat rotationSpeed, GLfloat orbitSpeed, GLfloat orbitAngle, std::list<Planet> listOfPlanetsmoons, GLfloat orbitScale, bool hasMoon,bool isMoon) {
	this->planetShader = planetShader;
    this->orbitShader = orbitShader;
	this->planetModel = planetModel;
    this->orbitModel = orbitModel;
	this->textModel = textModel;
	this->orbitDistance = orbitDistance;
	this->planetaryScale = planetaryScale;
	this->rotationSpeed = rotationSpeed;
	this->orbitSpeed = orbitSpeed;
	this->orbitAngle = orbitAngle;
	this->listOfPlanets = listOfPlanetsmoons;
    this->orbitScale = orbitScale;
    this->hasMoon = hasMoon;
	this->isMoon = isMoon;
}

Planet::~Planet() {
}

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

bool Planet::getHasMoon(){
    return hasMoon;
}

bool Planet::getisMoon() {
	return isMoon;
}


void Planet::drawPlanet(glm::vec2 planetPosition, GLfloat UniverseSpeed, glm::mat4 view, glm::mat4 projection) {
	planetShader.Use();

	//Drawing
	glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f,20.0f,0.0f);

	glm::vec2 currentPlanetPosition = calculatePos(planetPosition, UniverseSpeed);
	//Relative Position
	model = glm::translate(model, glm::vec3(currentPlanetPosition.x, 0.0f,currentPlanetPosition.y));
	float angle = 90.0f;
	//Rotation
	model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * rotationSpeed * UniverseSpeed), glm::vec3(0.0f, 1.0f, 0.0f));

	//Scaling
	model = glm::scale(model, glm::vec3(planetaryScale, planetaryScale, planetaryScale));
	glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	planetModel.Draw(planetShader);
	
	for (Planet planet : listOfPlanets) {

        planetShader.Use();
        glUniform3f(glGetUniformLocation(planetShader.Program, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
        glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(planetShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    		planet.drawName(currentPlanetPosition, UniverseSpeed,planet.isMoon);
        planet.drawPlanet(currentPlanetPosition, UniverseSpeed, view, projection);

	}
}


void Planet::drawOrbit(glm::vec2 orbitCenter, GLfloat UniverseSpeed) {
    orbitShader.Use();
    
    //Drawing
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 currentPlanetPosition = calculatePos(orbitCenter, UniverseSpeed);
    //Relative Position
    //model = glm::translate(model, glm::vec3(currentPlanetPosition.x, 0.0f,currentPlanetPosition.y));
    model = glm::translate(model, glm::vec3(orbitCenter.x, 0.0f,orbitCenter.y));
    
    //Scaling
    model = glm::scale(model, glm::vec3(orbitScale, orbitScale, orbitScale));
    glUniformMatrix4fv(glGetUniformLocation(orbitShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    orbitModel.Draw(orbitShader);
	
    for (Planet orbiter : listOfPlanets) {
		if (orbiter.hasMoon == false) {
			
			orbiter.drawOrbit(currentPlanetPosition, UniverseSpeed); 
			
			
				
			
			
		
		
		}
        else{
			orbiter.drawOrbit(currentPlanetPosition, UniverseSpeed);
			
		
		}
    }
}

void Planet::drawName(glm::vec2 planetposition, GLfloat UniverseSpeed,bool isMoon) {
	orbitShader.Use();

	glm::vec2 currentPlanetPosition = calculatePos(planetposition, UniverseSpeed);
	//Drawing
	glm::mat4 model = glm::mat4(1.0f);
	
		if (isMoon == true) {

			//Relative Position
			model = glm::translate(model, glm::vec3(planetposition.x, 0.0f, planetposition.y + 15.0f));




		}

		else {
			model = glm::translate(model, glm::vec3(orbitDistance, 0.0f, planetposition.y));





		}
		model =glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//Scaling
		model = glm::scale(model, glm::vec3(6, 6, 6));
		glUniformMatrix4fv(glGetUniformLocation(orbitShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		textModel.Draw(orbitShader);
	}


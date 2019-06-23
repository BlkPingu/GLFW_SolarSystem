//
//  Planet.cpp
//  CGTutorial
//
//  Created by Tobias Kolb on 23.06.19.
//

#include "Planet.hpp"
#include "Model.hpp"


Planet::Planet(Shader shader, Model planetModel, GLfloat orbitDistance, GLfloat planetaryScale, GLfloat rotationSpeed, GLfloat orbitSpeed, GLfloat orbitAngle, std::list<Planet> listOfPlanetsmoons) {
    
    std::cout << "Planet created" << std::endl;
}

Planet::~Planet(){
}

glm::vec2 Planet::calculatePos(){
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
    << "time" <<  (GLfloat)glfwGetTime() << std::endl;
    
    return glm::vec2(x,y);
}

void Planet::drawPlanet(glm::vec2 positions){
    
    //Drawing
    glm::mat4 model = glm::mat4(1.0f);
    
    //Relative Position
    model = glm::translate( model, glm::vec3( positions.x, 0.0f, positions.y));
    
    //Rotation
    model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * rotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
    
    //Scaling
    model = glm::scale( model, glm::vec3( planetaryScale, planetaryScale, planetaryScale ));
    glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    
    planetModel.Draw( shader );
    
    for(Planet moon : listOfPlanets) {
        moon.drawPlanet(positions);
    }
}


//
//  Planet.hpp
//  CGTutorial
//
//  Created by Tobias Kolb on 23.06.19.
//

#ifndef Planet_hpp
#define Planet_hpp

#include <stdio.h>
// Std. Includes
#include <string>
#include <list>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <math.h>
#include "Model.hpp"



class Planet {
public:
    void drawPlanet(glm::vec2 positions);
    glm::vec2 calculatePos();
    Planet(Shader shader, Model planetModel, GLfloat orbitDistance, GLfloat planetaryScale, GLfloat rotationSpeed, GLfloat orbitSpeed, GLfloat orbitAngle, std::list<Planet> listOfPlanets);
    
    ~Planet();
private:
    double length;
    Shader shader;
    Model planetModel;
    GLfloat orbitDistance;
    GLfloat planetaryScale, rotationSpeed, orbitSpeed, orbitAngle;
    std::list<Planet> listOfPlanets;
};

#endif /* Planet_hpp */

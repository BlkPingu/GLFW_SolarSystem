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
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include <math.h>


//Math
const double PI = 3.141592653589793238463;

GLfloat angle, radius, x, y;

#endif /* Planet_hpp */

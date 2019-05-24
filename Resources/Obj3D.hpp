//
//  Obj3D.hpp
//  CGTutorial
//
//  Created by Tobias Kolb on 23.05.18.
//

#ifndef Obj3D_hpp
#define Obj3D_hpp

#include <stdio.h>

class Obj3D{

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint vertexArrayID;

public:
Obj3D(const char* fn);
void(display());
~Obj3D();
    
    
};

#endif /* Obj3D_hpp */

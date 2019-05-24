//
//  Obj3D.cpp
//  CGTutorial
//
//  Created by Tobias Kolb on 23.05.18.
//

// a) Projekt -> Klasse hinzufügen
// b) Methode Display hinzufügen
// c) Variablen mit Copy / Paste
// d) Befehle übertragen / anpassen
// e) Obj3D teapot("teapot.obj");...

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "objects.hpp"
#include "objloader.hpp"
#include "texture.hpp"

using namespace glm;


#include "Obj3D.hpp"

Obj3D::Obj3D(const char* fn){

    bool res = loadOBJ("/Users/Tobias/Desktop/Pflichtteil/CG/source/CGTutorial/resources/teapot.obj", vertices, uvs, normals);
    
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
    
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*) 0);

    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

}


void Obj3D::display(){
    glBindVertexArray(vertexArrayID);
    glDrawArrays(GL_TRIANGLES,0,vertices.size());
}

Obj3D::~Obj3D(){
    glDeleteBuffers(1,&vertexbuffer);
    glDeleteBuffers(1,&normalbuffer);
    glDeleteBuffers(1,&uvbuffer);
}





    


    
    
    
    
    
    
    
    
    

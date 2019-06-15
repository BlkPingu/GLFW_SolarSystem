#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include "shader.hpp"
#include "objects.hpp"
#include "objloader.hpp"
#include "texture.hpp"
#include "Obj3D.hpp"


// NEUE IMPORTS
#include "Mesh.h"

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}


//Koordinatenkreuz
float xAxis = 0.0f;
float yAxis = 0.0f;
float zAxis = 0.0f;

//Arm
float yAxisArm =  0.0f;
float z1AxisArm = 0.0f;
float z2AxisArm = 0.0f;
float z3AxisArm = 0.0f;


//Funktion als Reaktion auf einen Tastendruck
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
        
        //QA WS ED fuer Coordinatenkreuz
        case GLFW_KEY_Q:
            xAxis-= 0.1f;
            break;
        case GLFW_KEY_A:
            xAxis+= 0.1f;
            break;
            
        case GLFW_KEY_W:
            yAxis-= 0.1f;
            break;
        case GLFW_KEY_S:
            yAxis+= 0.1f;
            break;
        case GLFW_KEY_E:
            zAxis-= 0.1f;
            break;
        case GLFW_KEY_D:
            zAxis+= 0.1f;
            break;

            
        //RF TG ZH UJ fuer Arm
        case GLFW_KEY_R: //y+
            yAxisArm+= 0.1f;
            break;
        
        case GLFW_KEY_F: //y-
            yAxisArm-= 0.1f;
            break;
            
        case GLFW_KEY_T: //y1+
            z1AxisArm+= 0.1f;
            break;
            
        case GLFW_KEY_G: //y1-
            z1AxisArm-= 0.1f;
            break;
            
        case GLFW_KEY_Y: //y2+
            z2AxisArm+= 0.1f;
            break;
            
        case GLFW_KEY_H: //y2-
            z2AxisArm-= 0.1f;
            break;
        case GLFW_KEY_U: //y3+
            z3AxisArm+= 0.1f;
            break;
            
        case GLFW_KEY_J: //y3-
            z3AxisArm-= 0.1f;
            break;
        
        
            
        //Reaktion auf ESC
        case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;

	default:
		break;
	}
}


// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;

//Ergebnis der Multiplikationen an die Grafikkarte mitteilen
void sendMVP()
{
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; 
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}

void zeichneKS(){
    //1 Balken
    glm:mat4 Save = Model;
    Model = glm::scale(Model,glm::vec3(5.0f,.01f,.01f));
    sendMVP();
    drawCube();
    Model = Save;
    
    //2 Balken
    Save = Model;
    Model = glm::scale(Model ,glm::vec3(.01f,5.0f,.01f));
    sendMVP();
    drawCube();
    Model = Save;
   
    
    //3 Balken
    Save = Model;
    Model = glm::scale(Model ,glm::vec3(.01f,.01f,5.0f));
    sendMVP();
    drawCube();
    Model = Save;
   
}

void zeichneSeg(float h){
    glm::mat4 Save = Model;
    Model = glm::translate(Model,glm::vec3(0,h/2,0));
    Model = glm::scale(Model,glm::vec3(h/10,h/2,h/10));
    sendMVP();
    drawSphere(100,100);
    Model = Save;
}

void zeichneArm(){
    glm::mat4 Save = Model;
    Model = glm::rotate(Model,yAxisArm, glm::vec3(0,1,0));
    Model = glm::rotate(Model,z1AxisArm, glm::vec3(0,0,1));

    zeichneSeg(1);
    Model = glm::translate(Model, glm::vec3(0,1,0));
    Model = glm::rotate(Model,z2AxisArm, glm::vec3(0,0,1));

    
    zeichneSeg(0.8);
    Model = glm::translate(Model, glm::vec3(0,0.8,0));
    Model = glm::rotate(Model,z3AxisArm, glm::vec3(0,0,1));
    zeichneSeg(0.6);
    
    glm::vec4 lightPos = Model * glm::vec4(0,0.6,0,1);
    glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"),lightPos.x,lightPos.y,lightPos.z);
    Model = Save;
    
    
}

int main(void)
{
	
    
    
    // Initialise GLFW
	if (!glfwInit())
	{
        //Wenn RW von glfwInit !=0, dann Fehlermeldung
        // GLEW ermšglicht Zugriff auf OpenGL-API > 1.1
        fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	//Fehler werden auf stderr ausgegeben, s. o.
    //Error Callback: Funktion error_callback wird aufgerufen (uebergeben als Parameter)
	glfwSetErrorCallback(error_callback);
    

    // Die folgenden vier Zeilen sind nštig auf dem Mac
    // Au§erdem mŸssen die zu ladenden Dateien bei der aktuellen Projektkonfiguration
    // unter DerivedData/Build/Products/Debug (oder dann Release) zu finden sein
    //Rueckagabewert von glfwCreateWindow ist ein Pointer auf das Fenster

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window and create its OpenGL context
	// glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen
	GLFWwindow* window = glfwCreateWindow(1024, // Breite
										  768,  // Hoehe
										  "CG - Tutorial", // Ueberschrift
										  NULL,  // windowed mode
										  NULL); // shared windoe

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current (wird nicht automatisch gemacht)
    glfwMakeContextCurrent(window);

	// Initialize GLEW
	// GLEW ermöglicht Zugriff auf OpenGL-API > 1.1
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
    
    

	// Auf Keyboard-Events reagieren
    //Wenn Benutzer eine Taste drŸckt, dann soll diese Funktion ausgefŸhrt werden
    glfwSetKeyCallback(window, key_callback);

	// Dark blue background
    // rot, gelb, blau, durchsichtbarkeit
    // Angabe in Prozent
    //Passiert nichts, aber Angabe die lšschbare Farbe
    //Auf wei§ setzen: 1,1,1 maximale Helligkeit
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    //*********** WICHTIG ************* //
    
    // Create and compile our GLSL program from the shaders
    //Installation von shaders: Textdateien als Parameter
    //Werden im BS geladen (an der richtigen Speicherstelle)
    
    //programID = LoadShaders("/Users/Tobias/Documents/Studium/8SoSe'18 AI/Computergrafik/Projektdateien/Uebung1/CG/source/CGTutorial/resources/TransformVertexShader.vertexshader", "/Users/Tobias/Documents/Studium/8SoSe'18 AI/Computergrafik/Projektdateien/Uebung1/CG/source/CGTutorial/resources/ColorFragmentShader.fragmentshader");
    
    programID = LoadShaders("/Users/Tobias/Documents/Studium/10SoSe_19_AI/Computergrafik_Jung/Pflichtteil/CG/source/CGTutorial/resources/StandardShading.vertexshader", "/Users/Tobias/Documents/Studium/10SoSe_19_AI/Computergrafik_Jung/Pflichtteil/CG/source/CGTutorial/resources/StandardShading.fragmentshader");

	// Shader auch benutzen !
	glUseProgram(programID);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LESS);
    
    Obj3D teapot("/Users/Tobias/Documents/Studium/10SoSe_19_AI/Computergrafik_Jung/Pflichtteil/CG/source/CGTutorial/resources/teapot.obj");
                                        /*
                                        
                                        std::vector<glm::vec3> vertices;
                                        std::vector<glm::vec2> uvs;
                                        std::vector<glm::vec3> normals;
                                        bool res = loadOBJ("/Users/Tobias/Documents/Studium/8SoSe'18 AI/Computergrafik/Projektdateien/Uebung1/CG/source/CGTutorial/resources/teapot.obj", vertices, uvs, normals);
                                        
                                        // Jedes Objekt eigenem VAO zuordnen, damit mehrere Objekte moeglich sind
                                        // VAOs sind Container fuer mehrere Buffer, die zusammen gesetzt werden sollen.
                                        GLuint VertexArrayIDTeapot;
                                        glGenVertexArrays(1, &VertexArrayIDTeapot);
                                        glBindVertexArray(VertexArrayIDTeapot);
                                        
                                        // Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
                                        GLuint vertexbuffer;
                                        glGenBuffers(1, &vertexbuffer); // Kennung erhalten
                                        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
                                        // Buffer zugreifbar für die Shader machen
                                        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
                                        
                                        // Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
                                        glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0
                                        glVertexAttribPointer(0,  // location = 0
                                                              3,  // Datenformat vec3: 3 floats fuer xyz
                                                              GL_FLOAT,
                                                              GL_FALSE, // Fixedpoint data normalisieren ?
                                                              0, // Eckpunkte direkt hintereinander gespeichert
                                                              (void*) 0); // abweichender Datenanfang ?

                                        GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
                                        glGenBuffers(1, &normalbuffer);
                                        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
                                        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
                                        glEnableVertexAttribArray(2); // siehe layout im vertex shader
                                        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
                                        
                                        GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)
                                        glGenBuffers(1, &uvbuffer);
                                        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
                                        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
                                        glEnableVertexAttribArray(1); // siehe layout im vertex shader
                                        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
                                        
                                         
                                         */
    // Load the texture
    GLuint Texture = loadBMP_custom("/Users/Tobias/Documents/Studium/10SoSe_19_AI/Computergrafik_Jung/Pflichtteil/CG/source/CGTutorial/resources/mandrill.bmp");
    
	// Eventloop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen
        //Bild faengt immer von vorne an
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

        //***** Transformationsmatrizen (Folie: Aufbau von Szenen) ******* //
        //Wo steht die Matrix
        // Projection matrix : 45¡ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        // Augenwinkel vom Betrachter
        Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		
        // Camera matrix (wo steht die Kamera)
		View = glm::lookAt(glm::vec3(0,0,-5), // Camera is at (0,0,-5), in World Space
						   glm::vec3(0,0,0),  // and looks at the origin
						   glm::vec3(0,1,0)); // Head is up (set to 0,-1,0 to look upside-down)
		
        //Anordnug des Objekts im Raum
		// Model matrix : an identity matrix (model will be at the origin)
		Model = glm::mat4(1.0f);
        Model = rotate(Model, xAxis, glm::vec3(1.0f,0.0f,0.0f));
        Model = rotate(Model, yAxis, glm::vec3(0.0f,1.0f,0.0f));
        Model = rotate(Model, zAxis, glm::vec3(0.0f,0.0f,1.0f));
        
        glm::mat4 Save = Model;
        Model = glm::translate(Model, glm::vec3(1.5, 0.0, 0.0));    
        
        Model = glm::scale(Model, glm::vec3(1.0 / 1000.0, 1.0 / 1000.0, 1.0 / 1000.0));
        
        //glm::vec3 lightPos = glm::vec3(4,4,-4);
        //glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

    
        

        sendMVP();


		
                                /*
                                glBindVertexArray(VertexArrayIDTeapot); //draw teapot
                                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
                                */
        
        teapot.display();
    
        //MALEN - aus objects.cpp
		//drawWireCube();
        //drawCube();  //CUBE
        

        Model = Save;
        Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
        
        sendMVP();
        
        //drawSphere(10, 10);
        
        
        
        zeichneKS();
        zeichneArm();


		// Swap buffers
        // Alles wird angezeigt
        glfwSwapBuffers(window);

		// Poll for and process events
        // glfw stellt fest, dass eine Taste gedrŸckt wurde --> pollEvents()
        glfwPollEvents();
        
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);
        
	}
    
                                        /*
                                        // Cleanup GPU Buffer
                                        glDeleteBuffers(1, &vertexbuffer);
                                        // Cleanup VBO and shader
                                        glDeleteProgram(programID);
                                        
                                        glDeleteBuffers(1, &normalbuffer);

                                        glDeleteBuffers(1, &uvbuffer);
                                        glDeleteTextures(1, &Texture);
                                        */
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}


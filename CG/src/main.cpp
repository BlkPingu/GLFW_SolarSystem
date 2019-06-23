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

// Properties
const GLuint WIDTH = 1600, HEIGHT = 1200;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera camera( glm::vec3( 0.0f, 100.0f, 0.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag" );
    
    // Load models
    //Model ourModel( "res/models/nanosuit/nanosuit.obj");
    Model earthModel( "res/models/planets/earth/earth.obj");
    Model jupiterModel( "res/models/planets/jupiter/jupiter1.obj");
    Model marsModel("res/models/planets/mars/Mars_normal-strong.obj");
    Model mercuryModel( "res/models/planets/mercury/mercury.obj");
    Model moonModel("res/models/planets/moon/moon1.obj");
    Model neptuneModel("res/models/planets/neptune/neptune.obj");
    Model saturnModel("res/models/planets/saturn/saturn3.obj");
    Model sunModel("res/models/planets/Sun/sun2.obj");
    Model uranusModel("res/models/planets/uranus/uranus.obj");
    Model venusModel( "res/models/planets/venus/venus.obj");

    Model sunMoons[] = {earthModel, jupiterModel, marsModel,mercuryModel, moonModel, neptuneModel, saturnModel, sunModel, uranusModel, venusModel};
    
    //view distance 300
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 300.0f );

    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        shader.Use( );
        
        glm::mat4 view = camera.GetViewMatrix( );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        
        ////Sun
        //radius = 0.0f
        //Angle 0.006f
        DrawPlanet(shader, sunModel, 0.0f, 0.01f, 16.0f, 0.0f, 0.006f);
        ////Mercury
        //radius = 70.0f
        //Angle 0.008f
        DrawPlanet(shader, mercuryModel, 70.0f, 0.01f, 8.0f, 1000.0f, 0.008f);
        ////Venus
        //radius = 80.0f
        //angle = 0.007f
        DrawPlanet(shader, venusModel, 80.0f, 0.01f, 8.0f, 1000.0f, 0.007f);
        ////earth
        //radius = 90.0f
        //angle = 0.006f
        DrawPlanet(shader, earthModel, 90.0f, 0.01f, 8.0f, 1000.0f, 0.006f);
        
        ////Mars
        //radius = 100.0f
        //angle = 0.005f
        DrawPlanet(shader, marsModel, 100.0f, 0.01f, 8.0f, 1000.0f, 0.005f);
        //// JUPITER
        //radius = 120.0f
        //angle = 0.0045f
        DrawPlanet(shader, jupiterModel, 120.0f, 0.01f, 8.0f, 1000.0f, 0.0045f);
        ////Uranus
        //radius = 190.0f
        //angle = 0.0035f
        DrawPlanet(shader, uranusModel, 190.0f, 0.01f, 8.0f, 1000.0f, 0.0035f);
        ////SATURN
        //radius = 160.0f
        //angle = 0.0040f
        DrawPlanet(shader, saturnModel, 160.0f, 0.01f, 8.0f, 1000.0f, 0.004f);
        //// NEPTUNE
        //radius = 220.0f
        //angle = 0.003f
        DrawPlanet(shader, neptuneModel, 220.0f, 0.01f, 8.0f, 1000.0f, 0.003f);

        

        
        
        //DrawPlanet(shader, neptuneModel, glm::vec3( 0.0f, -10.0f, 0.0f ), glm::vec3( 0.01f, 0.01f, 0.01f ), 8.0f);


        
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}




// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] ){
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] ){
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] ){
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] ){
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode ){
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action ){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 ){
        if ( action == GLFW_PRESS ){
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE ){
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos ){
    if ( firstMouse ){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}


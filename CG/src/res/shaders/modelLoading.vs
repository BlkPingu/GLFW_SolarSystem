/*
 #version 330 core
 layout (location = 0) in vec3 aPos;
 layout (location = 1) in vec3 aNormal;
 layout (location = 2) in vec2 aTexCoords;
 
 out vec2 TexCoords;
 
 uniform mat4 model;
 uniform mat4 view;
 uniform mat4 projection;
 
 void main()
 {
 TexCoords = aTexCoords;
 gl_Position = projection * view * model * vec4(aPos, 1.0);
 }
 
 
 
 #version 330 core
 
 // Input vertex data, different for all executions of this shader.
 layout(location = 0) in vec3 vertexPosition_modelspace;
 layout(location = 1) in vec2 vertexUV;
 layout(location = 2) in vec3 vertexNormal_modelspace;
 
 // Output data ; will be interpolated for each fragment.
 out vec2 UV;
 out vec3 Position_worldspace;
 out vec3 Normal_cameraspace;
 out vec3 EyeDirection_cameraspace;
 out vec3 LightDirection_cameraspace;
 
 // Values that stay constant for the whole mesh.
 uniform mat4 MVP;
 uniform mat4 V;
 uniform mat4 M;
 uniform vec3 LightPosition_worldspace;
 
 void main(){
 
 // Output position of the vertex, in clip space : MVP * position
 gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
 
 // Position of the vertex, in worldspace : M * position
 Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
 
 // Vector that goes from the vertex to the camera, in camera space.
 // In camera space, the camera is at the origin (0,0,0).
 vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
 
 // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
 vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
 
 // Normal of the the vertex, in camera space
 Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
 
 // UV of the vertex. No special space for this one.
 UV = vertexUV;
 }
 */


#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

// Output data ; will be interpolated for each fragment.
out vec2 TexCoords;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 LightPosition_worldspace;

void main(){
    
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  projection * view * model * vec4(aPos,1.0);
    
    // Position of the vertex, in worldspace : model * position
    Position_worldspace = (model * vec4(aPos,1.0)).xyz;
    
    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 vertexPosition_cameraspace = ( view * model * vec4(aPos,1.0)).xyz;
    EyeDirection_cameraspace = vec3(0.0,0.0,0.0) - vertexPosition_cameraspace;
    
    // Vector that goes from the vertex to the light, in camera space. model is ommited because it's identity.
    vec3 LightPosition_cameraspace = ( view * vec4(LightPosition_worldspace,1.0)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
    
    // Normal of the the vertex, in camera space
    Normal_cameraspace = ( view * model * vec4(aNormal,0.0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    
    // UV of the vertex. No special space for this one.
    TexCoords = aTexCoords;
}



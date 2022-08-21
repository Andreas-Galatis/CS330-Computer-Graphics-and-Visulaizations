#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#include <GL/GL.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      // Image loading Utility functions
#include <vector>

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

#include <camera.h>


using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Andreas Galatis: FINAL PROJECT - 3D scene of complex objects"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        // Handles for the vertex array object
        GLuint vao, vao1, vao2, vao3;
        GLuint vao4, vao5, vao6, vao7;
        GLuint vbo;         // Handle for the vertex buffer objects
        GLuint nVertices;   // number of vertices of the mesh
        GLuint nIndices;    // number of indices of the mesh

        int longitude = 30;
        int latitude = 30;
        double radius = 2;
        float Lats = 1 / (float)(longitude - 1);
        float Longs = 1 / (float)(latitude - 1);

        int r, s;
        long M_PI = 3.141592653;
        long M_PI_2 = 1.5707963267948;

    };

    


    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Triangle mesh data
    GLMesh gMesh;
    GLMesh gMeshFillLamp;
    GLMesh gMeshOuter;
    GLMesh gMeshInner;
    GLMesh gMeshPlane;
    GLMesh gMeshCandle;
    GLMesh gMeshCube;
    GLMesh gMeshBall;
   
   

    // Textures
    GLuint gTextureIdPlane;
    GLuint gTextureIdOuter;
    GLuint gTextureIdInner;
    GLuint gTextureIdCandle;
    GLuint gTextureIdCube;
    GLuint gTextureIdBall;
    

    // Shader programs
    GLuint gProgramIdOuter;
    GLuint gProgramIdInner;
    GLuint gProgramIdPlane;
    GLuint gLampProgramId;
    GLuint gProgramIdCandle;
    GLuint gProgramIdCube;
    GLuint gProgramIdBall;
    GLuint gFillLampProgramId;

    glm::vec2 gUVScale(5.0f, 5.0f);

    // camera control declarations 
    Camera gCamera(glm::vec3(0.0f, 3.0f, 15.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // timing declarations
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    bool perspective = true;

    // Subject position and scale
    glm::vec3 gObjectPosition(3.0f, -0.6f, -1.0f);  // Eye-glass case position
    glm::vec3 gPlanePosition(0.0f, 0.5f, 0.0f);     // Plane/Table position
    glm::vec3 gCandlePosition(0.0f, -0.9f, 2.0f);   // Candle position
    glm::vec3 gCubePosition(-0.5f, -0.4f, -2.5f);   // Cube position
    glm::vec3 gBallPosition(0.0f, 0.1f, -2.0f);     // Ball position
    
    glm::vec3 gObjectScale(1.0f);
    glm::vec3 gPlaneScale(3.0f);
    glm::vec3 gCandleScale(1.0f);
    glm::vec3 gCubeScale(2.0f);
    glm::vec3 gBallScale(1.0f);

    
    // Pyramid and light color
    glm::vec3 gObjectColor(1.0f, 0.2f, 0.0f);
    glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 gFillObjectColor(1.0f, 0.2f, 0.0f);
    glm::vec3 gFillLightColor(0.0f, 1.0f, 0.0f);
    


    // Light position and scale
    glm::vec3 gLightPosition(-3.0f, 4.0f, 5.0f);
    glm::vec3 gLightScale(0.3f);
    glm::vec3 gFillLightPosition(3.0f, 4.0f, -5.0f);
    glm::vec3 gFillLightScale(0.3f);

    
}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);

// function for keyboard inputs
void UProcessInput(GLFWwindow* window);


// ----- functions to create meshes
void UCreateLampMesh(GLMesh& mesh);
void UCreateOuterMesh(GLMesh& mesh);
void UCreateInnerMesh(GLMesh& mesh);
void UCreatePlaneMesh(GLMesh& mesh);
void UCreateCandleMesh(GLMesh& mesh);
void UCreateCubeMesh(GLMesh& mesh);
void UCreateFillLampMesh(GLMesh& mesh);
void UCreateBallMesh(GLMesh& mesh);

// function to destroy mesh
void UDestroyMesh(GLMesh& mesh);

// render functions
void URenderP();
void URenderO();

// ----- functions to create textures
bool UCreateOuterTexture(const char* filename, GLuint& textureId);
bool UCreateInnerTexture(const char* filename, GLuint& textureId);
bool UCreatePlaneTexture(const char* filename, GLuint& textureId);
bool UCreateCandleTexture(const char* filename, GLuint& textureId);
bool UCreateCubeTexture(const char* filename, GLuint& textureId);
bool UCreateBallTexture(const char* filename, GLuint& textureId);
// function to destroy texture
void UDestroyTexture(GLuint textureId);

// shader program function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

// declare callback functions for mouse handling input
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);



/// ---------------------------- SHADERS -------------------------- ///

/* LAMP Vertex Shader Source Code*/
const GLchar* lampVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

        //Uniform / Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
}
);

/* LAMP Fragment Shader Source Code*/
const GLchar* lampFragmentShaderSource = GLSL(440,

    out vec4 fragmentColor; // For outgoing lamp color (smaller pyramid on the left) to the GPU

void main()
{
    fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
}
);

/* OUTER Vertex Shader Source Code*/
const GLchar* outerVertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
    layout(location = 2) in vec2 textureCoordinate;  // Color data from Vertex Attrib Pointer 1
    layout(location = 1) in vec3 normal; // VAP position 1 for normals
    
    out vec2 vertexTextureCoordinate; // variable to transfer color data to the fragment shader
    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
    
    //Global variables for the  transform matrices
    uniform mat4 model1;
    uniform mat4 view1;
    uniform mat4 projection1;

void main()
{
    gl_Position = projection1 * view1 * model1 * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexFragmentPos = vec3(model1 * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model1))) * normal; // get normal vector
    vertexTextureCoordinate = textureCoordinate; // references incoming color data
   
}
);


/* OUTER Fragment Shader Source Code*/
const GLchar* outerFragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; // Variable to hold incoming color data from vertex shader
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position
   
    out vec4 fragmentColor;

    // Uniform / Global variables for object texture, color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;

    uniform vec3 fillObjectColor;
    uniform vec3 fillLightColor;
    uniform vec3 fillLightPos;
    uniform vec3 viewPosition;
   

    uniform sampler2D uTextureOuter;
    uniform vec2 uvScale;
    

void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    // Calculate Ambient lighting for both light angles
    float ambientStrength = 0.3f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    float ambientStrength2 = 0.1f;
    vec3 ambient2 = ambientStrength2 * fillLightColor; // Generate ambient light color

    // Calculate Diffuse lighting for both light angles
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit

    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on pyramid
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos);
    float impact2 = max(dot(norm, fillLightDirection), 0.0);
    vec3 diffuse2 = impact2 * fillLightColor;

    // Calculate Specular lighting for both light angles
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction

    float specularIntensity = 0.8f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    float specularIntensity2 = 0.2f;
    float highlightSize2 = 8.0f;
    vec3 reflectDir2 = reflect(-fillLightDirection, norm);

    // Calculate specular component for both light angles
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    float specularComponent2 = pow(max(dot(viewDir, reflectDir2), 0.0), highlightSize2);
    vec3 specular2 = specularIntensity2 * specularComponent2 * fillLightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTextureOuter, vertexTextureCoordinate * uvScale);

    // Calculate phong result for both light angles
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;
    vec3 phong2 = (ambient2 + diffuse2 + specular2) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0) + vec4(phong2, 1.0); // combine the two light results and send them to GPU


    
}
);

/* INNER Vertex Shader Source Code*/
const GLchar* innerVertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
    layout(location = 2) in vec2 textureCoordinate;  // Color data from Vertex Attrib Pointer 1
    layout(location = 1) in vec3 normal; // VAP position 1 for normals

    out vec2 vertexTextureCoordinate; // variable to transfer color data to the fragment shader
    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader


    //Global variables for the  transform matrices
    uniform mat4 model2;
    uniform mat4 view2;
    uniform mat4 projection2;

void main()
{
    gl_Position = projection2 * view2 * model2 * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexFragmentPos = vec3(model2 * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model2))) * normal; // get normal vector
    vertexTextureCoordinate = textureCoordinate; // references incoming color data

}
);


/* INNER Fragment Shader Source Code*/
const GLchar* innerFragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; // Variable to hold incoming color data from vertex shader
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position

    out vec4 fragmentColor;

    // Uniform / Global variables for object texture, color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;

    uniform vec3 fillObjectColor;
    uniform vec3 fillLightColor;
    uniform vec3 fillLightPos;
    uniform vec3 viewPosition;


    uniform sampler2D uTextureInner;
    uniform vec2 uvScale;

void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    // Calculate Ambient lighting for both light angles
    float ambientStrength = 0.3f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    float ambientStrength2 = 0.1f;
    vec3 ambient2 = ambientStrength2 * fillLightColor; // Generate ambient light color

    // Calculate Diffuse lighting for both light angles
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit

    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on pyramid
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos);
    float impact2 = max(dot(norm, fillLightDirection), 0.0);
    vec3 diffuse2 = impact2 * fillLightColor;

    // Calculate Specular lighting for both light angles
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction

    float specularIntensity = 0.8f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    float specularIntensity2 = 0.2f;
    float highlightSize2 = 8.0f;
    vec3 reflectDir2 = reflect(-fillLightDirection, norm);

    // Calculate specular component for both light angles
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    float specularComponent2 = pow(max(dot(viewDir, reflectDir2), 0.0), highlightSize2);
    vec3 specular2 = specularIntensity2 * specularComponent2 * fillLightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTextureInner, vertexTextureCoordinate * uvScale);

    // Calculate phong result for both light angles
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;
    vec3 phong2 = (ambient2 + diffuse2 + specular2) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0) + vec4(phong2, 1.0); // combine the two light results and send them to GPU




}
);

/* Plane Vertex Shader Source Code*/
const GLchar* planeVertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
    layout(location = 2) in vec2 textureCoordinate;  // Color data from Vertex Attrib Pointer 1
    layout(location = 1) in vec3 normal; // VAP position 1 for normals

    out vec2 vertexTextureCoordinate; // variable to transfer color data to the fragment shader
    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader


    //Global variables for the  transform matrices
    uniform mat4 model3;
    uniform mat4 view3;
    uniform mat4 projection3;

void main()
{
    gl_Position = projection3 * view3 * model3 * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexFragmentPos = vec3(model3 * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model3))) * normal; // get normal vector
    vertexTextureCoordinate = textureCoordinate; // references incoming color data

}
);


/* PLANE Fragment Shader Source Code*/
const GLchar* planeFragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; // Variable to hold incoming color data from vertex shader
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position

    out vec4 fragmentColor;

    // Uniform / Global variables for object texture, color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;

    uniform vec3 fillObjectColor;
    uniform vec3 fillLightColor;
    uniform vec3 fillLightPos;
    uniform vec3 viewPosition;


    uniform sampler2D uTexturePlane;
    uniform vec2 uvScale;


void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    // Calculate Ambient lighting for both light angles
    float ambientStrength = 0.3f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    float ambientStrength2 = 0.1f;
    vec3 ambient2 = ambientStrength2 * fillLightColor; // Generate ambient light color

    // Calculate Diffuse lighting for both light angles
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit

    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on pyramid
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos);
    float impact2 = max(dot(norm, fillLightDirection), 0.0);
    vec3 diffuse2 = impact2 * fillLightColor;

    // Calculate Specular lighting for both light angles
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction

    float specularIntensity = 0.8f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    float specularIntensity2 = 0.2f;
    float highlightSize2 = 8.0f;
    vec3 reflectDir2 = reflect(-fillLightDirection, norm);

    // Calculate specular component for both light angles
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    float specularComponent2 = pow(max(dot(viewDir, reflectDir2), 0.0), highlightSize2);
    vec3 specular2 = specularIntensity2 * specularComponent2 * fillLightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTexturePlane, vertexTextureCoordinate * uvScale);

    // Calculate phong result for both light angles
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;
    vec3 phong2 = (ambient2 + diffuse2 + specular2) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0) + vec4(phong2, 1.0); // combine the two light results and send them to GPU


}
);


/* CANDLE Vertex Shader Source Code*/
const GLchar* candleVertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
    layout(location = 2) in vec2 textureCoordinate;  // Color data from Vertex Attrib Pointer 1
    layout(location = 1) in vec3 normal; // VAP position 1 for normals

    out vec2 vertexTextureCoordinate; // variable to transfer color data to the fragment shader
    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader


    //Global variables for the  transform matrices
    uniform mat4 model4;
    uniform mat4 view4;
    uniform mat4 projection4;

void main()
{
    gl_Position = projection4 * view4 * model4 * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexFragmentPos = vec3(model4 * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model4))) * normal; // get normal vector
    vertexTextureCoordinate = textureCoordinate; // references incoming color data

}
);


/* CANDLE Fragment Shader Source Code*/
const GLchar* candleFragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; // Variable to hold incoming color data from vertex shader
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position

    out vec4 fragmentColor;

    // Uniform / Global variables for object texture, color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;

    uniform vec3 fillObjectColor;
    uniform vec3 fillLightColor;
    uniform vec3 fillLightPos;
    uniform vec3 viewPosition;


    uniform sampler2D uTextureCandle;
    uniform vec2 uvScale;


void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    // Calculate Ambient lighting for both light angles
    float ambientStrength = 0.3f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    float ambientStrength2 = 0.1f;
    vec3 ambient2 = ambientStrength2 * fillLightColor; // Generate ambient light color

    // Calculate Diffuse lighting for both light angles
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit

    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on pyramid
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos);
    float impact2 = max(dot(norm, fillLightDirection), 0.0);
    vec3 diffuse2 = impact2 * fillLightColor;

    // Calculate Specular lighting for both light angles
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction

    float specularIntensity = 0.8f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    float specularIntensity2 = 0.2f;
    float highlightSize2 = 8.0f;
    vec3 reflectDir2 = reflect(-fillLightDirection, norm);

    // Calculate specular component for both light angles
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    float specularComponent2 = pow(max(dot(viewDir, reflectDir2), 0.0), highlightSize2);
    vec3 specular2 = specularIntensity2 * specularComponent2 * fillLightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTextureCandle, vertexTextureCoordinate * uvScale);

    // Calculate phong result for both light angles
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;
    vec3 phong2 = (ambient2 + diffuse2 + specular2) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0) + vec4(phong2, 1.0); // combine the two light results and send them to GPU



}
);

/* CUBE Vertex Shader Source Code*/
const GLchar* cubeVertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
    layout(location = 2) in vec2 textureCoordinate;  // Color data from Vertex Attrib Pointer 1
    layout(location = 1) in vec3 normal; // VAP position 1 for normals

    out vec2 vertexTextureCoordinate; // variable to transfer color data to the fragment shader
    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader


    //Global variables for the  transform matrices
    uniform mat4 model5;
    uniform mat4 view5;
    uniform mat4 projection5;

void main()
{
    gl_Position = projection5 * view5 * model5 * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexFragmentPos = vec3(model5 * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model5))) * normal; // get normal vector
    vertexTextureCoordinate = textureCoordinate; // references incoming color data

}
);


/* CUBE Fragment Shader Source Code*/
const GLchar* cubeFragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; // Variable to hold incoming color data from vertex shader
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position

    out vec4 fragmentColor;

    // Uniform / Global variables for object texture, color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;

    uniform vec3 fillObjectColor;
    uniform vec3 fillLightColor;
    uniform vec3 fillLightPos;
    uniform vec3 viewPosition;


    uniform sampler2D uTextureCube;
    uniform vec2 uvScale;

void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    // Calculate Ambient lighting for both light angles
    float ambientStrength = 0.3f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    float ambientStrength2 = 0.1f;
    vec3 ambient2 = ambientStrength2 * fillLightColor; // Generate ambient light color

    // Calculate Diffuse lighting for both light angles
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit

    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on pyramid
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos);
    float impact2 = max(dot(norm, fillLightDirection), 0.0);
    vec3 diffuse2 = impact2 * fillLightColor;

    // Calculate Specular lighting for both light angles
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction

    float specularIntensity = 0.8f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    float specularIntensity2 = 0.2f;
    float highlightSize2 = 8.0f;
    vec3 reflectDir2 = reflect(-fillLightDirection, norm);

    // Calculate specular component for both light angles
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    float specularComponent2 = pow(max(dot(viewDir, reflectDir2), 0.0), highlightSize2);
    vec3 specular2 = specularIntensity2 * specularComponent2 * fillLightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTextureCube, vertexTextureCoordinate * uvScale);

    // Calculate phong result for both light angles
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;
    vec3 phong2 = (ambient2 + diffuse2 + specular2) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0) + vec4(phong2, 1.0); // combine the two light results and send them to GPU

}
);

/* BALL Vertex Shader Source Code*/
const GLchar* ballVertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
    layout(location = 2) in vec2 textureCoordinate;  // Color data from Vertex Attrib Pointer 1
    layout(location = 1) in vec3 normal; // VAP position 1 for normals

    out vec2 vertexTextureCoordinate; // variable to transfer color data to the fragment shader
    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader


    //Global variables for the  transform matrices
    uniform mat4 model6;
    uniform mat4 view6;
    uniform mat4 projection6;

void main()
{
    gl_Position = projection6 * view6 * model6 * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexFragmentPos = vec3(model6 * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model6))) * normal; // get normal vector
    vertexTextureCoordinate = textureCoordinate; // references incoming color data

}
);


/* BALL Fragment Shader Source Code*/
const GLchar* ballFragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; // Variable to hold incoming color data from vertex shader
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position

    out vec4 fragmentColor;

    // Uniform / Global variables for object texture, color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;

    uniform vec3 fillObjectColor;
    uniform vec3 fillLightColor;
    uniform vec3 fillLightPos;
    uniform vec3 viewPosition;


    uniform sampler2D uTextureBall;
    uniform vec2 uvScale;


void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    // Calculate Ambient lighting for both light angles
    float ambientStrength = 0.3f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    float ambientStrength2 = 0.1f;
    vec3 ambient2 = ambientStrength2 * fillLightColor; // Generate ambient light color

    // Calculate Diffuse lighting for both light angles
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit

    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on pyramid
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos);
    float impact2 = max(dot(norm, fillLightDirection), 0.0);
    vec3 diffuse2 = impact2 * fillLightColor;

    // Calculate Specular lighting for both light angles
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction

    float specularIntensity = 0.8f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector

    float specularIntensity2 = 0.2f;
    float highlightSize2 = 8.0f;
    vec3 reflectDir2 = reflect(-fillLightDirection, norm);

    // Calculate specular component for both light angles
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    float specularComponent2 = pow(max(dot(viewDir, reflectDir2), 0.0), highlightSize2);
    vec3 specular2 = specularIntensity2 * specularComponent2 * fillLightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTextureBall, vertexTextureCoordinate * uvScale);

    // Calculate phong result for both light angles
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;
    vec3 phong2 = (ambient2 + diffuse2 + specular2) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0) + vec4(phong2, 1.0); // combine the two light results and send them to GPU

}
);


/* FILL LAMP Vertex Shader Source Code*/
const GLchar* fillLampVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

        //Uniform / Global variables for the  transform matrices
uniform mat4 model7;
uniform mat4 view7;
uniform mat4 projection7;

void main()
{
    gl_Position = projection7 * view7 * model7 * vec4(position, 1.0f); // Transforms vertices into clip coordinates
}
);
/* FILL LAMP Fragment Shader Source Code*/
const GLchar* fillLampFragmentShaderSource = GLSL(440,

    out vec4 fragmentColor; // For outgoing lamp color (smaller pyramid on the right) to the GPU

void main()
{
    fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
}
);


// -------------------------------


// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}



// ------------------ MAIN ---------------------------  //
// ---------------------------------------------------  //

int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the OUTER mesh and shader program
    UCreateOuterMesh(gMeshOuter); 
    if (!UCreateShaderProgram(outerVertexShaderSource, outerFragmentShaderSource, gProgramIdOuter))
        return EXIT_FAILURE;
                                  
    // Create the INNER mesh and shader program
    UCreateInnerMesh(gMeshInner);
    if (!UCreateShaderProgram(innerVertexShaderSource, innerFragmentShaderSource, gProgramIdInner))
        return EXIT_FAILURE;

    // Create the PLANE mesh and shader program
    UCreatePlaneMesh(gMeshPlane);
    if (!UCreateShaderProgram(planeVertexShaderSource, planeFragmentShaderSource, gProgramIdPlane))
        return EXIT_FAILURE;

    // Create the LAMP mesh and shader program
    UCreateLampMesh(gMesh);
    if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, gLampProgramId))
        return EXIT_FAILURE;
    UCreateFillLampMesh(gMeshFillLamp);
    if (!UCreateShaderProgram(fillLampVertexShaderSource, fillLampFragmentShaderSource, gFillLampProgramId))
        return EXIT_FAILURE;


    // Create the CANDLE mesh and shader program
    UCreateCandleMesh(gMeshCandle);
    if (!UCreateShaderProgram(candleVertexShaderSource, candleFragmentShaderSource, gProgramIdCandle))
        return EXIT_FAILURE;

    // Create the CUBE mesh and shader program
    UCreateCubeMesh(gMeshCube);
    if (!UCreateShaderProgram(cubeVertexShaderSource, cubeFragmentShaderSource, gProgramIdCube))
        return EXIT_FAILURE;

    // Create the BALL mesh and shader program
    UCreateBallMesh(gMeshBall);
    if (!UCreateShaderProgram(ballVertexShaderSource, ballFragmentShaderSource, gProgramIdBall))
        return EXIT_FAILURE;

   
    /// --------- LOAD TEXTURES ------------- ///

    // Load texture for the OUTER CASE
    const char* texFilename = "liquid.jpg";
    if (!UCreateOuterTexture(texFilename, gTextureIdOuter))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // Load texture for the INNER CASE
    const char* texFilename2 = "mosaic.jpg";
    if (!UCreateInnerTexture(texFilename2, gTextureIdInner))
    {
        cout << "Failed to load texture " << texFilename2 << endl;
        return EXIT_FAILURE;
    }
    // Load texture for the PLANE
    const char* texFilename3 = "tableTop.jpg";
    if (!UCreatePlaneTexture(texFilename3, gTextureIdPlane))
    {
        cout << "Failed to load texture " << texFilename3 << endl;
        return EXIT_FAILURE;
    }
    // Load texture for the CANDLE
    texFilename = "mosaic.jpg";
    if (!UCreateCandleTexture(texFilename, gTextureIdCandle))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // Load texture for the CUBE
    const char* texFilename5 = "wallpaper.jpg";
    if (!UCreateCubeTexture(texFilename5, gTextureIdCube))
    {
        cout << "Failed to load texture " << texFilename5 << endl;
        return EXIT_FAILURE;
    }
    // Load texture for the BALL
    const char* texFilename6 = "mosaic.jpg";
    if (!UCreateBallTexture(texFilename6, gTextureIdBall))
    {
        cout << "Failed to load texture " << texFilename6 << endl;
        return EXIT_FAILURE;
    }
    
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gProgramIdOuter);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gProgramIdOuter, "uTextureOuter"), 0);
    glUseProgram(gProgramIdInner);
    // We set the texture as texture unit 1
    glUniform1i(glGetUniformLocation(gProgramIdInner, "uTextureInner"), 1);
    glUseProgram(gProgramIdPlane);
    // We set the texture as texture unit 2
    glUniform1i(glGetUniformLocation(gProgramIdPlane, "uTexturePlane"), 2);
    glUseProgram(gProgramIdCandle);
    // We set the texture as texture unit 3
    glUniform1i(glGetUniformLocation(gProgramIdCandle, "uTextureCandle"), 3);
    glUseProgram(gProgramIdCube);
    // We set the texture as texture unit 3
    glUniform1i(glGetUniformLocation(gProgramIdCube, "uTextureCube"), 4);
    glUseProgram(gProgramIdBall);
    // We set the texture as texture unit 3
    glUniform1i(glGetUniformLocation(gProgramIdBall, "uTextureBall"), 5);


    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        
        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;


        // input
        // -----
        UProcessInput(gWindow);

        
        // Render in perspective  or ortho
        if (perspective) {
            // Render this frame in perspective
            URenderP();
        }
        else {
            // render this frame in ortho
            URenderO();
        }

        

        glfwPollEvents();
    }

    // Clean up
    {
        // Release mesh data
        UDestroyMesh(gMeshOuter);
        UDestroyMesh(gMeshInner);
        UDestroyMesh(gMeshPlane);
        UDestroyMesh(gMesh);
        UDestroyMesh(gMeshCandle);
        UDestroyMesh(gMeshCube);
        UDestroyMesh(gMeshBall);
        UDestroyMesh(gMeshFillLamp);

        // Release texture
        UDestroyTexture(gTextureIdPlane);
        UDestroyTexture(gTextureIdOuter);
        UDestroyTexture(gTextureIdInner);
        UDestroyTexture(gTextureIdCandle);
        UDestroyTexture(gTextureIdCube);
        UDestroyTexture(gTextureIdBall);


        // Release shader program
        UDestroyShaderProgram(gProgramIdOuter);
        UDestroyShaderProgram(gProgramIdInner);
        UDestroyShaderProgram(gProgramIdPlane);
        UDestroyShaderProgram(gLampProgramId);
        UDestroyShaderProgram(gProgramIdCandle);
        UDestroyShaderProgram(gProgramIdCube);
        UDestroyShaderProgram(gProgramIdBall);
        UDestroyShaderProgram(gFillLampProgramId);
    }

    exit(EXIT_SUCCESS); // Terminates the program successfully
}



// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);

    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{
    static const float cameraSpeed = 2.5f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Keyboard control declarations
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)     // W - FORWARD
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)     // S - BACKWARDS
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)     // A - LEFT
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)     // D - RIGHT
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)     // E - UP
        gCamera.ProcessKeyboard(UP, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)     // Q - DOWN
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)     // P - Alternate between ortho and perspective projection
        if (perspective == true) {
            perspective = false;
        }
        else {
            perspective = true;
        }
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// glfw: Whenever the mouse moves, this callback is called.
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // Reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Whenever the mouse scroll wheel scrolls, this callback is called.
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

// glfw: handle mouse button events
// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}

// Render in Perspective
void URenderP()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Creates a perspective projection
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    
    //---------------------------------- Draw the OUTER CASE:
    // ------------------------------------------------------
    
        // Set the shader to be used
        glUseProgram(gProgramIdOuter);

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 model = glm::translate(gObjectPosition) * glm::scale(gObjectScale) * glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        // camera/view transformation
        glm::mat4 view = gCamera.GetViewMatrix();

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshOuter.vao1);

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc1 = glGetUniformLocation(gProgramIdOuter, "model1");
        GLint viewLoc1 = glGetUniformLocation(gProgramIdOuter, "view1");
        GLint projLoc1 = glGetUniformLocation(gProgramIdOuter, "projection1");

        glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc1, 1, GL_FALSE, glm::value_ptr(projection));

        // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
        GLint objectColorLoc = glGetUniformLocation(gProgramIdOuter, "objectColor");
        GLint lightColorLoc = glGetUniformLocation(gProgramIdOuter, "lightColor");
        GLint lightPositionLoc = glGetUniformLocation(gProgramIdOuter, "lightPos");

        GLint fillObjectColorLoc = glGetUniformLocation(gProgramIdOuter, "fillObjectColor");
        GLint fillLightColorLoc = glGetUniformLocation(gProgramIdOuter, "fillLightColor");
        GLint fillLightPositionLoc = glGetUniformLocation(gProgramIdOuter, "fillLightPos");

        GLint viewPositionLoc = glGetUniformLocation(gProgramIdOuter, "viewPosition");

        // Pass color, light, and camera data to the Shader program's corresponding uniforms
        glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
        glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
        glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

        glUniform3f(fillObjectColorLoc, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
        glUniform3f(fillLightColorLoc, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
        glUniform3f(fillLightPositionLoc, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

        const glm::vec3 cameraPosition = gCamera.Position;
        glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        GLint UVScaleLoc = glGetUniformLocation(gProgramIdOuter, "uvScale");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gTextureIdOuter);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMeshOuter.nVertices); // Draws the triangle
    

    //---------------------------------- Draw the INNER CASE:
    // ------------------------------------------------------
    
        // Set the shader to be used
        glUseProgram(gProgramIdInner);

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshInner.vao2);

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc2 = glGetUniformLocation(gProgramIdInner, "model2");
        GLint viewLoc2 = glGetUniformLocation(gProgramIdInner, "view2");
        GLint projLoc2 = glGetUniformLocation(gProgramIdInner, "projection2");

        glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection));

        // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
        GLint objectColorLoc2 = glGetUniformLocation(gProgramIdInner, "objectColor");
        GLint lightColorLoc2 = glGetUniformLocation(gProgramIdInner, "lightColor");
        GLint lightPositionLoc2 = glGetUniformLocation(gProgramIdInner, "lightPos");

        GLint fillObjectColorLoc2 = glGetUniformLocation(gProgramIdInner, "fillObjectColor");
        GLint fillLightColorLoc2 = glGetUniformLocation(gProgramIdInner, "fillLightColor");
        GLint fillLightPositionLoc2 = glGetUniformLocation(gProgramIdInner, "fillLightPos");

        GLint viewPositionLoc2 = glGetUniformLocation(gProgramIdInner, "viewPosition");

        // Pass color, light, and camera data to the Shader program's corresponding uniforms
        glUniform3f(objectColorLoc2, gObjectColor.r, gObjectColor.g, gObjectColor.b);
        glUniform3f(lightColorLoc2, gLightColor.r, gLightColor.g, gLightColor.b);
        glUniform3f(lightPositionLoc2, gLightPosition.x, gLightPosition.y, gLightPosition.z);

        glUniform3f(fillObjectColorLoc2, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
        glUniform3f(fillLightColorLoc2, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
        glUniform3f(fillLightPositionLoc2, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

        //const glm::vec3 cameraPosition = gCamera.Position;
        glUniform3f(viewPositionLoc2, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        GLint UVScaleLoc2 = glGetUniformLocation(gProgramIdInner, "uvScale");
        glUniform2fv(UVScaleLoc2, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gTextureIdInner);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMeshInner.nVertices); // Draws the triangle
    

    // -------------------------------------- Draw the PLANE:
    // ------------------------------------------------------
    
        // Set the shader to be used
        glUseProgram(gProgramIdPlane);

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 modelPlane = glm::translate(gPlanePosition) * glm::scale(gPlaneScale);

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshPlane.vao3);

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc3 = glGetUniformLocation(gProgramIdPlane, "model3");
        GLint viewLoc3 = glGetUniformLocation(gProgramIdPlane, "view3");
        GLint projLoc3 = glGetUniformLocation(gProgramIdPlane, "projection3");

        glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(modelPlane));
        glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc3, 1, GL_FALSE, glm::value_ptr(projection));

        // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
        GLint objectColorLoc3 = glGetUniformLocation(gProgramIdPlane, "objectColor");
        GLint lightColorLoc3 = glGetUniformLocation(gProgramIdPlane, "lightColor");
        GLint lightPositionLoc3 = glGetUniformLocation(gProgramIdPlane, "lightPos");

        GLint fillObjectColorLoc3 = glGetUniformLocation(gProgramIdPlane, "fillObjectColor");
        GLint fillLightColorLoc3 = glGetUniformLocation(gProgramIdPlane, "fillLightColor");
        GLint fillLightPositionLoc3 = glGetUniformLocation(gProgramIdPlane, "fillLightPos");

        GLint viewPositionLoc3 = glGetUniformLocation(gProgramIdPlane, "viewPosition");

        // Pass color, light, and camera data to the Shader program's corresponding uniforms
        glUniform3f(objectColorLoc3, gObjectColor.r, gObjectColor.g, gObjectColor.b);
        glUniform3f(lightColorLoc3, gLightColor.r, gLightColor.g, gLightColor.b);
        glUniform3f(lightPositionLoc3, gLightPosition.x, gLightPosition.y, gLightPosition.z);

        glUniform3f(fillObjectColorLoc3, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
        glUniform3f(fillLightColorLoc3, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
        glUniform3f(fillLightPositionLoc3, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

        //const glm::vec3 cameraPosition = gCamera.Position;
        glUniform3f(viewPositionLoc3, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        GLint UVScaleLoc3 = glGetUniformLocation(gProgramIdPlane, "uvScale");
        glUniform2fv(UVScaleLoc3, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gTextureIdPlane);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMeshPlane.nVertices); // Draws the triangle
    

    // ------------------------------------- Draw the CANDLE:
    // ------------------------------------------------------
    
        // Set the shader to be used
        glUseProgram(gProgramIdCandle);

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 modelCandle = glm::translate(gCandlePosition) * glm::scale(gCandleScale) * glm::rotate(330.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshCandle.vao4);

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc4 = glGetUniformLocation(gProgramIdCandle, "model4");
        GLint viewLoc4 = glGetUniformLocation(gProgramIdCandle, "view4");
        GLint projLoc4 = glGetUniformLocation(gProgramIdCandle, "projection4");

        glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(modelCandle));
        glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc4, 1, GL_FALSE, glm::value_ptr(projection));

        // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
        GLint objectColorLoc4 = glGetUniformLocation(gProgramIdCandle, "objectColor");
        GLint lightColorLoc4 = glGetUniformLocation(gProgramIdCandle, "lightColor");
        GLint lightPositionLoc4 = glGetUniformLocation(gProgramIdCandle, "lightPos");

        GLint fillObjectColorLoc4 = glGetUniformLocation(gProgramIdCandle, "fillObjectColor");
        GLint fillLightColorLoc4 = glGetUniformLocation(gProgramIdCandle, "fillLightColor");
        GLint fillLightPositionLoc4 = glGetUniformLocation(gProgramIdCandle, "fillLightPos");

        GLint viewPositionLoc4 = glGetUniformLocation(gProgramIdCandle, "viewPosition");

        // Pass color, light, and camera data to the Shader program's corresponding uniforms
        glUniform3f(objectColorLoc4, gObjectColor.r, gObjectColor.g, gObjectColor.b);
        glUniform3f(lightColorLoc4, gLightColor.r, gLightColor.g, gLightColor.b);
        glUniform3f(lightPositionLoc4, gLightPosition.x, gLightPosition.y, gLightPosition.z);

        glUniform3f(fillObjectColorLoc4, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
        glUniform3f(fillLightColorLoc4, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
        glUniform3f(fillLightPositionLoc4, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

        //const glm::vec3 cameraPosition = gCamera.Position;
        glUniform3f(viewPositionLoc4, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        GLint UVScaleLoc4 = glGetUniformLocation(gProgramIdCandle, "uvScale");
        glUniform2fv(UVScaleLoc4, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gTextureIdCandle);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMeshCandle.nVertices); // Draws the triangle
    

        // ------------------------------------- Draw the CUBE:
        // ----------------------------------------------------

        // Set the shader to be used
        glUseProgram(gProgramIdCube);

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 modelCube = glm::translate(gCubePosition) * glm::scale(gCubeScale) * glm::rotate(225.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshCube.vao5);

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc5 = glGetUniformLocation(gProgramIdCube, "model5");
        GLint viewLoc5 = glGetUniformLocation(gProgramIdCube, "view5");
        GLint projLoc5 = glGetUniformLocation(gProgramIdCube, "projection5");

        glUniformMatrix4fv(modelLoc5, 1, GL_FALSE, glm::value_ptr(modelCube));
        glUniformMatrix4fv(viewLoc5, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc5, 1, GL_FALSE, glm::value_ptr(projection));

        // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
        GLint objectColorLoc5 = glGetUniformLocation(gProgramIdCube, "objectColor");
        GLint lightColorLoc5 = glGetUniformLocation(gProgramIdCube, "lightColor");
        GLint lightPositionLoc5 = glGetUniformLocation(gProgramIdCube, "lightPos");

        GLint fillObjectColorLoc5 = glGetUniformLocation(gProgramIdCube, "fillObjectColor");
        GLint fillLightColorLoc5 = glGetUniformLocation(gProgramIdCube, "fillLightColor");
        GLint fillLightPositionLoc5 = glGetUniformLocation(gProgramIdCube, "fillLightPos");

        GLint viewPositionLoc5 = glGetUniformLocation(gProgramIdCube, "viewPosition");

        // Pass color, light, and camera data to the Shader program's corresponding uniforms
        glUniform3f(objectColorLoc5, gObjectColor.r, gObjectColor.g, gObjectColor.b);
        glUniform3f(lightColorLoc5, gLightColor.r, gLightColor.g, gLightColor.b);
        glUniform3f(lightPositionLoc5, gLightPosition.x, gLightPosition.y, gLightPosition.z);

        glUniform3f(fillObjectColorLoc5, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
        glUniform3f(fillLightColorLoc5, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
        glUniform3f(fillLightPositionLoc5, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

        //const glm::vec3 cameraPosition = gCamera.Position;
        glUniform3f(viewPositionLoc5, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        GLint UVScaleLoc5 = glGetUniformLocation(gProgramIdCube, "uvScale");
        glUniform2fv(UVScaleLoc5, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, gTextureIdCube);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMeshCube.nVertices); // Draws the triangle



    //---------------------------------------- Draw the LAMP: 
    //-------------------------------------------------------
    
        glUseProgram(gLampProgramId);

        //Transform the smaller pyramid used as a visual que for the light source
        model = glm::translate(gLightPosition) * glm::scale(gLightScale);

        // Reference matrix uniforms from the Lamp Shader program
        modelLoc3 = glGetUniformLocation(gLampProgramId, "model");
        viewLoc3 = glGetUniformLocation(gLampProgramId, "view");
        projLoc3 = glGetUniformLocation(gLampProgramId, "projection");

        // Pass matrix data to the Lamp Shader program's matrix uniforms
        glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc3, 1, GL_FALSE, glm::value_ptr(projection));

        glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    
        //------------------------------ FILL LAMP: draw Fill lamp
        //--------------------------------------------------------
        glUseProgram(gFillLampProgramId);

        //Transform the smaller pyramid used as a visual que for the light source
        model = glm::translate(gFillLightPosition) * glm::scale(gFillLightScale);

        // Reference matrix uniforms from the Lamp Shader program
        GLuint modelLoc7 = glGetUniformLocation(gFillLampProgramId, "model7");
        GLuint viewLoc7 = glGetUniformLocation(gFillLampProgramId, "view7");
        GLuint projLoc7 = glGetUniformLocation(gFillLampProgramId, "projection7");

        // Pass matrix data to the Lamp Shader program's matrix uniforms
        glUniformMatrix4fv(modelLoc7, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc7, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc7, 1, GL_FALSE, glm::value_ptr(projection));

        glDrawArrays(GL_TRIANGLES, 0, gMeshFillLamp.nVertices);


        // ------------------------------------- Draw the BALL:
        // ----------------------------------------------------

        // Set the shader to be used
        glUseProgram(gProgramIdBall);

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 modelBall = glm::translate(gBallPosition) * glm::scale(gBallScale) * glm::rotate(225.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshBall.vao6);

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc6 = glGetUniformLocation(gProgramIdBall, "model6");
        GLint viewLoc6 = glGetUniformLocation(gProgramIdBall, "view6");
        GLint projLoc6 = glGetUniformLocation(gProgramIdBall, "projection6");

        glUniformMatrix4fv(modelLoc6, 1, GL_FALSE, glm::value_ptr(modelCube));
        glUniformMatrix4fv(viewLoc6, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc6, 1, GL_FALSE, glm::value_ptr(projection));

        // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
        GLint objectColorLoc6 = glGetUniformLocation(gProgramIdBall, "objectColor");
        GLint lightColorLoc6 = glGetUniformLocation(gProgramIdBall, "lightColor");
        GLint lightPositionLoc6 = glGetUniformLocation(gProgramIdBall, "lightPos");

        GLint fillObjectColorLoc6 = glGetUniformLocation(gProgramIdBall, "fillObjectColor");
        GLint fillLightColorLoc6 = glGetUniformLocation(gProgramIdBall, "fillLightColor");
        GLint fillLightPositionLoc6 = glGetUniformLocation(gProgramIdBall, "fillLightPos");

        GLint viewPositionLoc6 = glGetUniformLocation(gProgramIdBall, "viewPosition");

        // Pass color, light, and camera data to the Shader program's corresponding uniforms
        glUniform3f(objectColorLoc6, gObjectColor.r, gObjectColor.g, gObjectColor.b);
        glUniform3f(lightColorLoc6, gLightColor.r, gLightColor.g, gLightColor.b);
        glUniform3f(lightPositionLoc6, gLightPosition.x, gLightPosition.y, gLightPosition.z);

        glUniform3f(fillObjectColorLoc6, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
        glUniform3f(fillLightColorLoc6, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
        glUniform3f(fillLightPositionLoc6, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

        //const glm::vec3 cameraPosition = gCamera.Position;
        glUniform3f(viewPositionLoc6, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        GLint UVScaleLoc6 = glGetUniformLocation(gProgramIdBall, "uvScale");
        glUniform2fv(UVScaleLoc6, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, gTextureIdBall);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMeshBall.nVertices); // Draws the triangle

        //------------------------------------------------------------------------------
        
        // Deactivate the Vertex Array Object
        glBindVertexArray(0);
        glUseProgram(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.


}
// Render in Ortho
void URenderO()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create an ortho projection
    glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -4.0f, 5.0f, 0.1f, 100.0f);


    //---------------------------------- Draw the OUTER CASE:
    // ------------------------------------------------------

        // Set the shader to be used
    glUseProgram(gProgramIdOuter);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = glm::translate(gObjectPosition) * glm::scale(gObjectScale) * glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMeshOuter.vao1);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc1 = glGetUniformLocation(gProgramIdOuter, "model1");
    GLint viewLoc1 = glGetUniformLocation(gProgramIdOuter, "view1");
    GLint projLoc1 = glGetUniformLocation(gProgramIdOuter, "projection1");

    glUniformMatrix4fv(modelLoc1, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc1, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
    GLint objectColorLoc = glGetUniformLocation(gProgramIdOuter, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(gProgramIdOuter, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(gProgramIdOuter, "lightPos");

    GLint fillObjectColorLoc = glGetUniformLocation(gProgramIdOuter, "fillObjectColor");
    GLint fillLightColorLoc = glGetUniformLocation(gProgramIdOuter, "fillLightColor");
    GLint fillLightPositionLoc = glGetUniformLocation(gProgramIdOuter, "fillLightPos");

    GLint viewPositionLoc = glGetUniformLocation(gProgramIdOuter, "viewPosition");

    // Pass color, light, and camera data to the Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    glUniform3f(fillObjectColorLoc, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
    glUniform3f(fillLightColorLoc, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
    glUniform3f(fillLightPositionLoc, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(gProgramIdOuter, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureIdOuter);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMeshOuter.nVertices); // Draws the triangle


//---------------------------------- Draw the INNER CASE:
// ------------------------------------------------------

    // Set the shader to be used
    glUseProgram(gProgramIdInner);

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMeshInner.vao2);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc2 = glGetUniformLocation(gProgramIdInner, "model2");
    GLint viewLoc2 = glGetUniformLocation(gProgramIdInner, "view2");
    GLint projLoc2 = glGetUniformLocation(gProgramIdInner, "projection2");

    glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc2, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc2, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
    GLint objectColorLoc2 = glGetUniformLocation(gProgramIdInner, "objectColor");
    GLint lightColorLoc2 = glGetUniformLocation(gProgramIdInner, "lightColor");
    GLint lightPositionLoc2 = glGetUniformLocation(gProgramIdInner, "lightPos");

    GLint fillObjectColorLoc2 = glGetUniformLocation(gProgramIdInner, "fillObjectColor");
    GLint fillLightColorLoc2 = glGetUniformLocation(gProgramIdInner, "fillLightColor");
    GLint fillLightPositionLoc2 = glGetUniformLocation(gProgramIdInner, "fillLightPos");

    GLint viewPositionLoc2 = glGetUniformLocation(gProgramIdInner, "viewPosition");

    // Pass color, light, and camera data to the Shader program's corresponding uniforms
    glUniform3f(objectColorLoc2, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc2, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc2, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    glUniform3f(fillObjectColorLoc2, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
    glUniform3f(fillLightColorLoc2, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
    glUniform3f(fillLightPositionLoc2, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

    //const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc2, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc2 = glGetUniformLocation(gProgramIdInner, "uvScale");
    glUniform2fv(UVScaleLoc2, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gTextureIdInner);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMeshInner.nVertices); // Draws the triangle


// -------------------------------------- Draw the PLANE:
// ------------------------------------------------------

    // Set the shader to be used
    glUseProgram(gProgramIdPlane);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 modelPlane = glm::translate(gPlanePosition) * glm::scale(gPlaneScale);

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMeshPlane.vao3);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc3 = glGetUniformLocation(gProgramIdPlane, "model3");
    GLint viewLoc3 = glGetUniformLocation(gProgramIdPlane, "view3");
    GLint projLoc3 = glGetUniformLocation(gProgramIdPlane, "projection3");

    glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(modelPlane));
    glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc3, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
    GLint objectColorLoc3 = glGetUniformLocation(gProgramIdPlane, "objectColor");
    GLint lightColorLoc3 = glGetUniformLocation(gProgramIdPlane, "lightColor");
    GLint lightPositionLoc3 = glGetUniformLocation(gProgramIdPlane, "lightPos");

    GLint fillObjectColorLoc3 = glGetUniformLocation(gProgramIdPlane, "fillObjectColor");
    GLint fillLightColorLoc3 = glGetUniformLocation(gProgramIdPlane, "fillLightColor");
    GLint fillLightPositionLoc3 = glGetUniformLocation(gProgramIdPlane, "fillLightPos");

    GLint viewPositionLoc3 = glGetUniformLocation(gProgramIdPlane, "viewPosition");

    // Pass color, light, and camera data to the Shader program's corresponding uniforms
    glUniform3f(objectColorLoc3, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc3, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc3, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    glUniform3f(fillObjectColorLoc3, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
    glUniform3f(fillLightColorLoc3, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
    glUniform3f(fillLightPositionLoc3, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

    //const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc3, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc3 = glGetUniformLocation(gProgramIdPlane, "uvScale");
    glUniform2fv(UVScaleLoc3, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gTextureIdPlane);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMeshPlane.nVertices); // Draws the triangle


// ------------------------------------- Draw the CANDLE:
// ------------------------------------------------------

    // Set the shader to be used
    glUseProgram(gProgramIdCandle);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 modelCandle = glm::translate(gCandlePosition) * glm::scale(gCandleScale) * glm::rotate(330.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMeshCandle.vao4);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc4 = glGetUniformLocation(gProgramIdCandle, "model4");
    GLint viewLoc4 = glGetUniformLocation(gProgramIdCandle, "view4");
    GLint projLoc4 = glGetUniformLocation(gProgramIdCandle, "projection4");

    glUniformMatrix4fv(modelLoc4, 1, GL_FALSE, glm::value_ptr(modelCandle));
    glUniformMatrix4fv(viewLoc4, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc4, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
    GLint objectColorLoc4 = glGetUniformLocation(gProgramIdCandle, "objectColor");
    GLint lightColorLoc4 = glGetUniformLocation(gProgramIdCandle, "lightColor");
    GLint lightPositionLoc4 = glGetUniformLocation(gProgramIdCandle, "lightPos");

    GLint fillObjectColorLoc4 = glGetUniformLocation(gProgramIdCandle, "fillObjectColor");
    GLint fillLightColorLoc4 = glGetUniformLocation(gProgramIdCandle, "fillLightColor");
    GLint fillLightPositionLoc4 = glGetUniformLocation(gProgramIdCandle, "fillLightPos");

    GLint viewPositionLoc4 = glGetUniformLocation(gProgramIdCandle, "viewPosition");

    // Pass color, light, and camera data to the Shader program's corresponding uniforms
    glUniform3f(objectColorLoc4, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc4, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc4, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    glUniform3f(fillObjectColorLoc4, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
    glUniform3f(fillLightColorLoc4, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
    glUniform3f(fillLightPositionLoc4, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

    //const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc4, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc4 = glGetUniformLocation(gProgramIdCandle, "uvScale");
    glUniform2fv(UVScaleLoc4, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gTextureIdCandle);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMeshCandle.nVertices); // Draws the triangle


    // ------------------------------------- Draw the CUBE:
    // ----------------------------------------------------

    // Set the shader to be used
    glUseProgram(gProgramIdCube);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 modelCube = glm::translate(gCubePosition) * glm::scale(gCubeScale) * glm::rotate(225.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMeshCube.vao5);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc5 = glGetUniformLocation(gProgramIdCube, "model5");
    GLint viewLoc5 = glGetUniformLocation(gProgramIdCube, "view5");
    GLint projLoc5 = glGetUniformLocation(gProgramIdCube, "projection5");

    glUniformMatrix4fv(modelLoc5, 1, GL_FALSE, glm::value_ptr(modelCube));
    glUniformMatrix4fv(viewLoc5, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc5, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
    GLint objectColorLoc5 = glGetUniformLocation(gProgramIdCube, "objectColor");
    GLint lightColorLoc5 = glGetUniformLocation(gProgramIdCube, "lightColor");
    GLint lightPositionLoc5 = glGetUniformLocation(gProgramIdCube, "lightPos");

    GLint fillObjectColorLoc5 = glGetUniformLocation(gProgramIdCube, "fillObjectColor");
    GLint fillLightColorLoc5 = glGetUniformLocation(gProgramIdCube, "fillLightColor");
    GLint fillLightPositionLoc5 = glGetUniformLocation(gProgramIdCube, "fillLightPos");

    GLint viewPositionLoc5 = glGetUniformLocation(gProgramIdCube, "viewPosition");

    // Pass color, light, and camera data to the Shader program's corresponding uniforms
    glUniform3f(objectColorLoc5, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc5, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc5, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    glUniform3f(fillObjectColorLoc5, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
    glUniform3f(fillLightColorLoc5, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
    glUniform3f(fillLightPositionLoc5, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

    //const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc5, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc5 = glGetUniformLocation(gProgramIdCube, "uvScale");
    glUniform2fv(UVScaleLoc5, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, gTextureIdCube);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMeshCube.nVertices); // Draws the triangle


//---------------------------------------- Draw the LAMP: 
//-------------------------------------------------------

    glUseProgram(gLampProgramId);

    //Transform the smaller pyramid used as a visual que for the light source
    model = glm::translate(gLightPosition) * glm::scale(gLightScale);

    // Reference matrix uniforms from the Lamp Shader program
    modelLoc3 = glGetUniformLocation(gLampProgramId, "model");
    viewLoc3 = glGetUniformLocation(gLampProgramId, "view");
    projLoc3 = glGetUniformLocation(gLampProgramId, "projection");

    // Pass matrix data to the Lamp Shader program's matrix uniforms
    glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc3, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc3, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);


    //------------------------------ FILL LAMP: draw Fill lamp
    //--------------------------------------------------------
    glUseProgram(gFillLampProgramId);

    //Transform the smaller pyramid used as a visual que for the light source
    model = glm::translate(gFillLightPosition) * glm::scale(gFillLightScale);

    // Reference matrix uniforms from the Lamp Shader program
    GLuint modelLoc7 = glGetUniformLocation(gFillLampProgramId, "model7");
    GLuint viewLoc7 = glGetUniformLocation(gFillLampProgramId, "view7");
    GLuint projLoc7 = glGetUniformLocation(gFillLampProgramId, "projection7");

    // Pass matrix data to the Lamp Shader program's matrix uniforms
    glUniformMatrix4fv(modelLoc7, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc7, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc7, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, gMeshFillLamp.nVertices);

    
    // ------------------------------------- Draw the BALL:
    // ----------------------------------------------------

        // Set the shader to be used
    glUseProgram(gProgramIdBall);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 modelBall = glm::translate(gBallPosition) * glm::scale(gBallScale) * glm::rotate(225.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMeshBall.vao6);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc6 = glGetUniformLocation(gProgramIdBall, "model6");
    GLint viewLoc6 = glGetUniformLocation(gProgramIdBall, "view6");
    GLint projLoc6 = glGetUniformLocation(gProgramIdBall, "projection6");

    glUniformMatrix4fv(modelLoc6, 1, GL_FALSE, glm::value_ptr(modelCube));
    glUniformMatrix4fv(viewLoc6, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc6, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Shader program for the pyramid color, light color, light position, and camera position
    GLint objectColorLoc6 = glGetUniformLocation(gProgramIdBall, "objectColor");
    GLint lightColorLoc6 = glGetUniformLocation(gProgramIdBall, "lightColor");
    GLint lightPositionLoc6 = glGetUniformLocation(gProgramIdBall, "lightPos");

    GLint fillObjectColorLoc6 = glGetUniformLocation(gProgramIdBall, "fillObjectColor");
    GLint fillLightColorLoc6 = glGetUniformLocation(gProgramIdBall, "fillLightColor");
    GLint fillLightPositionLoc6 = glGetUniformLocation(gProgramIdBall, "fillLightPos");

    GLint viewPositionLoc6 = glGetUniformLocation(gProgramIdBall, "viewPosition");

    // Pass color, light, and camera data to the Shader program's corresponding uniforms
    glUniform3f(objectColorLoc6, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc6, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc6, gLightPosition.x, gLightPosition.y, gLightPosition.z);

    glUniform3f(fillObjectColorLoc6, gFillObjectColor.r, gFillObjectColor.g, gFillObjectColor.b);
    glUniform3f(fillLightColorLoc6, gFillLightColor.r, gFillLightColor.g, gFillLightColor.b);
    glUniform3f(fillLightPositionLoc6, gFillLightPosition.x, gFillLightPosition.y, gFillLightPosition.z);

    //const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc6, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc6 = glGetUniformLocation(gProgramIdBall, "uvScale");
    glUniform2fv(UVScaleLoc6, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, gTextureIdBall);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMeshBall.nVertices); // Draws the triangle

    //------------------------------------------------------------------------------

    // Deactivate the Vertex Array Object
    glBindVertexArray(0);
    glUseProgram(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.


}


// Implements the Ball UCreateMesh function
void UCreateBallMesh(GLMesh& mesh)
{
    

    int longitude = 30;
    int latitude = 30;
    double radius = 2;
    float Lats = 1 / (float)(longitude - 1);
    float Longs = 1 / (float)(latitude - 1);

    int r, s;
    long M_PI = 3.141592653;
    long M_PI_2 = 1.5707963267948;
    
    vector<GLfloat> verts;

    for (r = 0; r < longitude; r++)
    {
        for (s = 0; s < latitude; s++)
        {
            float const x = cos(2 * M_PI * s * Longs) * sin(M_PI * r * Lats);
            float const y = sin(-M_PI_2 + M_PI * r * Lats);
            float const z = sin(2 * M_PI * s * Longs) * sin(M_PI * r * Lats);

            verts.push_back(x * radius);
            verts.push_back(y * radius);
            verts.push_back(z* radius);

            verts.push_back(x);
            verts.push_back(y);
            verts.push_back(z);

            verts.push_back(s* Lats);
            verts.push_back(r* Longs);

            

            
        }
    }


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

  
    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerNormal +  floatsPerUV + floatsPerVertex));

    glGenVertexArrays(1, &mesh.vao6); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao6);

    // Create bufferfor the vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
   

}

// Implements the Lamp UCreateMesh function
void UCreateLampMesh(GLMesh& mesh)
{
    // Position, normals, and color data
    GLfloat verts[] = {     // Vertices to draw outer casing of eye-glass case
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create bufferfor the vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);

}

// Implements the Lamp UCreateMesh function
void UCreateFillLampMesh(GLMesh& mesh)
{
    // Position, normals, and color data
    GLfloat verts[] = {     // Vertices to draw outer casing of eye-glass case
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao7); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao7);

    // Create bufferfor the vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);

}

// Implements the Outer Case UCreateMesh function
void UCreateOuterMesh(GLMesh& mesh)
{
    // Position, normals, and color data
    GLfloat verts[] = {     // Vertices to draw outer casing of eye-glass case
        // Vertex Positions    // Normal             // Textures
        // back of outer case rectangle
        -2.0f,  0.6f,  0.0f,   0.0f,  0.0f, -1.0f,    0.0f, 1.0f,      // Top front triangle peak
         2.0f,  0.6f,  0.0f,   0.0f,  0.0f, -1.0f,    1.0f, 1.0f,      // Top back triangle peak
        -2.0f, -0.4f, -0.6f,   0.0f,  0.0f, -1.0f,    0.0f, 0.0f,      // front left bottom corner
         2.0f,  0.6f,  0.0f,   0.0f,  0.0f, -1.0f,    0.0f, 1.0f,      // Top front triangle peak
        -2.0f, -0.4f, -0.6f,   0.0f,  0.0f, -1.0f,    1.0f, 1.0f,      // Top back triangle peak
         2.0f, -0.4f, -0.6f,   0.0f,  0.0f, -1.0f,    1.0f, 0.0f,      // Back left bottom corner

        // front of outer case rectangle
         2.0f,  0.6f,  0.0f,   0.0f,  0.0f,  1.0f,    1.0f, 1.0f,      // Top back triangle peak
        -2.0f, -0.4f,  0.6f,   0.0f,  0.0f,  1.0f,    1.0f, 0.0f,      // Front right bottom corner
         2.0f, -0.4f,  0.6f,   0.0f,  0.0f,  1.0f,    0.0f, 1.0f,      // Back right bottom corner
        -2.0f,  0.6f,  0.0f,   0.0f,  0.0f,  1.0f,    0.0f, 1.0f,      // Top front triangle peak
         2.0f,  0.6f,  0.0f,   0.0f,  0.0f,  1.0f,    1.0f, 1.0f,      // Top back triangle peak
        -2.0f, -0.4f,  0.6f,   0.0f,  0.0f,  1.0f,    1.0f, 0.0f,      // Front right bottom corner

        // bottom of outer case rectangle
        -2.0f, -0.4f, -0.6f,   0.0f, -1.0f,  0.0f,    0.0f, 0.0f,      // front left bottom corner
         2.0f, -0.4f, -0.6f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,      // front right bottom corner
        -2.0f, -0.4f,  0.6f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,      // back left bottom corner	
        -2.0f, -0.4f,  0.6f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,      // Back left bottom corner
         2.0f, -0.4f,  0.6f,   0.0f, -1.0f,  0.0f,    1.0f, 1.0f,      // back right bottom corner
         2.0f, -0.4f, -0.6f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,      // front right bottom corner

         
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));


    glGenVertexArrays(1, &mesh.vao1); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao1);

    // Create buffer for the vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);   

}

// Implements the Inner Case UCreateMesh function
void UCreateInnerMesh(GLMesh& mesh)
{
    // Position, normals, and texture data
    GLfloat verts[] = {     // Vertices to draw outer casing of eye-glass case
        // Vertex Positions    // Normals           // Textures
        // indentation for front end triangle 
       -1.9f,  0.6f,  0.0f,   -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  // Top front triangle peak
       -1.9f, -0.4f, -0.5f,   -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  // front left bottom corner
       -1.9f, -0.4f,  0.5f,   -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  // Front right bottom corner

        // indentation for back end triangle
        1.9f,  0.6f,  0.0f,    1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  // Top back triangle peak
        1.9f, -0.4f, -0.5f,    1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  // Back left bottom corner
        1.9f, -0.4f,  0.5f,    1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  // Back right bottom corner

        // Vertex Positions    // Normal             // Textures
        // back of outer case rectangle
        -1.9f,  0.6f,  0.0f,   0.0f,  0.0f, -1.0f,    0.0f, 1.0f,      // Top front triangle peak
         1.9f,  0.6f,  0.0f,   0.0f,  0.0f, -1.0f,    1.0f, 1.0f,      // Top back triangle peak
        -1.9f, -0.4f, -0.5f,   0.0f,  0.0f, -1.0f,    0.0f, 0.0f,      // front left bottom corner
         1.9f,  0.6f,  0.0f,   0.0f,  0.0f,  1.0f,    0.0f, 1.0f,      // Top front triangle peak
        -1.9f, -0.4f, -0.5f,   0.0f,  0.0f,  1.0f,    1.0f, 1.0f,      // Top back triangle peak
         1.9f, -0.4f, -0.5f,   0.0f,  0.0f,  1.0f,    1.0f, 0.0f,      // Back left bottom corner

        // front of outer case rectangle
         1.9f,  0.6f,  0.0f,  -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,      // Top back triangle peak
        -1.9f, -0.4f,  0.5f,  -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,      // Front right bottom corner
         1.9f, -0.4f,  0.5f,  -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,      // Back right bottom corner
        -1.9f,  0.6f,  0.0f,   1.0f,  0.0f,  0.0f,    0.0f, 1.0f,      // Top front triangle peak
         1.9f,  0.6f,  0.0f,   1.0f,  0.0f,  0.0f,    1.0f, 1.0f,      // Top back triangle peak
        -1.9f, -0.4f,  0.5f,   1.0f,  0.0f,  0.0f,    1.0f, 0.0f,      // Front right bottom corner

        // bottom of outer case rectangle
        -1.9f, -0.4f, -0.5f,   0.0f, -1.0f,  0.0f,    0.0f, 0.0f,      // front left bottom corner
         1.9f, -0.4f, -0.5f,   0.0f, -1.0f,  0.0f,    1.0f, 0.0f,      // front right bottom corner
        -1.9f, -0.4f,  0.5f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,      // back left bottom corner	
        -1.9f, -0.4f,  0.5f,   0.0f,  1.0f,  0.0f,    0.0f, 1.0f,      // Back left bottom corner
         1.9f, -0.4f,  0.5f,   0.0f,  1.0f,  0.0f,    1.0f, 1.0f,      // back right bottom corner
         1.9f, -0.4f, -0.5f,   0.0f,  1.0f,  0.0f,    1.0f, 0.0f      // front right bottom corner

    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao2); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao2);

    // Create buffer for the vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU


    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);

}

// Implements the Plane UCreateMesh function
void UCreatePlaneMesh(GLMesh& mesh)
{
    // Position, normals, and texture data
    GLfloat verts[] = {     // Vertices to draw outer casing of eye-glass case
        // Vertex Positions    // Normals           // Textures  
        -5.0f, -0.5f, -5.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -5.0f, -0.5f,  5.0f,   0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         5.0f, -0.5f,  5.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         5.0f, -0.5f,  5.0f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         5.0f, -0.5f, -5.0f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -5.0f, -0.5f, -5.0f,   0.0f,  1.0f,  0.0f,  0.0f, 1.0f

    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV)); 

    glGenVertexArrays(1, &mesh.vao3); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao3);

    // Create buffers for the vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);

}

// Implements the Candle UCreateMesh function
void UCreateCandleMesh(GLMesh& mesh)
{
    // Position, normals, and texture data
    GLfloat verts[] = {     // Vertices to draw candle
        // positions          // normals           // texture coords
        -2.5f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         2.5f, -0.1f, -0.2f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         2.5f,  0.2f, -0.2f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         2.5f,  0.2f, -0.2f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -2.5f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -2.5f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -2.5f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         2.5f, -0.1f,  0.2f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         2.5f,  0.2f,  0.2f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         2.5f,  0.2f,  0.2f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -2.5f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -2.5f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -2.5f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -2.5f,  0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -2.5f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -2.5f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -2.5f, -0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -2.5f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         2.5f,  0.2f,  0.2f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         2.5f,  0.2f, -0.2f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         2.5f, -0.1f, -0.2f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         2.5f, -0.1f, -0.2f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         2.5f, -0.1f,  0.2f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         2.5f,  0.2f,  0.2f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -2.5f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         2.5f, -0.1f, -0.2f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         2.5f, -0.1f,  0.2f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         2.5f, -0.1f,  0.2f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -2.5f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -2.5f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -2.5f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         2.5f,  0.2f, -0.2f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         2.5f,  0.2f,  0.2f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         2.5f,  0.2f,  0.2f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -2.5f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -2.5f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

        -2.5f,  0.1f, -0.1f,  0.0f,  0.0f,  -1.0f,  1.0f,  0.0f,
        -2.5f, -0.1f, -0.1f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f,
        -3.0f,  0.0f,  0.0f,  0.0f,  0.0f,  -1.0f,  0.5f,  1.0f,
        -2.5f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -2.5f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -3.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.5f,  1.0f,

        -2.5f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -2.5f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        -3.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  1.0f,
        -2.5f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -2.5f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -3.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,


    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao4); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao4);

    // Create buffers for the vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);

}

// Implements the Cube UCreateMesh function
void UCreateCubeMesh(GLMesh& mesh)
{
    // Position, normals, and texture data
    GLfloat verts[] = {     // Vertices to draw cube
        // positions          // normals           // texture coords
        -0.5f, -0.3f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.3f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.3f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.3f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.3f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.3f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.3f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.3f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.3f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.3f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.3f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.3f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.3f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.3f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.3f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.3f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.3f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.3f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.3f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.3f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.3f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.3f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.3f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.3f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.3f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.3f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.3f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.3f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.3f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.3f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.3f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.3f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.3f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.3f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.3f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.3f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao5); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao5);

    // Create buffers for the vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);

}


// Destroy Mesh
void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteVertexArrays(1, &mesh.vao1);
    glDeleteVertexArrays(1, &mesh.vao2);
    glDeleteVertexArrays(1, &mesh.vao3);
    glDeleteVertexArrays(1, &mesh.vao4);
    glDeleteVertexArrays(1, &mesh.vao5);
    glDeleteVertexArrays(1, &mesh.vao6);
    glDeleteVertexArrays(1, &mesh.vao7);
    glDeleteBuffers(1, &mesh.vbo);
}

/*Generate and load the texture*/
bool UCreateOuterTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}

/*Generate and load the texture*/
bool UCreateInnerTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}

/*Generate and load the texture*/
bool UCreatePlaneTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}

/*Generate and load the texture*/
bool UCreateCandleTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}

/*Generate and load the texture*/
bool UCreateCubeTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}

/*Generate and load the texture*/
bool UCreateBallTexture(const char* filename, GLuint& textureId)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);
    if (image)
    {
        flipImageVertically(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}


// Destroy texture
void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}

// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrieve the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}

// Destroy shader
void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}
#pragma once 
#include <GL/glew.h>  
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream> 
#include <string> 
#include <fstream> 
#include <sstream> 
#include <vector> 
#include <algorithm>


/// @brief Reads shader source code from a file with path validation
/// @param filePath Path to the shader file
/// @return String containing the shader source code
std::string readShaderFile(const char* filePath);

/// @brief Callback function for window resize events. Adjusts the OpenGL viewport when the window framebuffer size changes.
/// @param window The GLFW window pointer that received the event
/// @param width The new width of the framebuffer in pixels
/// @param height The new height of the framebuffer in pixels
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/// @brief Initializes GLFW, GLEW, and creates an OpenGL window
/// @param width The широта of the window in pixels
/// @param height The узота of the window in pixels
/// @return Pointer to the created GLFWwindow on success, else nullptr
GLFWwindow* InitAll(int width = 200, int height = 200);

/// @brief Compiles a shader from source code
/// @param shaderType Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.)
/// @param source Null-terminated string containing shader source code
/// @return GLuint ID of the compiled shader
GLuint compileShader(GLenum shaderType, const char* source);

/// @brief Creates a complete shader program from vertex and fragment shader files
/// @param vertexPath File path to the vertex shader source
/// @param fragmentPath File path to the fragment shader source
/// @return GLuint ID of the linked shader programs
GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);

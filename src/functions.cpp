#include <functions.h>

std::string readShaderFile(const char* filePath) {
    std::string shaderCode;
    std::ifstream shaderFile(filePath);
    
    if(!shaderFile.is_open()){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << filePath << std::endl;
        return "";
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    
    shaderCode = shaderStream.str();
    shaderFile.close();
    return shaderCode;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* InitAll(int width, int height){
    if (!glfwInit())
    {
        std::cout << "ERROR: could not start GLFW3\n" << std::endl;
        exit(-1); 
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = nullptr; 

    window = glfwCreateWindow(width, height, "Hell Yeah", NULL, NULL);
 
    glfwMakeContextCurrent(window); 
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE; 

    if (glewInit() != GLEW_OK) 
    { 
        std::cout << "ERROR: could not start GLEW\n"; 
        return nullptr; 
    } 

    return window;
}


GLuint compileShader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint result = GL_FALSE; 
    int infoLogLength;
    glGetShaderiv (shader, GL_COMPILE_STATUS, &result); 
    glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength); 
    if (infoLogLength > 0) 
    { 
        char* errorMessage = new char[infoLogLength + 1]; 
        glGetShaderInfoLog(shader, infoLogLength, NULL, errorMessage); 
        std::cout << errorMessage; 
        delete errorMessage; 
    } 
    
    return shader;
}

GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath){
    std::string vertexCode = readShaderFile(vertexPath);
    std::string fragmentCode = readShaderFile(fragmentPath);
    
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);    

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

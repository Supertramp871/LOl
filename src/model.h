#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <functions.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

class Model {
    public:
        enum PrimitiveType {
            POINTS,
            LINES,
            LINE_STRIP,
            LINE_LOOP,
            TRIANGLES,
            TRIANGLE_STRIP,
            TRIANGLE_FAN
        };    
        Model();
        ~Model();

        /// @brief 
        /// @param vertexPath 
        /// @param fragmentPath 
        void initialize(const char* vertexPath, const char* fragmentPath); // set shaderProgram filed?
        
        /// @brief 
        void render();

        /// @brief 
        /// @param window 
        /// @param key 
        /// @param scancode 
        /// @param action 
        /// @param mods 
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void Task1(int n, float radius = 0.8);
        void Task2(int n, float radius);
        void Task3();
        void Task4();
        void Task5();
        void Task6(int n, float radius= 0.8);
        void Task7();
        void Task8();

        /// @brief 
        /// @param task 
        void setCurrentTask(int task);

        /// @brief 
        void updateRenderSettings();
    
    private:
        PrimitiveType primitiveType;
        GLuint VAO, VBO;
        GLuint shaderProgram;

        std::vector<float> vertices;
        std::vector<float> colors;
        int numVertices;

        /// @brief 
        void setupBuffers();

        /// @brief 
        void clearBuffers();

        /// @brief 
        /// @return 
        glm::vec3 getRandomColor();

        float pointSize;
        float lineWidth;
        int currentTask;
        bool smoothPoints;
        int smoothMode;
        static int renderMode; 
        std::vector<int> fanOffsets;
        bool flatMode;
        static int polygonMode;
};


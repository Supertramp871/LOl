#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functions.h>
#include <model.h>


int main() {
    GLFWwindow* window = InitAll(1920, 1080); //925 991

    if(window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    Model model;
    
    model.initialize("../shader/vertex_shader.glsl", "../shader/fragment_shader.glsl");
    model.setCurrentTask(5);

    auto key_callback_wrapper = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        Model* model = static_cast<Model*>(glfwGetWindowUserPointer(w));
        if (model) model->key_callback(w, key, scancode, action, mods);
    };

    glfwSetWindowUserPointer(window, &model);
    glfwSetKeyCallback(window, key_callback_wrapper);

    while(!glfwWindowShouldClose(window))
    {       
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        
        model.render();

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();

    return 0;
}


#include "model.h" 
#include "func.h" 
 
int WinWidth; 
int WinHeight; 
 
int main() { 
 
    GLFWwindow* window = InitAll(1024, 768, false); 
    if (window == nullptr) 
    { 
        EndAll(); 
        return -1; 
    } 
 
    //Создание модели 
    Model main(window); 
     
    //Массивы координат и цветов 
    glm::vec3 points[] = { 
        {0.0f, 0.5f, 0.0f}, 
        {0.5f, -0.5f, 0.0f}, 
        {-0.5f, -0.5f, 0.0f} 
    }; 
    glm::vec3 colors[] = { 
        {1.0f, 0.0f, 0.0f}, 
        {0.0f, 1.0f, 0.0f}, 
        {0.0f, 0.0f, 1.0f} 
    }; 
      //Массив индексов 
      GLuint indices[] = { 0, 1, 2}; 
 
      main.load_coords(points, 3); 
      main.load_colors(colors, 3); 
      main.load_indices(indices, 3); 
   
      main.load_shaders("vs.glsl", "fs.glsl"); 
   
      //Цвет фона – белый 
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
   
      float rotation = 0; 
   
      while (!glfwWindowShouldClose(window)) { 
          glViewport(0, 0, WinWidth, WinHeight); 
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
          main.render(GL_TRIANGLES); 
          glfwPollEvents(); 
          glfwSwapBuffers(window); 
          if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) 
          { 
              glfwSetWindowShouldClose(window, 1); 
          } 
      } 
   return 0; 
  } 
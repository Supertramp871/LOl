#include <model.h>

int Model::renderMode = 0;
int Model::polygonMode = 0;

Model::Model() {
    VAO = 0; VBO = 0;
    shaderProgram = 0;
    pointSize = 24.0f;
    lineWidth = 8.0f;
    numVertices = 0;
    currentTask = 1;
    smoothPoints = true;
    smoothMode = 1;
    flatMode = true;
}

Model::~Model() {
    clearBuffers();
}

void Model::clearBuffers() {
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
}

void Model::initialize(const char* vertexPath, const char* fragmentPath) {
    shaderProgram = createShaderProgram(vertexPath, fragmentPath);
}

void Model::setupBuffers() {
    clearBuffers();

    std::vector<float> combinedData;
    for (size_t i = 0; i < vertices.size() / 3; ++i) {
        combinedData.push_back(vertices[i * 3]);
        combinedData.push_back(vertices[i * 3 + 1]);
        combinedData.push_back(vertices[i * 3 + 2]);
        
        if (i < colors.size() / 3) {
            combinedData.push_back(colors[i * 3]);
            combinedData.push_back(colors[i * 3 + 1]);
            combinedData.push_back(colors[i * 3 + 2]);
        } else {
            combinedData.push_back(1.0f); // R
            combinedData.push_back(1.0f); // G  
            combinedData.push_back(1.0f); // B
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, combinedData.size() * sizeof(float), combinedData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Model::render() {
    glUseProgram(shaderProgram);
    updateRenderSettings();

    glBindVertexArray(VAO);
    
    switch (primitiveType) {
        case POINTS:
            glDrawArrays(GL_POINTS, 0, numVertices);
            break;
        case LINES:
            glDrawArrays(GL_LINES, 0, numVertices);
            break;
        case LINE_STRIP:
            glDrawArrays(GL_LINE_STRIP, 0, numVertices);
            break;
        case LINE_LOOP:
            glDrawArrays(GL_LINE_LOOP, 0, numVertices);
            break;
        case TRIANGLES:
            glDrawArrays(GL_TRIANGLES, 0, numVertices);
            break;
        case TRIANGLE_STRIP:
            glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
            break;
        case TRIANGLE_FAN:
            if (fanOffsets.size() > 1) {
                for (size_t i = 0; i < fanOffsets.size(); ++i) {
                    int start = fanOffsets[i];
                    int count = (i == fanOffsets.size() - 1) ? 
                               numVertices - start : 
                               fanOffsets[i + 1] - start;
                    
                    if (count > 0) {
                        glDrawArrays(GL_TRIANGLE_FAN, start, count);
                    }
                }
            } else {
                glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
            }
            break;
    }
    
    glBindVertexArray(0);
}

void Model::updateRenderSettings() {
    glUseProgram(shaderProgram);
    
    int smoothModeLoc = glGetUniformLocation(shaderProgram, "u_smoothMode");
    glUniform1i(smoothModeLoc, smoothMode);

    int flatModeLoc = glGetUniformLocation(shaderProgram, "u_flatMode");
    glUniform1i(flatModeLoc, flatMode);

    if (primitiveType == POINTS) {
        glPointSize(pointSize);
    } else if (primitiveType == LINES || primitiveType == LINE_STRIP || primitiveType == LINE_LOOP) {
        glLineWidth(lineWidth);
    }
    
}

glm::vec3 Model::getRandomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    
    return glm::vec3(dis(gen), dis(gen), dis(gen));
}

void Model::setCurrentTask(int task) {
    currentTask = task;
    
    switch (currentTask) {
        case 1:
            Task1(7, 0.5f);
            break;
        case 2:
            Task2(7, 0.5f);
            break;
        case 3:
            Task3();
            break;
        case 4:
            Task4();
            break;
        case 5:
            Task5();
            break;
        case 6:
            Task6(7, 0.5f);
            break;
        case 7:
            Task7();
            break;
        case 8:
            Task8();
            break;
    }
}

void Model::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_1: 
                if (action == GLFW_PRESS) setCurrentTask(1); 
                break;
            case GLFW_KEY_2: 
                if (action == GLFW_PRESS) setCurrentTask(2); 
                break;
            case GLFW_KEY_3: 
                if (action == GLFW_PRESS) setCurrentTask(3); 
                break;
            case GLFW_KEY_4: 
                if (action == GLFW_PRESS) setCurrentTask(4); 
                break;
            case GLFW_KEY_5: 
                if (action == GLFW_PRESS) setCurrentTask(5); 
                break;
            case GLFW_KEY_6: 
                if (action == GLFW_PRESS) setCurrentTask(6); 
                break;
            case GLFW_KEY_7: 
                if (action == GLFW_PRESS) setCurrentTask(7); 
                break;
            case GLFW_KEY_8: 
                if (action == GLFW_PRESS) setCurrentTask(8); 
                break;
            
            case GLFW_KEY_K:
                if (currentTask == 1) {
                    pointSize += 1.0f;
                    std::cout << "Point size: " << pointSize << std::endl;
                    updateRenderSettings();
                } else if (currentTask == 2 || currentTask == 3 || currentTask == 4){
                    lineWidth += 0.5f;
                    std::cout << "Line width: " << lineWidth << std::endl;
                    updateRenderSettings();
                }
                break;
                
            case GLFW_KEY_J:
                if (currentTask == 1) {
                    pointSize = std::max(1.0f, pointSize - 1.0f);
                    std::cout << "Point size: " << pointSize << std::endl;
                    updateRenderSettings();
                } else if (currentTask == 2 || currentTask == 3 || currentTask == 4){
                    lineWidth = std::max(1.0f, lineWidth - 0.5f);
                    std::cout << "Line width: " << lineWidth << std::endl;
                    updateRenderSettings();
                }
                break;
                
            case GLFW_KEY_S:
                if (action == GLFW_PRESS && currentTask == 1) {
                    smoothMode = (smoothMode + 1) % 2; // WHAT?
                    const char* modes[] = {"OFF", "ON"};
                    std::cout << "Smoot: " << modes[smoothMode] << std::endl;
                    updateRenderSettings();
                }
                break;

            case GLFW_KEY_F:
            if (action == GLFW_PRESS && currentTask != 1) {
                flatMode = !flatMode;
                const char* modes[] = {"SMOOTH", "FLAT"};
                std::cout << "Flat mode: " << modes[flatMode ? 1 : 0] << std::endl;
                updateRenderSettings();
            }
            break;
        }
    }
}

void Model::Task1(int n, float radius) {
    fanOffsets.clear(); 
    vertices.clear();
    colors.clear();
    
    for (int i = 0; i < n; ++i) {
        float angle = 2.0f * M_PI * i / n;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        
        colors.push_back(1.0f);
        colors.push_back(1.0f);
        colors.push_back(1.0f);
    }
    
    numVertices = n;
    primitiveType = POINTS;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setupBuffers();
}

void Model::Task2(int n, float radius) {
    fanOffsets.clear(); 
    vertices.clear();
    colors.clear();
    
    for (int i = 0; i < n; ++i) {
        float angle1 = 2.0f * M_PI * i / n;
        float angle2 = 2.0f * M_PI * (i + 1) / n;
        
        vertices.push_back(radius * cos(angle1));
        vertices.push_back(radius * sin(angle1));
        vertices.push_back(0.0f);
        
        vertices.push_back(radius * cos(angle2));
        vertices.push_back(radius * sin(angle2));
        vertices.push_back(0.0f);
        
        for (int j = 0; j < 2; ++j) {
            glm::vec3 color = getRandomColor();
            colors.push_back(color.r);
            colors.push_back(color.g);
            colors.push_back(color.b);
        }
    }
    
    numVertices = n * 2;
    primitiveType = LINES;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setupBuffers();
}

void Model::Task3() {
    fanOffsets.clear(); 
    vertices.clear();
    colors.clear();
    
    std::vector<glm::vec2> points = {
        {-0.85f, 0.48f},   // 1
        {-0.623f, -0.26f}, // 2
        {-0.349f, 0.12f},  // 3
        {0.075f, 0.12f},   // 4 
        {-0.05f, 0.6f},    // 5 
        {0.740f, 0.6f},    // 6
        {0.130f, -0.454f}  // 7
    };
    
    for (const auto& point : points) {
        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(0.0f);
        
        glm::vec3 color = getRandomColor();
        colors.push_back(color.r);
        colors.push_back(color.g);
        colors.push_back(color.b);
    }
    
    numVertices = 7;
    primitiveType = LINE_STRIP;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setupBuffers();
}

void Model::Task4() {
    fanOffsets.clear(); 
    vertices.clear();
    colors.clear();
    
    std::vector<glm::vec2> points = {
        {-0.7f, 0.8f},     //1 
        {0.7f, 0.8f},      //2
        {0.2f, 0.5f},      //3
        {0.2f, 0.0f},      //4
        {0.6f, 0.0f},      //5
        {0.6f, -0.4f},     //6
        {-0.6f, -0.4f},    //7
        {-0.1358f,0.2985f},//8
    };
    
    for (const auto& point : points) {
        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(0.0f);
        
        glm::vec3 color = getRandomColor();
        colors.push_back(color.r);
        colors.push_back(color.g);
        colors.push_back(color.b);
    }
    
    numVertices = points.size();
    primitiveType = LINE_LOOP;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setupBuffers();
}

void Model::Task5() {
    fanOffsets.clear(); 
    vertices.clear();
    colors.clear();

    static int renderMode = 0; // 0 - TRIANGLES, 1 - TRIANGLE_STRIP, 2 - TRIANGLE_FAN
    
    std::vector<glm::vec2> triangles = {
        {-0.7f, 0.8f}, {0.7f, 0.8f}, {0.2f, 0.5f},       // (1-2-3)
        {-0.7f, 0.8f}, {0.2f, 0.5f}, {-0.1358f,0.2985f}, // (1-3-8)
        {0.2f, 0.5f}, {0.2f, 0.0f}, {-0.1358f,0.2985f},  // (3-4-8)       
        {0.2f, 0.0f}, {0.6f, 0.0f}, {0.6f, -0.4f},       // (4-5-6)      
        {0.2f, 0.0f}, {0.6f, -0.4f}, {-0.6f, -0.4f},     // (4-6-7)
        {0.2f, 0.0f}, {-0.6f, -0.4f}, {-0.1358f,0.2985f} // (4-7-8)
    };

    switch (renderMode) {
        case 0: // GL_TRIANGLES
        {
            int vertexCount = 0;
            for (int i = 0; i < triangles.size(); i += 3) {
                glm::vec2 v1 = triangles[i];
                glm::vec2 v2 = triangles[i + 1];
                glm::vec2 v3 = triangles[i + 2];
                
                vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(0.0f);
                vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(0.0f);
                vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(0.0f);
                
                glm::vec3 color = getRandomColor();
                for (int j = 0; j < 3; ++j) {
                    colors.push_back(color.r);
                    colors.push_back(color.g);
                    colors.push_back(color.b);

                vertexCount += 1;
                }
            }

            primitiveType = TRIANGLES;
            std::cout << "GL_TRIANGLES(" 
                      << vertexCount << " vertex)" << std::endl;
            break;
        }
        
        case 1: // GL_TRIANGLE_STRIP
        {
            
            std::vector<glm::vec2> triangleStrip = {
                {0.7f, 0.8f},        // 2
                {-0.7f, 0.8f},       // 1
                {0.2f, 0.5f},        // 3
                {-0.1358f,0.2985f},  // 4
                {0.2f, 0.0f},        // 5
                {-0.6f, -0.4f},      // 6
                {0.6f, 0.0f},        // 7
                {0.6f, -0.4f},       // 8
            };

            for (int i = 0; i < triangleStrip.size(); ++i) {
                glm::vec2 vertex = triangleStrip[i];
                vertices.push_back(vertex.x); 
                vertices.push_back(vertex.y); 
                vertices.push_back(0.0f);
                
                glm::vec3 color = getRandomColor();
                colors.push_back(color.r);
                colors.push_back(color.g);
                colors.push_back(color.b);
            }
        
            primitiveType = TRIANGLE_STRIP;
            std::cout << "GL_TRIANGLE_STRIP (" 
                    << triangleStrip.size() << " vertex)" << std::endl;
            break;
        }
        
        case 2: // GL_TRIANGLE_FAN
        {
            fanOffsets.clear();
            std::vector<glm::vec2> triangleFan1 = {
                {0.2f, 0.0f}, {0.6f, 0.0f}, {0.6f, -0.4f}, 
                {-0.6f, -0.4f}, {-0.1358f, 0.2985f}
            };
         
            std::vector<glm::vec2> triangleFan2 = {
                {-0.7f, 0.8f}, {0.7f, 0.8f}, {0.2f, 0.5f}, {0.2f, 0.0f}
            };
        
            fanOffsets.push_back(0); 
            fanOffsets.push_back(triangleFan1.size()); 

            for (const auto& vertex : triangleFan1) {
                vertices.push_back(vertex.x); 
                vertices.push_back(vertex.y); 
                vertices.push_back(0.0f);
                
                glm::vec3 color = getRandomColor();
                colors.push_back(color.r);
                colors.push_back(color.g);
                colors.push_back(color.b);
            }
        
            for (const auto& vertex : triangleFan2) {
                vertices.push_back(vertex.x); 
                vertices.push_back(vertex.y); 
                vertices.push_back(0.0f);
                
                glm::vec3 color = getRandomColor();
                colors.push_back(color.r);
                colors.push_back(color.g);
                colors.push_back(color.b);
            }
        
            primitiveType = TRIANGLE_FAN;
            numVertices = vertices.size() / 3;
            
            std::cout << "GL_TRIANGLE_FAN (" << triangleFan1.size() + triangleFan2.size() << " vertex)" << std::endl;
            break;
        }
    }
    
    numVertices = vertices.size() / 3;

    renderMode = (renderMode + 1) % 3;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setupBuffers();
}

void Model::Task6(int n, float radius) {
    fanOffsets.clear(); 
    vertices.clear();
    colors.clear();
    
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    
    for (int i = 0; i <= n; ++i) {
        float angle = 2.0f * M_PI * i / n;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
        
        glm::vec3 color = getRandomColor();
        colors.push_back(color.r);
        colors.push_back(color.g);
        colors.push_back(color.b);
    }
    
    numVertices = n + 2;
    primitiveType = TRIANGLE_FAN;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setupBuffers();
}

void Model::Task7() { 
    fanOffsets.clear(); 
    vertices.clear();
    colors.clear();

    std::vector<glm::vec2> vertices7_points = {
        {-0.5f, 0.8f},     // 0 - vertex 1
        {0.7f, 0.7f},      // 1 - vertex 2  
        {-0.1f, 0.5f},     // 2 - vertex 3
        {-0.4f, -0.1f},    // 3 - vertex 4
        {0.4f, -0.5f},     // 4 - vertex 5
        {0.1f, -0.2f},     // 5 - vertex 6
        {-0.1f, -0.6f},    // 6 - vertex 7
        {-0.8, 0.0},       // 7 - vertex 8
        {0.8f, 0.0f},      // 8 - vertex 9
    };
    
    std::vector<glm::ivec3> triangles7_indices = {
        {0, 1, 2},
        {0, 2, 7},
        {2, 3, 7},
        {3, 5, 6},
        {3, 6, 7},
        {1, 2, 8},
        {2, 8, 4},
    };

    for (const auto& tri : triangles7_indices) {
        vertices.push_back(vertices7_points[tri.x].x);
        vertices.push_back(vertices7_points[tri.x].y);
        vertices.push_back(0.0f);
            
        vertices.push_back(vertices7_points[tri.y].x);
        vertices.push_back(vertices7_points[tri.y].y);
        vertices.push_back(0.0f);
            
        vertices.push_back(vertices7_points[tri.z].x);
        vertices.push_back(vertices7_points[tri.z].y);
        vertices.push_back(0.0f);
            
        glm::vec3 color1 = getRandomColor();
        glm::vec3 color2 = getRandomColor(); 
        glm::vec3 color3 = getRandomColor();
        
        colors.push_back(color1.r); colors.push_back(color1.g); colors.push_back(color1.b);
        colors.push_back(color2.r); colors.push_back(color2.g); colors.push_back(color2.b);
        colors.push_back(color3.r); colors.push_back(color3.g); colors.push_back(color3.b);
    }

    primitiveType = TRIANGLES;
    
    numVertices = vertices.size() / 3;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    setupBuffers();
}

void Model::Task8() {
    static int polygonMode = 0; // 0 - Points, 1 - Fill, 2 - Line
    
    fanOffsets.clear(); 
    vertices.clear();
    colors.clear();

    std::vector<glm::vec2> vertices7_points = {
        {-0.5f, 0.8f},     // 0 - vertex 1
        {0.7f, 0.7f},      // 1 - vertex 2  
        {-0.1f, 0.5f},     // 2 - vertex 3
        {-0.4f, -0.1f},    // 3 - vertex 4
        {0.4f, -0.5f},     // 4 - vertex 5
        {0.1f, -0.2f},     // 5 - vertex 6
        {-0.1f, -0.6f},    // 6 - vertex 7
        {-0.8f, 0.0f},     // 7 - vertex 8
        {0.8f, 0.0f},      // 8 - vertex 9
    };
    
    std::vector<glm::ivec3> triangles7_indices = {
        {0, 1, 2},
        {0, 2, 7},
        {2, 3, 7},
        {3, 5, 6},
        {3, 6, 7},
        {1, 2, 8},
        {2, 8, 4},
    };

    for (const auto& tri : triangles7_indices) {
        vertices.push_back(vertices7_points[tri.x].x);
        vertices.push_back(vertices7_points[tri.x].y);
        vertices.push_back(0.0f);
            
        vertices.push_back(vertices7_points[tri.y].x);
        vertices.push_back(vertices7_points[tri.y].y);
        vertices.push_back(0.0f);
            
        vertices.push_back(vertices7_points[tri.z].x);
        vertices.push_back(vertices7_points[tri.z].y);
        vertices.push_back(0.0f);
            
        glm::vec3 color1 = getRandomColor();
        glm::vec3 color2 = getRandomColor(); 
        glm::vec3 color3 = getRandomColor();
        
        colors.push_back(color1.r); colors.push_back(color1.g); colors.push_back(color1.b);
        colors.push_back(color2.r); colors.push_back(color2.g); colors.push_back(color2.b);
        colors.push_back(color3.r); colors.push_back(color3.g); colors.push_back(color3.b);
    }

    primitiveType = TRIANGLES;
    numVertices = vertices.size() / 3;
    setupBuffers();
    
    switch (polygonMode) {
        case 0: 
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glPointSize(pointSize);
            std::cout << "A::GL_POINT" << std::endl;
            break;
            
        case 1: 
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            std::cout << "B::GL_FILL" << std::endl;
            break;
            
        case 2: 
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(lineWidth);
            std::cout << "C::GL_LINE" << std::endl;
            break;
    }

    primitiveType = TRIANGLES;
    numVertices = vertices.size() / 3;
    setupBuffers();

    polygonMode = (polygonMode + 1) % 3;

}
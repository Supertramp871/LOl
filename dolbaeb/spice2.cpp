#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cstdlib>  // для rand()
#include <ctime>    // для srand
#include <math.h>
const unsigned int WINDOW_WIDTH = 777;
const unsigned int WINDOW_HEIGHT = 777;

// Шейдеры
const char *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
flat out vec3 vColor;
void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    vColor = aColor;
}
)";

const char *fragmentShaderSource = R"(
#version 400
flat in vec3 vColor;
out vec4 FragColor;
void main()
{
    FragColor = vec4(vColor, 1.0);
}
)";

// Режимы
enum class RenderMode {
    POINTS = 0,
    LINES = 1,
    LINE_STRIP = 2,
    LINE_LOOP = 3,
    TRIANGLES = 4,
    TRIANGLE_STRIP = 5,
    TRIANGLE_FAN = 6,
    PENTAGON_FAN = 7,
    F_POINT = 8,
    F_FILL_B_LINE = 9,
    FB_LINE = 10
};

RenderMode currentMode = RenderMode::POINTS;
float size = 5;

// Генерация вершин пятиугольника
std::vector<float> generatePolygonVertices(int N, float radius = 0.5f) {
    auto randomColor = []() { return static_cast<float>(std::rand()) / RAND_MAX; };
    std::vector<float> vertices;
    const float PI = 3.14159265359f;
    for (int i = 0; i < N; i++) {
        float angle = 2.0f * PI * i / N - PI / 2.0f + PI;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(randomColor());
        vertices.push_back(randomColor());
        vertices.push_back(randomColor());
    }
    return vertices;
}

// Индексы для линий
std::vector<GLuint> generatePolygonLineIndices(int N) {
    std::vector<GLuint> indices;
    for (int i = 0; i < N; i++) {
        indices.push_back(i);
        indices.push_back((i + 1) % N);
    }
    return indices;
}

// LINE_STRIP
std::vector<float> generateLineStripVertices() {
    std::vector<float> coords = {
        -0.8f, 0.3f,
        -0.2f, 0.6f,
        0.0f, 0.2f,
        -0.4f, -0.3f,
        0.4f, -0.3f,
        0.8f, 0.2f
    };

    std::vector<float> verts;
    auto randomColor = []() { return static_cast<float>(std::rand()) / RAND_MAX; };

    for (size_t i = 0; i < coords.size() / 2; ++i) {
        verts.push_back(coords[i * 2]);
        verts.push_back(coords[i * 2 + 1]);
        verts.push_back(randomColor());
        verts.push_back(randomColor());
        verts.push_back(randomColor());
    }

    return verts;
}

// LINE_LOOP
std::vector<float> generateLineLoopVertices() {
    std::vector<float> coords = {
        -0.6f, -0.1f,
        -0.1f, 0.7f,
        0.3f, 0.45f,
        0.1f, 0.1f,
        0.55f, -0.15f,
        0.3f, -0.6f
    };

    std::vector<float> verts;
    auto randomColor = []() { return static_cast<float>(std::rand()) / RAND_MAX; };

    for (size_t i = 0; i < coords.size() / 2; ++i) {
        verts.push_back(coords[i * 2]);
        verts.push_back(coords[i * 2 + 1]);
        verts.push_back(randomColor());
        verts.push_back(randomColor());
        verts.push_back(randomColor());
    }

    return verts;
}

// TRIANGLES
std::vector<float> generateTrianglesVertices() {
    std::srand(std::time(nullptr));
    std::vector<float> verts;

    auto randomColor = []() {
        return static_cast<float>(std::rand()) / RAND_MAX;
    };

    struct Triangle {
        float x[3];
        float y[3];
    };

    Triangle triangles[4] = {
        {{-0.1f, 0.3f, 0.1f}, {0.7f, 0.45f, 0.1f}},
        {{-0.1f, 0.1f, -0.6f}, {0.7f, 0.1f, -0.1f}},
        {{-0.6f, 0.1f, 0.3f}, {-0.1f, 0.1f, -0.6f}},
        {{0.1f, 0.55f, 0.3f}, {0.1f, -0.15f, -0.6f}}
    };

    for (int t = 0; t < 4; ++t) {
        for (int v = 0; v < 3; ++v) {
            verts.push_back(triangles[t].x[v]);
            verts.push_back(triangles[t].y[v]);
            verts.push_back(randomColor());
            verts.push_back(randomColor());
            verts.push_back(randomColor());
        }
    }
    return verts;
}

// TRIANGLES_STRIP
std::vector<float> generateTriangleStripVertices() {
    std::vector<float> coords = {
        -0.1f, 0.7f,
        0.3f, 0.45f,
        -0.6f, -0.1f,
        0.1f, 0.1f,
        0.3f, -0.6f,
        0.55f, -0.15f
    };

    std::vector<float> verts;
    auto randomColor = []() { return static_cast<float>(std::rand()) / RAND_MAX; };

    // TRIANGLE_STRIP требует упорядоченные вершины
    for (size_t i = 0; i < coords.size() / 2; ++i) {
        verts.push_back(coords[i * 2]);
        verts.push_back(coords[i * 2 + 1]);
        verts.push_back(randomColor());
        verts.push_back(randomColor());
        verts.push_back(randomColor());
    }

    return verts;
}

// TRIANGLES_FAN
std::vector<float> generateTriangleFanVertices() {
    std::vector<float> coords = {
        0.1f, 0.1f,
        0.55f, -0.15f,
        0.3f, -0.6f,
        -0.6f, -0.1f,
        -0.1f, 0.7f,
        0.3f, 0.45f
    };

    std::vector<float> verts;
    auto randomColor = []() { return static_cast<float>(std::rand()) / RAND_MAX; };

    for (size_t i = 0; i < coords.size() / 2; ++i) {
        verts.push_back(coords[i * 2]);
        verts.push_back(coords[i * 2 + 1]);
        verts.push_back(randomColor());
        verts.push_back(randomColor());
        verts.push_back(randomColor());
    }

    return verts;
}

// PENTAGON_TRIANGLE_FAN
std::vector<float> generatePentagonFanVertices() {
    std::srand(std::time(nullptr));
    std::vector<float> verts;
    std::vector<float> pent = generatePolygonVertices(5);
    return pent;
}

// CUSTOM 7 TRIANGLES
std::vector<float> generateCustomTriangles() {
    std::srand(std::time(nullptr));
    std::vector<float> verts;

    float coords[] = {
        -0.7f, 0.6f, 0.1f, 0.6f, -0.5f, 0.1f,
        -0.7f, -0.6f, -0.5f, 0.1f, -0.7f, 0.6f,
        -0.7f, -0.6f, -0.5f, 0.1f, -0.15f, -0.3f,
        -0.15f, -0.3f, -0.7f, -0.6f, 0.7f, -0.6f,
        0.7f, -0.6f, -0.15f, -0.3f, 0.2f, 0.07f,
        0.2f, 0.07f, 0.7f, -0.6f, 0.7f, 0.2f,
        0.2f, 0.07f, 0.7f, 0.2f, 0.3f, 0.45f
    };

    auto randomColor = []() { return static_cast<float>(std::rand()) / RAND_MAX; };

    // каждый треугольник отдельным цветом
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            verts.push_back(coords[i * 6 + j * 2 + 0]);
            verts.push_back(coords[i * 6 + j * 2 + 1]);
            verts.push_back(randomColor());
            verts.push_back(randomColor());
            verts.push_back(randomColor());
        }
    }
    return verts;
}

// Компиляция шейдера
GLuint compileShader(GLenum type, const char *src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader,GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << log << std::endl;
    }
    return shader;
}

// Программа
GLuint createShaderProgram() {
    GLuint v = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint f = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, v);
    glAttachShader(prog, f);
    glLinkProgram(prog);
    glDeleteShader(v);
    glDeleteShader(f);
    return prog;
}

// Ввод
void processInput(GLFWwindow *window) {
    static double lastPress = 0.0;
    double now = glfwGetTime();
    if (now - lastPress < 0.15) return;
    if (glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS) {
        currentMode = (RenderMode) (((int) currentMode + 1) % 11);
        lastPress = now;
    }
    if (glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS) {
        currentMode = (RenderMode) (((int) currentMode + 10) % 11);
        lastPress = now;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        size += 1.0f;
        std::cout << "size: " << size << std::endl;
        lastPress = now;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        size -= 1.0f;
        if (size < 1.0f) size = 1.0f;
        std::cout << "size: " << size << std::endl;
        lastPress = now;
    }
}

int main() {
    if (!glfwInit()) return -1;

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Это второй!", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return -1;
    }

    const GLubyte* version = glGetString(GL_VERSION);
    printf("OpenGL version supportted %s\n", version);

    // Pentagon
    std::vector<float> vertsPent = generatePolygonVertices(5);
    std::vector<GLuint> indsPent = generatePolygonLineIndices(5);
    // Line strip
    std::vector<float> vertsStrip = generateLineStripVertices();
    // Line loop
    std::vector<float> vertsLoop = generateLineLoopVertices();
    // Triangles
    std::vector<float> vertsTri = generateTrianglesVertices();

    // Pentagon VAO
    GLuint VAO1, VBO1, EBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, vertsPent.size() * sizeof(float), vertsPent.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indsPent.size() * sizeof(GLuint), indsPent.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Line strip VAO
    GLuint VAO2, VBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, vertsStrip.size() * sizeof(float), vertsStrip.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Line loop VAO
    GLuint VAO3, VBO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, vertsLoop.size() * sizeof(float), vertsLoop.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Triangles VAO
    GLuint VAO4, VBO4;
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);
    glBindVertexArray(VAO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, vertsTri.size() * sizeof(float), vertsTri.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3,GL_FLOAT,GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TRIANGLE_STRIP VAO
    std::vector<float> vertsStripTri = generateTriangleStripVertices();
    GLuint VAO5, VBO5;
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);
    glBindVertexArray(VAO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, vertsStripTri.size() * sizeof(float), vertsStripTri.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TRIANGLE_FAN VAO
    std::vector<float> vertsFan = generateTriangleFanVertices();
    GLuint VAO6, VBO6;
    glGenVertexArrays(1, &VAO6);
    glGenBuffers(1, &VBO6);
    glBindVertexArray(VAO6);
    glBindBuffer(GL_ARRAY_BUFFER, VBO6);
    glBufferData(GL_ARRAY_BUFFER, vertsFan.size() * sizeof(float), vertsFan.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // PENTAGON TRIANGLE_FAN VAO
    std::vector<float> vertsPentFan = generatePentagonFanVertices();
    GLuint VAO7, VBO7;
    glGenVertexArrays(1, &VAO7);
    glGenBuffers(1, &VBO7);
    glBindVertexArray(VAO7);
    glBindBuffer(GL_ARRAY_BUFFER, VBO7);
    glBufferData(GL_ARRAY_BUFFER, vertsPentFan.size() * sizeof(float), vertsPentFan.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // CUSTOM 7 TRIANGLES VAO
    std::vector<float> vertsCustom = generateCustomTriangles();
    GLuint VAO8, VBO8;
    glGenVertexArrays(1, &VAO8);
    glGenBuffers(1, &VBO8);
    glBindVertexArray(VAO8);
    glBindBuffer(GL_ARRAY_BUFFER, VBO8);
    glBufferData(GL_ARRAY_BUFFER, vertsCustom.size() * sizeof(float), vertsCustom.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint shader = createShaderProgram();
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);


        switch (currentMode) {
            case RenderMode::POINTS:
                glBindVertexArray(VAO1);
                glPointSize(size);
                glDrawArrays(GL_POINTS, 0, 5);
                break;
            case RenderMode::LINES:
                glBindVertexArray(VAO1);
                glLineWidth(size);
                glDrawElements(GL_LINES, indsPent.size(),GL_UNSIGNED_INT, 0);
                break;
            case RenderMode::LINE_STRIP:
                glBindVertexArray(VAO2);
                glLineWidth(size);
                glDrawArrays(GL_LINE_STRIP, 0, 6);
                break;
            case RenderMode::LINE_LOOP:
                glBindVertexArray(VAO3);
                glLineWidth(size);
                glDrawArrays(GL_LINE_LOOP, 0, 6);
                break;
            case RenderMode::TRIANGLES:
                glBindVertexArray(VAO4);
                glDrawArrays(GL_TRIANGLES, 0, vertsTri.size() / 5);
                break;
            case RenderMode::TRIANGLE_STRIP:
                glBindVertexArray(VAO5);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, vertsStripTri.size() / 5);
                break;
            case RenderMode::TRIANGLE_FAN:
                glBindVertexArray(VAO6);
                glDrawArrays(GL_TRIANGLE_FAN, 0, vertsFan.size() / 5);
                break;
            case RenderMode::PENTAGON_FAN:
                glBindVertexArray(VAO7);
                glDrawArrays(GL_TRIANGLE_FAN, 0, vertsPentFan.size() / 5);
                break;
            case RenderMode::F_POINT:
                glBindVertexArray(VAO8);
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                glPolygonMode(GL_FRONT, GL_POINT);
                glPointSize(size);
                glDrawArrays(GL_TRIANGLES, 0, vertsCustom.size() / 5);
                glDisable(GL_CULL_FACE);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            case RenderMode::F_FILL_B_LINE:
                glBindVertexArray(VAO8);
                glPolygonMode(GL_FRONT, GL_FILL);
                glPolygonMode(GL_BACK, GL_LINE);
                glLineWidth(size);
                glDrawArrays(GL_TRIANGLES, 0, vertsCustom.size() / 5);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            case RenderMode::FB_LINE:
                glBindVertexArray(VAO8);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glLineWidth(size);
                glDrawArrays(GL_TRIANGLES, 0, vertsCustom.size() / 5);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &EBO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO2);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO3);
    glDeleteVertexArrays(1, &VAO3);
    glDeleteBuffers(1, &VBO4);
    glDeleteVertexArrays(1, &VAO4);
    glDeleteBuffers(1, &VBO5);
    glDeleteVertexArrays(1, &VAO5);
    glDeleteBuffers(1, &VBO6);
    glDeleteVertexArrays(1, &VAO6);
    glDeleteBuffers(1, &VBO7);
    glDeleteVertexArrays(1, &VAO7);
    glDeleteBuffers(1, &VBO8);
    glDeleteVertexArrays(1, &VAO8);
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}
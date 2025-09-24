#include "func.h" 
#include "globals.h" 
string LoadShader(const char* filename) 
{ 
    string res; 
    ifstream file(filename, ios::in); 
    if (file.is_open()) 
    { 
        std::stringstream sstr; // Буфер для чтения 
        sstr << file.rdbuf(); // Считываем файл 
        res = sstr.str();   //Получаем строку из буфера 
        file.close();       //Закрываем файл 
    } 
    return res; 
} 
 
GLFWwindow* InitAll(int w, int h, bool Fullscreen) 
{ 
    //В глобальные переменные сохраняем размеры окна 
    WinWidth = w; 
    WinHeight = h; 
 
    //Создаём переменную для хранения ID окна 
    GLFWwindow* window = nullptr; 
 
    //Подключение GLFW 
    if (!glfwInit()) 
    { 
        cerr << "ERROR: could not start GLFW3\n"; 
        exit(-1); 
    } 
 
    // Это для MacOS 
    /* 
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
     
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
     
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); */ 
 
 
    // В зависимости от режима экрана создаётся окно с  
    //заголовком OpenGL 
    if (Fullscreen) 
    { 
        GLFWmonitor* mon = glfwGetPrimaryMonitor(); 
        const GLFWvidmode* vmode = glfwGetVideoMode(mon); 
        WinWidth = vmode->width; 
        WinHeight = vmode->height; 
        window = glfwCreateWindow(WinWidth, WinHeight, "OpenGL", mon, NULL); 
    } 
    else 
        window = glfwCreateWindow(WinWidth, WinHeight, "OpenGL", NULL, NULL); 
 
    // Привязываются функции для обработки событий 
    glfwSetWindowSizeCallback(window, window_size_callback); 
    glfwSetKeyCallback(window, key_callback); 
 
    // Активируется контекст окна 
    glfwMakeContextCurrent(window); 
 
    // Подключение новейшей из доступных версий OpenGL 
    glewExperimental = GL_TRUE; 
 
    // Подключение функций OpenGL 
    if (glewInit() != GLEW_OK) 
    { 
        cerr << "ERROR: could not start GLEW\n"; 
        return nullptr; 
    } 
 
    // Если дошли до сюда - возвращаем окно 
    return window; 
} 
 
void EndAll() 
{ 
    // Просто выключение GLFW 
    // Сюда же можно добавить очистку памяти, если будет  
    //нужно 
    glfwTerminate(); 
} 
 
void window_size_callback(GLFWwindow* window, int width, int height) 
{ 
    // Просто меняем глобальные переменные 
    WinWidth = width; 
    WinHeight = height; 
} 
 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{ 
    //Сюда следует добавить обработку клавиш 
}
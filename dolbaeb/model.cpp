#include "model.h" 
#include "func.h" 
#include "globals.h" 
 
void Model::render(GLuint mode) 
{ 
    //Самый простой вариант - просто рисует,  
    //ничего в шейдер не передаёт. 
    //Выбираем шейдерную программу 
    glUseProgram(shader_programme); 
    //Активируем массив вершин 
    glBindVertexArray(vao); 
    //Если индексы есть - рисуем по ним 
    if (indices_count > 0) 
        glDrawElements(mode, indices_count, GL_UNSIGNED_INT, 0); 
    else 
        glDrawArrays(mode, 0, verteces_count); 
} 
 
void Model::load_coords(glm::vec3* verteces, size_t count) 
{ 
    //Сохранение размера массива 
    verteces_count = count; 
 
    //Создание VBO 
    GLuint coords_vbo = 0; 
    glGenBuffers(1, &coords_vbo); 
    glBindBuffer(GL_ARRAY_BUFFER, coords_vbo); 
    glBufferData(GL_ARRAY_BUFFER, count * sizeof (glm::vec3), verteces, GL_STATIC_DRAW); 
 
    //Так как VAO уже создан, то можно сразу связать с ним 
    glBindVertexArray(vao); 
 
    //Собственно связь 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
 
    //Подключение атрибута 
    glEnableVertexAttribArray(0); 
 
} 
 
void Model::load_colors(glm::vec3* colors, size_t count) 
{ 
    //Аналогично координатам 
        //Размер сохранять не требуется 
    // – важно только число вершин 
    GLuint colors_vbo = 0; 
    glGenBuffers(1, &colors_vbo); 
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo); 
    glBufferData(GL_ARRAY_BUFFER, count * sizeof (glm::vec3), colors, GL_STATIC_DRAW); 
 
    glBindVertexArray(vao); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
    glEnableVertexAttribArray(1); 
} 
 
void Model::load_indices(GLuint* indices, size_t count) 
{ 
    //Сохранение числа индексов 
    indices_count = count; 
 
    glBindVertexArray(vao); 
 
    GLuint elementbuffer; 
    glGenBuffers(1, &elementbuffer); // Генерация одного 
    // объекта буфера вершин 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer); // Привязка элементного буфера 
    //Загрузка индексов в используемый элементный буфер 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
count 
* 
sizeof 
(GLuint), 
indices, 
GL_STATIC_DRAW); 
 
} 
 
void Model::load_shaders(const char* vect, const char* frag) 
{ 
    // Переменные под результат компиляции программы 
    GLint result = GL_FALSE; 
    int infoLogLength; 
 
    //Создание шейдерной программы 
    shader_programme = glCreateProgram(); 
 
    //Загрузка текстов шейдеров из файлов 
    string vstext = LoadShader(vect); 
    const char* vertex_shader = vstext.c_str(); 
    string fstext = LoadShader(frag); 
    const char* fragment_shader = fstext.c_str(); 
 
    //Создание вершинного шейдера 
    GLuint vs = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vs, 1, &vertex_shader, NULL); 
    glCompileShader(vs); 
 
    //Проверка результата компиляции 
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result); 
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength); 
    //Вывод сообщения об ошибке если что-то пошло не так 
    if (infoLogLength > 0) 
    { 
        char* errorMessage = new char[infoLogLength + 1]; 
        glGetShaderInfoLog(vs, infoLogLength, NULL, errorMessage); 
        std::cout << errorMessage; 
        delete errorMessage; 
    } 
 
    //Аналогично с фрагментным шейдером 
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fs, 1, &fragment_shader, NULL); 
    glCompileShader(fs); 
 
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result); 
    glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLogLength); 
    if (infoLogLength > 0) 
    { 
        char* errorMessage = new char[infoLogLength + 1]; 
        glGetShaderInfoLog(fs, infoLogLength, NULL, errorMessage); 
        std::cout << errorMessage; 
        delete errorMessage; 
    } 
 
    //Сборка программы 
    glAttachShader(shader_programme, vs); 
    glAttachShader(shader_programme, fs); 
 
    //Тонкий момент – тут жёстко связываются  
    //названия входных переменных в шейдерах  
    //с атрибутами массива вершин 
    //Это лучше вынести в отдельные функции для большей 
    //универсальности. 
    //Однако так как это пример, то оставлено так 
    glBindAttribLocation(shader_programme, 0, "vertex_position"); 
    glBindAttribLocation(shader_programme, 1, "vertex_color"); 
 
    //Компоновка шейдерной программы 
    glLinkProgram(shader_programme); 
 
} 
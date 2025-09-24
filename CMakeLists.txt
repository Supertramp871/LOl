cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# Поиск необходимых библиотек
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "src/*.h" "src/*.hpp")

add_executable(main ${SOURCES} ${HEADERS})

target_link_libraries(main 
    OpenGL::GL 
    GLEW::GLEW 
    glfw
)

# Включение директорий
#target_include_directories(main PRIVATE ${GLEW_INCLUDE_DIRS})
target_include_directories(main PRIVATE 
    ${GLEW_INCLUDE_DIRS}
    src/
)

# Добавляем папку src в include пути для удобства включения заголовков
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
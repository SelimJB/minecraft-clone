cmake_minimum_required(VERSION 3.0.0)
project(opengllearning VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Debug)
include(CTest)
enable_testing()

include_directories(./inc)
link_directories(./lib)

set(SRCS
./src/main.cpp
./src/glad.c
./src/textrenderer.cpp
./src/shader.cpp
    )
    
set(HEADERS
./inc/glad/glad.h
./inc/GLFW/glfw3.h
)
	
	
add_executable(main ${SRCS} ${HEADERS})

target_link_libraries(main glfw3)
target_link_libraries(main freetype)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

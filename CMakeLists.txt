cmake_minimum_required(VERSION 3.20)

# 如果未设置构建类型，则默认设置为 Release 模式
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

project(RGS 
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/
)

link_directories(
    # ${CMAKE_SOURCE_DIR}/3rdlib/glad/
)

# file(GLOB GLAD_SRC ${CMAKE_SOURCE_DIR}/3rdlib/glad/src/glad.c)

set(
    HEADERS 

    ${CMAKE_SOURCE_DIR}/src/Bace.h
    ${CMAKE_SOURCE_DIR}/src/Application.h
    ${CMAKE_SOURCE_DIR}/src/Window.h
    ${CMAKE_SOURCE_DIR}/src/WindowsWindow.h
    ${CMAKE_SOURCE_DIR}/src/InputCodes.h
    ${CMAKE_SOURCE_DIR}/src/Maths.h
    ${CMAKE_SOURCE_DIR}/src/Framebuffer.h
)

set(
    SOURCES
    
    ${CMAKE_SOURCE_DIR}/Src/Application.cpp
    ${CMAKE_SOURCE_DIR}/Src/Window.cpp
    ${CMAKE_SOURCE_DIR}/Src/WindowsWindow.cpp
    ${CMAKE_SOURCE_DIR}/Src/Maths.cpp
    ${CMAKE_SOURCE_DIR}/Src/Framebuffer.cpp
)

#set(EXECUTABLE_OUTPUT_PATH output) #设置可执行文件的生成路径

add_executable(
            RGS

            main.cpp

            ${HEADERS}
            ${SOURCES} 
)

# target_link_libraries(
#             RGS 
#             PRIVATE 
            
#             libglfw3.a
#             libglad.a
# )
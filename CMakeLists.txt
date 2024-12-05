cmake_minimum_required(VERSION 3.29)
project(ByteOperations)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(ByteOperations
    fileMan.cpp
    fileMan.h
    image_utils.h
    main.cpp
    memoryMan.cpp
    memoryMan.h
    OpenGLMan.cpp
    OpenGLMan.h
    test.cpp)
target_link_libraries(ByteOperations
        "H:/Dev/C++/ByteOperations/libraries/freeglut.lib"
        "H:/Dev/C++/ByteOperations/libraries/freeglut_static.lib"
        "H:/Dev/C++/ByteOperations/libraries/glfw3dll.lib"
        "H:/Dev/C++/ByteOperations/libraries/glfw3.dll"
        "H:/Dev/C++/ByteOperations/libraries/glfw3.lib")
cmake_minimum_required(VERSION 3.13.4)
project(PhysicsSimulation)

set(CMAKE_CXX_STANDARD 17)

set(LIB_ROOT /Users/akarshkumar0101/Developer/Library/CppLibraries/)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)


add_executable(${PROJECT_NAME}
        src/main.cpp
        #src/testing.cpp
        #src/imguiAndOpenGL.cpp

        src/RigidBody.h src/RigidBody.cpp
        src/Physics.h
        src/PhysicsSimulation.h

        )


target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/GLM/glm/)

target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/Glad/glad/include/)
target_link_libraries(PhysicsSimulation ${LIB_ROOT}/Glad/libGlad.a)

target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/ImGUI/)
target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/ImGUI/examples/)
target_link_libraries(PhysicsSimulation ${LIB_ROOT}/ImGUI/libImGUI.a)




target_include_directories(PhysicsSimulation PUBLIC ${OPENGL_INCLUDE_DIR})

target_link_libraries(PhysicsSimulation glfw)
target_link_libraries(PhysicsSimulation ${OPENGL_gl_LIBRARY})
target_link_libraries(PhysicsSimulation ${OPENGL_glu_LIBRARY})



target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/../LibAKCpp/include/)
target_link_libraries(PhysicsSimulation ${LIB_ROOT}/../LibAKCpp/cmake-build-debug/libak.a)


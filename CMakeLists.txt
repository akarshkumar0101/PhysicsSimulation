cmake_minimum_required(VERSION 3.13.4)
project(PhysicsSimulation)

set(CMAKE_CXX_STANDARD 17)

set(LIB_ROOT /Users/akarshkumar0101/Developer/Library/CppLibraries/)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)


add_executable(${PROJECT_NAME}
        #Libraries/GL/glad.c
        #${IMGUI_SRC}
        src/main.cpp
        #src/testing.cpp
        #src/imguiAndOpenGL.cpp
        src/math/Vector.h src/math/VectorOps.h src/math/Matrix.h src/math/MatrixOps.h src/RigidBody.h src/PhysicsSimulation.h src/math/Quaternion.h src/Physics.h src/math/QuaternionOps.h src/SimulationDisplay.h src/Shader.h src/Camera.h src/Renderer.h src/GraphicsBuffer.h src/Shader.cpp src/GraphicsBuffer.cpp src/Window.cpp src/Window.h src/Camera.cpp src/Renderer.cpp src/RigidBody.cpp
        src/Node.h src/Node.cpp src/GridNode.cpp src/GridNode.h src/DrawingNode.cpp src/DrawingNode.h
        )




target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/Glad/glad/include/)
target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/GLM/glm/)
target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/ImGUI/)
target_include_directories(PhysicsSimulation PRIVATE ${LIB_ROOT}/ImGUI/examples/)

target_include_directories(PhysicsSimulation PUBLIC ${OPENGL_INCLUDE_DIR})

target_link_libraries(PhysicsSimulation glfw)
target_link_libraries(PhysicsSimulation ${OPENGL_gl_LIBRARY})
target_link_libraries(PhysicsSimulation ${OPENGL_glu_LIBRARY})

target_link_libraries(PhysicsSimulation ${LIB_ROOT}/Glad/libGlad.a)
target_link_libraries(PhysicsSimulation ${LIB_ROOT}/ImGUI/libImGUI.a)



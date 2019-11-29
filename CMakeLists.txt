cmake_minimum_required(VERSION 3.14)
project(PhysicsSimulation)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)



set(CMAKE_CXX_STANDARD 17)

add_executable(PhysicsSimulation main.cpp math/Vector.h math/VectorOps.h math/Matrix.h math/MatrixOps.h RigidModel.h Simulation.h math/Quaternion.h Physics.h glad.c math/QuaternionOps.h TestingGL.h Shader.h ModelData.h GLFWWrapper.h Camera.h Renderer.h GraphicsBuffer.h)


target_link_libraries(PhysicsSimulation glfw)

target_include_directories(PhysicsSimulation PRIVATE GL/glad/include/)
target_include_directories(PhysicsSimulation PRIVATE GLM/glm/)

target_include_directories(PhysicsSimulation PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(PhysicsSimulation ${OPENGL_gl_LIBRARY})
target_link_libraries(PhysicsSimulation ${OPENGL_glu_LIBRARY})

#target_link_libraries(PhysicsSimulation glfw)
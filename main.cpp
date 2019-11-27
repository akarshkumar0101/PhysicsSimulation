#include <iostream>
#include <thread>

#include "math/Vector.h"
#include "math/VectorOps.h"
#include "math/Matrix.h"
#include "math/MatrixOps.h"

#include "RigidModel.h"
#include "Simulation.h"

#include "math/Quaternion.h"

#include "SimulationDisplay.h"
#include "TestingGL.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    using namespace std;
    using namespace AKMath;

    cout<<"hello"<<endl;

    testGL();


//    std::array<PointMass3D,2> arr = {PointMass(1, Vector3D({0,0,0})),PointMass(1, Vector3D({0,1,0}))};
//    RigidModel3D model(arr);

    int width = 500, height = 600;
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);

    glm::vec4 point(1,2,3,1);

    for(int j=0;j<4;j++){
        for(int i=0;i<4;i++){
            cout<<proj[j][i]<<" ";
        }
        cout<<endl;
    }
    glm::vec4 p2 = proj*point;
    for(int i=0;i<4;i++){
        cout<<p2[i]<<" ";
    }
    cout<<endl;


    PhysicsSimulation simulation;

    //std::thread* thread = simulation.simulateOnThread();
    //thread->join();


    cout<<"hello2"<<endl;

    return 0;
}
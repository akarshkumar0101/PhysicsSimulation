//
// Created by Akarsh Kumar on 12/20/19.
//

#include "RigidBody.h"
#include "PhysicsSimulation.h"

RigidBody::RigidBody() {
}

template<size_t M>
RigidBody::RigidBody(const std::array<PointMass, M> &stdArray): RigidBody() {
    mPointMasses.insert(mPointMasses.begin(), std::begin(stdArray), std::end(stdArray));
    establishModel();
}

void RigidBody::establishModel(){
    if(establishedModel){
        return;
    }
    establishedModel = true;

    //center of mass;
    Point cm(0.0);

    mMass = 0.0f;
    for(const PointMass& pm: mPointMasses){
        mMass += pm.mass();
        cm += (pm.mass()*pm.r());
    }
    cm/=mMass;
    //found center of mass now

    // realign everything to center of mass
    for(PointMass& pm: mPointMasses){
        pm.r() -= cm;
    }
    cm -= cm;

    mX = glm::vec3(0.3,0.0,0.0);
    //mR = glm::mat3(1.0);
    mQ = glm::quat(glm::vec3(0.0));
    mP = glm::vec3(0.0,0.0,0.0);
    mL = glm::vec3(0.0,0.0,0.0);

    mIBody = glm::mat4(0.0);
    for(PointMass& pm: mPointMasses){
        glm::vec3 rp = pm.r();

        glm::vec3 r0i = glm::transpose(mR) * rp;
        r0i = rp;

        float in = glm::dot(r0i,r0i);
        glm::mat3 out = glm::outerProduct(r0i,r0i);

        glm::mat3 lol = in * glm::mat3(1.0) - out;
        mIBody += pm.mass()* (lol);
    }

    if(glm::determinant(mIBody) == 0) {
        throw "Cannot create inverse body matrix";
    }

    mIBodyInv = glm::inverse(mIBody);

    computeDerivedQuantities();
}

glm::mat3 star(glm::vec3 a){
    glm::mat3 ans(0.0);
    ans[0][1] = -a.z;
    ans[0][2] = a.y;

    ans[1][0] = a.z;
    ans[1][2] = -a.x;

    ans[2][0] = -a.y;
    ans[2][1] = a.x;

    ans = glm::transpose(ans);

    return ans;
}

void RigidBody::computeDerivedQuantities(){
    mR = glm::toMat3(mQ);

    mV = mP / mMass;
    mI = mR * mIBody * glm::transpose(mR);

    // following two should be the same:

    mIInv = mR * mIBodyInv * glm::transpose(mR);
    // mIInv = glm::inverse(mI);

    mW = mIInv * mL;
//    std::cout<<""+3<<std::endl;
}

float prevKin = 0;
float prevPot = 0;

int i=0;
void RigidBody::update(float dt, Force force, Torque torque) {
    float det = glm::determinant(mR);

    mP += force * dt;
    mL += torque * dt;

    computeDerivedQuantities();

    mX += mV * dt;
//    mR += star(mW) * mR * dt;
    mQ += glm::quat(0,mW) * mQ / 2.0f;
    mQ = glm::normalize(mQ);

    computeDerivedQuantities();

    float kin = glm::dot(mP,mP)/(2.0f*mMass);//+ (glm::dot(mL,mI*mL)/2.0f);
    float pot = (mX.x*mX.x)/2.0f;

    if(i++%100==0) {
//        std::cout << std::endl << std::endl;
//        std::cout << "dkin = " << kin - prevKin << std::endl;
//        std::cout << "dpot = " << pot - prevPot << std::endl;
//        std::cout << "den = " << pot + kin - prevKin - prevPot << std::endl;
//        std::cout << "doriginal = " << pot + kin - 1/8.0f << std::endl;
    }

    prevKin = kin;
    prevPot = pot;

}

void RigidBody::update(float dt, const PhysicsSimulation& simulation) {
    dt = 0.1;
    Force force(0.0);
    Torque torque(0.0);

    for(PointMass& pm: mPointMasses){
        glm::vec3 r = mR * pm.r() + mX;
        glm::vec3 rp = r-mX;

        Force forceI = simulation.forceAt(r);

        force += forceI;
        torque += glm::cross(rp,forceI);
    }

    update(dt, force, torque);
}


std::vector<PointMass> &RigidBody::pointMasses() const {
    return (std::vector<PointMass>&) mPointMasses;
}








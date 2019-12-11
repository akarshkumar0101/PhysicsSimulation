//
// Created by Akarsh Kumar on 11/27/19.
//

#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


enum CameraMovement {
    FORWARD, BACKWARD, LEFTWARD, RIGHTWARD, UPWARD, DOWNWARD
};

class Camera {
public:
    static constexpr float DEFAULT_MOVEMENT_SPEED = 1.0f;
    static constexpr float DEFAULT_LOOKING_SPEED = 0.01f;
    static constexpr float DEFAULT_FOVY = 45.0f;
private:
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;

    glm::vec3 mWorldUp;


    float mMovementSpeed;
    float mLookingSpeed;
    float mFOVy;

    void updateVectors(){
        //mPosition and mFront are set
        mRight = glm::cross(mFront, mWorldUp);
        mUp = glm::cross(mRight, mFront);
    }
public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp) : mPosition(position), mFront(front), mWorldUp(worldUp),
                                                                     mMovementSpeed(DEFAULT_MOVEMENT_SPEED),
                                                                     mLookingSpeed(DEFAULT_LOOKING_SPEED),
                                                                     mFOVy(DEFAULT_FOVY) {
        updateVectors();
    }

    glm::mat4 computeViewMatrix() const{
        return glm::lookAt(mPosition, mPosition + mFront, mUp);
    }
    glm::mat4 computePerspectiveProjectionMatrix(float aspectRatio) const {
        return glm::perspective(glm::radians(mFOVy), aspectRatio, 0.1f, 100.0f);
    }
    glm::mat4 computePerspectiveOrthogonalMatrix() const {
        return glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    }

    void move(CameraMovement movement, float dt) {
        float speed = mMovementSpeed * dt;
        switch (movement) {
            case FORWARD: {
                mPosition += mFront * speed;
                break;
            }
            case BACKWARD: {
                mPosition -= mFront * speed;
                break;
            }
            case RIGHTWARD: {
                mPosition += mRight * speed;
                break;
            }
            case LEFTWARD: {
                mPosition -= mRight * speed;
                break;
            }
            case UPWARD: {
                mPosition += mUp * speed;
                break;
            }
            case DOWNWARD: {
                mPosition -= mUp * speed;
                break;
            }
        }
    }

    void look(float xOffset, float yOffset) {
        xOffset *= -mLookingSpeed;
        yOffset *= -mLookingSpeed;

        glm::mat4 rotationalMat(1.0);
        rotationalMat = glm::rotate(rotationalMat, xOffset, mUp);
        rotationalMat = glm::rotate(rotationalMat, yOffset, mRight);

        mFront = rotationalMat * glm::vec4(mFront,1.0);

        updateVectors();
    }

};
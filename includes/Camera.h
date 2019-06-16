#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

//--------------------------------------------------------------
// Abstract Camera Class
//--------------------------------------------------------------
class Camera
{
public:
    glm::mat4 getViewMatrix() const;

    virtual void setPosition(const glm::vec3 &position) {}
    virtual void rotate(float yaw, float pitch) {} // in degrees
    virtual void move(const glm::vec3 &offsetPos) {}

    const glm::vec3 &getLook() const;
    const glm::vec3 &getRight() const;
    const glm::vec3 &getUp() const;
    const glm::vec3 &getPosition() const;

    float getFOV() const { return mFOV; }
    void setFOV(float fov) { mFOV = fov; } // in degrees

protected:
    Camera();

    virtual void updateCameraVectors() {}

    glm::vec3 mPosition;
    glm::vec3 mTargetPos;
    glm::vec3 mLook;
    glm::vec3 mUp;
    glm::vec3 mRight;
    const glm::vec3 WORLD_UP;

    // Euler Angles (in radians)
    float mYaw;
    float mPitch;

    // Camera parameters
    float mFOV; // degrees
};

//--------------------------------------------------------------
// Orbit Camera Class
//--------------------------------------------------------------
class OrbitCamera : public Camera
{
public:
    OrbitCamera();

    virtual void rotate(float yaw, float pitch); // in degrees

    // Camera Controls
    void setLookAt(const glm::vec3 &target);
    void setRadius(float radius);
    void move(glm::vec3 offsetPos);

private:
    void updateCameraVectors(glm::vec3 offset_);

    // Camera parameters
    float mRadius;
};
#endif //CAMERA_H

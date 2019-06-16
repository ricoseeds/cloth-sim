#include <iostream>
#include "../includes/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

const float DEF_FOV = 100.0f; // degrees

Camera::Camera()
    : mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
      mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
      mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      mRight(1.0f, 0.0f, 0.0f),
      WORLD_UP(0.0f, 1.0f, 0.0f),
      mYaw(glm::pi<float>()),
      mPitch(0.0f),
      mFOV(DEF_FOV)
{
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(mPosition, mTargetPos, mUp);
}

const glm::vec3 &Camera::getLook() const
{
    return mLook;
}

const glm::vec3 &Camera::getRight() const
{
    return mRight;
}

const glm::vec3 &Camera::getUp() const
{
    return mUp;
}

const glm::vec3 &Camera::getPosition() const
{
    return mPosition;
}

OrbitCamera::OrbitCamera()
    : mRadius(10.0f)
{
}

void OrbitCamera::setLookAt(const glm::vec3 &target)
{
    mTargetPos = target;
}

void OrbitCamera::setRadius(float radius)
{
    // Clamp the radius
    mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::rotate(float yaw, float pitch)
{
    mYaw = glm::radians(yaw);
    mPitch = glm::radians(pitch);

    mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors(glm::vec3(0.0f, 0.0f, 0.0f));
}

void OrbitCamera::updateCameraVectors(glm::vec3 offset_cam)
{
    mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
    mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
    mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}
void OrbitCamera::move(glm::vec3 offsetPos)
{
    updateCameraVectors(offsetPos);
}
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Config.hpp>
#include <overkill/Entity.hpp>
#include <overkill/Scene.hpp>


namespace overkill
{
    
struct CameraTransform      // Not to be confused with the cameras actual transform defined in Entity.
{                           // This struct is used to convey info to main and shader.
    glm::vec3 position;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};

class EntityCamera : public Entity
{
private:
    float m_FOV;
    float m_aspectRatio;
    float m_nearClip;
    float m_farClip;

    glm::mat4 getViewMatrix(glm::mat4 parentMatrix = glm::mat4(1));

public:
    CameraTransform m_cameraTransform;

    EntityCamera(C::Tag entityTag, int entityID, 
                glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 rot = glm::vec3(0,0,0), 
                glm::vec3 vel = glm::vec3(0,0,0), glm::vec3 angVel = glm::vec3(0,0,0),
                float FOV = C::FOV, float aspectRatio = C::AspectRatio, 
                float nearClip = C::NearClip, float farClip = C::FarClip);

    void update(float dt, glm::mat4 parentMatrix = glm::mat4(1));
    void draw(float t);
};

}
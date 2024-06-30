#pragma once

#include <glm/glm/glm.hpp>
#include <vector>
#include <memory>

#include "Graphics/Shader.h"

class Light {
public:
    glm::vec3 color;
    
    Light(const glm::vec3& color);
    virtual ~Light();

    virtual void SendToShader(Shader& shader, const std::string& name) = 0;
};

class DirectionalLight : public Light {
public:
    glm::vec3 direction;

    DirectionalLight(const glm::vec3& color, const glm::vec3& direction);
    void SendToShader(Shader& shader, const std::string& name) override;
};

class PointLight : public Light {
public:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    PointLight(const glm::vec3& color, const glm::vec3& position, float constant, float linear, float quadratic);
    void SendToShader(Shader& shader, const std::string& name) override;
};

class SpotLight : public Light {
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction, float cutOff, float outerCutOff);
    void SendToShader(Shader& shader, const std::string& name) override;
};

class LightManager {
public:
    std::vector<std::shared_ptr<Light>> lights;

    LightManager();
    ~LightManager();

    void addLight(std::shared_ptr<Light> light);
    void removeLight(std::shared_ptr<Light> light);
};
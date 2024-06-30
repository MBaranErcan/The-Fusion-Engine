#include "Graphics/Light.h"

// Light class
Light::Light(const glm::vec3& color) : color(color) {}

Light::~Light() {}


// DirectionalLight class
DirectionalLight::DirectionalLight(const glm::vec3& color, const glm::vec3& direction)
    : Light(color), direction(direction) {}

void DirectionalLight::SendToShader(Shader& shader, const std::string& name) {
    shader.setVec3(name + ".color", color);
    shader.setVec3(name + ".direction", direction);
}


// PointLight class
PointLight::PointLight(const glm::vec3& color, const glm::vec3& position, float constant, float linear, float quadratic)
    : Light(color), position(position), constant(constant), linear(linear), quadratic(quadratic) {}

void PointLight::SendToShader(Shader& shader, const std::string& name) {
    shader.setVec3(name + ".color", color);
    shader.setVec3(name + ".position", position);
    shader.setFloat(name + ".constant", constant);
    shader.setFloat(name + ".linear", linear);
    shader.setFloat(name + ".quadratic", quadratic);
}


// SpotLight class
SpotLight::SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction, float cutOff, float outerCutOff)
    : Light(color), position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff) {}

void SpotLight::SendToShader(Shader& shader, const std::string& name) {
    shader.setVec3(name + ".color", color);
    shader.setVec3(name + ".position", position);
    shader.setVec3(name + ".direction", direction);
    shader.setFloat(name + ".cutOff", cutOff);
    shader.setFloat(name + ".outerCutOff", outerCutOff);
}


// LightManager class
LightManager::LightManager() {}

LightManager::~LightManager() {}

void LightManager::addLight(std::shared_ptr<Light> light) {
    lights.push_back(light);
}

void LightManager::removeLight(std::shared_ptr<Light> light) {
    lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());
}

#include "Light.h"
#include "GL/gl3w.h"

void PointLight::bind_at(unsigned int slot, std::string array_name, const Shader &shader) const
{
    std::string positionUniformName = array_name + "[" + std::to_string(slot) + "].position";
    std::string ambientUniformName = array_name + "[" + std::to_string(slot) + "].ambient";
    std::string diffuseUniformName = array_name + "[" + std::to_string(slot) + "].diffuse";
    std::string specularUniformName = array_name + "[" + std::to_string(slot) + "].specular";
    std::string constantUniformName = array_name + "[" + std::to_string(slot) + "].constant";
    std::string linearUniformName = array_name + "[" + std::to_string(slot) + "].linear";
    std::string quadraticUniformName = array_name + "[" + std::to_string(slot) + "].quadratic";

    GLuint positionLocation = shader.get_uniform_location(positionUniformName);
    GLuint diffuseLocation = shader.get_uniform_location(diffuseUniformName);
    GLuint specularLocation = shader.get_uniform_location(specularUniformName);
    GLuint constantLocation = shader.get_uniform_location(constantUniformName);
    GLuint linearLocation = shader.get_uniform_location(linearUniformName);
    GLuint quadraticLocation = shader.get_uniform_location(quadraticUniformName);
    GLuint ambientLocation = shader.get_uniform_location(ambientUniformName);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform3f(diffuseLocation, diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(specularLocation, specular.x, specular.y, specular.z);
    glUniform3f(ambientLocation, ambient.x, ambient.y, ambient.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(quadraticLocation, quadratic);

}

void DirectionalLight::bind(const Shader& shader) const
{

    const std::string array_name = "directional_light";
    std::string directionUniformName = array_name + ".direction";
    std::string ambientUniformName = array_name + ".ambient";
    std::string diffuseUniformName = array_name + ".diffuse";
    std::string specularUniformName = array_name + ".specular";

    // Get the uniform locations using the shader's get_uniform method
    GLuint directionLocation = shader.get_uniform_location(directionUniformName);
    GLuint ambientLocation = shader.get_uniform_location(ambientUniformName);
    GLuint diffuseLocation = shader.get_uniform_location(diffuseUniformName);
    GLuint specularLocation = shader.get_uniform_location(specularUniformName);

    // Set the uniform values using glUniform* functions
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform3f(ambientLocation, ambient.x, ambient.y, ambient.z);
    glUniform3f(diffuseLocation, diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(specularLocation, specular.x, specular.y, specular.z);

}

void SpotLight::bind_at(unsigned int slot, const std::string& array_name, const Shader& shader) const
{
    std::string positionUniformName = array_name + "[" + std::to_string(slot) + "].position";
    std::string directionUniformName = array_name + "[" + std::to_string(slot) + "].direction";
    std::string ambientUniformName = array_name + "[" + std::to_string(slot) + "].ambient";
    std::string diffuseUniformName = array_name + "[" + std::to_string(slot) + "].diffuse";
    std::string specularUniformName = array_name + "[" + std::to_string(slot) + "].specular";
    std::string cutoffUniformName = array_name + "[" + std::to_string(slot) + "].cutoff";
    std::string outerCutoffUniformName = array_name + "[" + std::to_string(slot) + "].outer_cutoff";

    GLuint positionLocation = shader.get_uniform_location(positionUniformName);
    GLuint directionLocation = shader.get_uniform_location(directionUniformName);
    GLuint ambientLocation = shader.get_uniform_location(ambientUniformName);
    GLuint diffuseLocation = shader.get_uniform_location(diffuseUniformName);
    GLuint specularLocation = shader.get_uniform_location(specularUniformName);
    GLuint cutoffLocation = shader.get_uniform_location(cutoffUniformName);
    GLuint outerCutoffLocation = shader.get_uniform_location(outerCutoffUniformName);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform3f(ambientLocation, ambient.r, ambient.g, ambient.b);
    glUniform3f(diffuseLocation, diffuse.r, diffuse.g, diffuse.b);
    glUniform3f(specularLocation, specular.r, specular.g, specular.b);
    glUniform1f(cutoffLocation, cutoff);
    glUniform1f(outerCutoffLocation, outer_cutoff);

}

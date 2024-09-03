#ifndef FPS_ENGINE_SHADER_HPP
#define FPS_ENGINE_SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void set_uniform(const char* uniform_name, float value);
    void set_uniform(const char* uniform_name, double value);
    void set_uniform(const char* uniform_name, int value);
    void set_uniform(const char* uniform_name, const glm::mat4& value);
    void set_uniform(const char* uniform_name, const glm::vec3& value);
    void use();
};

#endif
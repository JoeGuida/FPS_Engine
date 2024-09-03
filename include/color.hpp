#ifndef FPS_ENGINE_COLOR_HPP
#define FPS_ENGINE_COLOR_HPP

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Color {
    float r;
    float g;
    float b;
    float a;

    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
    Color(float s) : r(s), g(s), b(s), a(1.0f) {}
    Color(const Color&) = default;
    Color(Color&&) = default;
    virtual ~Color() = default;

    Color& operator=(const Color& color) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
        return *this;
    }

    operator glm::vec3() const { return glm::vec3(r, g, b); }
    operator glm::vec4() const { return glm::vec4(r, g, b, a); }

    static Color Red;
    static Color Green;
    static Color Blue;
    static Color Yellow;
    static Color Purple;
    static Color Cyan;
    static Color White;
    static Color Black;
};

#endif
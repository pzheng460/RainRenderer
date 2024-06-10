#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
public:
    Light(const glm::vec3& position, const glm::vec3& color)
            : position(position), color(color) {}
    void setPosition(const glm::vec3& position) { this->position = position; }
    void setColor(const glm::vec3& color) { this->color = color; }
    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getColor() const { return color; }
    friend class GUI;
private:
    glm::vec3 position;
    glm::vec3 color;
};

#endif // LIGHT_H

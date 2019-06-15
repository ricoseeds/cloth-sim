#ifndef PARTICLE_H
#define PARTICLE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/norm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "../includes/Spring.h"
#include <vector>

class Particle
{
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 total_force;
    std::vector<Spring> springs;
    double mass;
    bool fixed;

public:
    Particle(glm::vec3 pos);
    void toggle_fixed();
};
#endif
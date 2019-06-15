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
    void add_spring(int from, int to, double spring_const);
    glm::vec3 get_position()
    {
        return position;
    }
    int get_spring_count()
    {
        return springs.size();
    }
};
#endif
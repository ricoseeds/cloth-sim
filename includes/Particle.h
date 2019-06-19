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
    void add_spring(int, int, double, double);
    void unfix();
    glm::vec3
    get_position()
    {
        return position;
    }
    double get_mass()
    {
        return mass;
    }
    glm::vec3 set_position_for_fixed_points(glm::vec3 pos)
    {
        position = pos;
    }
    glm::vec3 set_position(double deltaT)
    {
        position = position + (velocity * (float)(deltaT));
    }
    glm::vec3 get_velocity()
    {
        return velocity;
    }
    void set_velocity(glm::vec3 force, double deltaT)
    {
        velocity = velocity + (force * (float)((1 / mass) * deltaT));
    }
    int get_spring_count()
    {
        return springs.size();
    }
    int get_spring_destination(int index)
    {
        return springs[index].get_destination();
    }
    glm::vec3 calc_force_on_particle(int index, glm::vec3 xa, glm::vec3 xb)
    {
        return springs[index].calculate_spring_force(xa, xb, velocity);
    }
    bool is_fixed()
    {
        return fixed;
    }
};
#endif
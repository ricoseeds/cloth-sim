#include "../includes/Particle.h"
#include <iostream>
Particle::Particle(glm::vec3 pos)
{
    fixed = false;
    mass = 1.0f;
    position = pos;
    velocity = glm::vec3(0.0, 0.0, 0.0);
    total_force = glm::vec3(0.0, 0.0, 0.0);
}
void Particle::toggle_fixed()
{
    fixed = !fixed;
}
void Particle::add_spring(int from, int to, double spring_const)
{
    springs.emplace_back(from, to, spring_const);
}
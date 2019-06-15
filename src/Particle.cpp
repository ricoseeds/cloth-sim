#include "../includes/Particle.h"
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
void Particle::add_spring(Spring *spring)
{
    springs.push_back(spring);
}
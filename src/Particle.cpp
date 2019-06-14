#include "../includes/Particle.h"
Particle::Particle()
{
    fixed = false;
    mass = 1.0f;
}
void Particle::toggle_fixed()
{
    fixed = !fixed;
}
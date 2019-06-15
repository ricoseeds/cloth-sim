#include "../includes/Spring.h"

Spring::Spring(int from, int to)
{
    ks = 2.0f;
    kd = 1.0f;
    l0 = 1.0f;
    orig = from;
    dest = to;
}

glm::vec3 Spring::calculate_spring_force(glm::vec3 xa, glm::vec3 xb, double mass, glm::vec3 velocity)
{
    glm::vec3 total_force = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 xa_minus_xb = xa - xb;
    glm::vec3 normalized_dir = glm::normalize(xa - xb);
    total_force = normalized_dir * (float)(ks * (glm::length(xa_minus_xb) - l0));
    total_force += glm::vec3(0.0, -9.8, 0) * (float)mass;
    total_force += normalized_dir * (float)(glm::dot(velocity, normalized_dir));
    return total_force;
}

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/norm.hpp"
#include <glm/gtc/type_ptr.hpp>

class Particle
{
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 internal_force;
    glm::vec3 external_force;
    glm::vec3 total_force;
    double mass;
    bool fixed;

public:
    Particle()
    {
        fixed = false;
        mass = 1.0f;
    }
    void toggle_fixed()
    {
        fixed = !fixed;
    }
};
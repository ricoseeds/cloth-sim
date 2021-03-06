#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/norm.hpp"
#include <glm/gtc/type_ptr.hpp>

#ifndef SPRING_H
#define SPRING_H

// tal force = mg − ks(‖x‖ − l0) e − kd(v · e) e + fuser
class Spring
{
private:
    double ks;
    double kd;
    double l0;
    int orig, dest;

public:
    Spring(int, int, double, double);
    glm::vec3 calculate_spring_force(glm::vec3 xa, glm::vec3 xb, glm::vec3 velocity);
    int get_destination()
    {
        return dest;
    }
    int get_origin()
    {
        return orig;
    }
};
#endif
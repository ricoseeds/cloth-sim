#include "../includes/Geometry.h"
#define epsilon 0.000001

float geometry_k::sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool geometry_k::point_in_tri(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

bool geometry_k::get_line_intersection(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float *i_x, float *i_y)
{
    // if ((abs(p0.x - p2.x) < epsilon && abs(p0.y - p2.y) < epsilon) || (abs(p3.x - p1.x) < epsilon && abs(p3.y - p1.y) < epsilon))
    // {
    //     return false;
    // }

    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1.x - p0.x;
    s1_y = p1.y - p0.y;
    s2_x = p3.x - p2.x;
    s2_y = p3.y - p2.y;

    float s, t;
    s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0.x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0.y + (t * s1_y);
        return true;
    }

    return false; // No collision
}
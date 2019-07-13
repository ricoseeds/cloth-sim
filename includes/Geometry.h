#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "glm/glm.hpp"

namespace geometry_k
{
float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
bool point_in_tri(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);
bool get_line_intersection(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, float *i_x, float *i_y);
} // namespace geometry_k
#endif
// int main()
// {
//     float i_x, i_y;
//     bool test = geometry_k::get_line_intersection(glm::vec2(1.2, 6.3), glm::vec2(4.4, 3.3), glm::vec2(0, 0), glm::vec2(-1, -1), &i_x, &i_y);
//     if (test)
//     {
//         std::cout << i_x << " " << i_y << "\n";
//     }
//     else
//     {
//         std::cout << "No_intersection";
//     }
// }
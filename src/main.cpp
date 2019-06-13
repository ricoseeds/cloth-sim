#include <iostream>
#include "../includes/Particle.h"
#include "../includes/Gridify.h"
#include <Eigen/Dense>
using namespace Eigen;

int main()
{
    Gridify *grid;
    grid = new Gridify(4);
    grid->build_connections();
    MatrixXd conn = grid->get_mat();
    std::cout << conn << std::endl;
    return 0;
}
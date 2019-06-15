#ifndef GRIDIFY_H
#define GRIDIFY_H

#include <Eigen/Dense>
using namespace Eigen;
class Gridify
{
private:
    int size;
    MatrixXd connx;

public:
    Gridify(int n);
    void build_connections();
    MatrixXd get_mat()
    {
        return connx;
    }
};
#endif
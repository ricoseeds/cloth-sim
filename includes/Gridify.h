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
    Gridify(int n)
    {
        size = n;
        connx.resize(n * n, n * n);
        connx.setZero(n * n, n * n);
    }
    void build_connections();
    MatrixXd get_mat()
    {
        return connx;
    }
};
#endif
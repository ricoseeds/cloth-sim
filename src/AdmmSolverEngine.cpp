#include "../includes/AdmmSolverEngine.h"

void AdmmSolverEngine::admm_iter(Eigen::VectorXd &x, Eigen::VectorXd &v, double d_t)
{
    Eigen::VectorXd Y = x + (d_t * v);
    Eigen::VectorXd b = (M * Y) + (delta_t * delta_t * D.transpose() * y) + (rho * delta_t * delta_t * D.transpose() * z);
    Eigen::VectorXd x_k_plus_1 = solver.solve(b);
    if (solver.info() != Eigen::Success)
    {
        std::cerr << "solving failed";
        return;
    }
    x = x_k_plus_1;
    Eigen::VectorXd zi(3);
    double scalar_zi, factor_zi;
    for (int i = 0; i <= z.rows() - 3; i += 3)
    {
        zi = z.segment(i, 3);
        scalar_zi = (rho * D.block(i, 0, 3, 3 * x.size()) * x).norm() + (l[i / 3] * k[i / 3]);
        scalar_zi = scalar_zi / (k[i / 3] + rho);
        //solve for zi
        factor_zi = (k[i / 3] * ((zi.norm() - l[i / 3]) / zi.norm())) + rho;
        zi = (rho * D.block(i, 0, 3, 3 * x.size()) * x) - y.segment(i, 3);
        zi = zi / factor_zi;
        z.segment(i, 3) = zi;
    }
    // now we have updated x and z
    // update the dual variable
    y = y + rho * (D * x - z);
}

void AdmmSolverEngine::resize_M(int i, int j)
{
    M.resize(i, j);
}

void AdmmSolverEngine::resize_D(int i, int j)
{
    D.resize(i, j);
}

void AdmmSolverEngine::set_M(Eigen::SparseMatrix<double> &mat)
{
    M = mat;
}
void AdmmSolverEngine::set_D_and_compute_z(Eigen::SparseMatrix<double> &mat, Eigen::VectorXd &x)
{
    D = mat;
    z = D * x;
}
#include "../includes/AdmmSolverEngine.h"

void AdmmSolverEngine::run(double d_t)
{
    for (int i = 0; i < 20; i++)
    {
        admm_iter(d_t);
    }
}
void AdmmSolverEngine::admm_iter(double d_t)
{
    Eigen::VectorXd Y = x + (d_t * v); //add gravity + 0.5  * g * t * t
    Eigen::VectorXd b = (M * Y) + (delta_t * delta_t * D.transpose() * y) + (rho * delta_t * delta_t * D.transpose() * z);
    Eigen::VectorXd x_k_plus_1 = solver.solve(b);
    if (solver.info() != Eigen::Success)
    {
        std::cerr << "solving failed";
        return;
    }
    x = x_k_plus_1;
    Eigen::VectorXd zi(3);
    // std::cout << "Overall z\n"
    //           << z << std::endl;
    double scalar_zi, factor_zi;
    // std::cout << "Chunks of z" << std::endl;
    for (int i = 0; i < z.rows() / 3; i += 1)
    {
        zi = z.segment(3 * i, 3);
        // std::cout << zi << std::endl;
        // std::cout << D.block(3 * i, 0, 3, x.size());
        scalar_zi = (rho * D.block(3 * i, 0, 3, x.size()) * x).norm() + (l[i * 3] * k[i * 3]);
        scalar_zi = scalar_zi / (k[i] + rho);
        // //solve for zi
        factor_zi = (k[3 * i] * ((zi.norm() - l[i * 3]) / zi.norm())) + rho;
        zi = (rho * D.block(3 * i, 0, 3, x.size()) * x) - y.segment(3 * i, 3);
        zi = zi / factor_zi;
        z.segment(3 * i, 3) = zi;
    }
    // now we have updated x and z
    // update the dual variable
    y += (rho * ( z - (D * x)));
    // std::cout << " y: " << y;
    // std::cout << " z: " << z;
}
Eigen::VectorXd AdmmSolverEngine::get_x()
{
    return this->x;
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
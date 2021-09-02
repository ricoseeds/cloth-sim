#include "../includes/AdmmSolverEngine.h"
#include <omp.h>
void AdmmSolverEngine::run(double d_t)
{
    // std::cout << "HERE HERE HERE";
    Y = x + (d_t * v) + g * d_t * d_t;
    // std::cout << "Update UpdateUpdateUpdateUpdateUpdateUpdateUpdate ";
    // Y.segment(x.rows() - x_attached.rows(), x_attached.rows()) = x_attached;
    Eigen::VectorXd old_x = x;
    for (int i = 0; i < 20; i++)
    {
        admm_iter(d_t);
    }
    v = (x - old_x) * (1.0 / d_t);
    // v.segment(x.rows() - x_attached.rows(), x_attached.rows()) = Eigen::VectorXd::Zero(x_attached.rows());
    u.setZero(); // clear out dual variable
}
void AdmmSolverEngine::admm_iter(double d_t)
{
    // double w_i = 0.707107;
    // cout << z.rows();
    Eigen::VectorXd Dx = D * x;
#pragma omp parallel for
    for (int i = 0; i < z.rows() / 3; i++)
    {
        // std::cout << "RR RR RR";
        double w_i = W.coeff(3 * i, 3 * i);
        // std::cout << "EE EE EE";

        // Take the unit vector along D_i x + u_i and multiply it by(w_i ^ 2 || D_i x + u_i || +k l_i) / (w_i ^ 2 + k)
        Eigen::VectorXd Dix_plus_ui = Dx.segment(3 * i, 3) + u.segment(3 * i, 3);
        Eigen::VectorXd Dix_plus_ui_minus_x_star = Dix_plus_ui - x_star.segment(3 * i, 3);
        Eigen::VectorXd unit_vect = Dix_plus_ui_minus_x_star;
        unit_vect.normalize();
        // std::cout << "Dix" << Dix_plus_ui << std::endl
        //           << "unit " << unit_vect << "" << std::endl;
        double multiplier = w_i * w_i * Dix_plus_ui_minus_x_star.norm() + (l[i] * k[i]);
        multiplier = multiplier / (double)(k[i] + (w_i * w_i));
        z.segment(3 * i, 3) = multiplier * unit_vect;
        u.segment(3 * i, 3) += Dx.segment(3 * i, 3) - x_star.segment(3 * i, 3) - z.segment(3 * i, 3);
    }
    // update x
    Eigen::VectorXd b = (M * Y) + (delta_t * delta_t * D.transpose() * W.transpose() * W * (x_star + z - u));
    Eigen::VectorXd x_k_plus_1 = solver.solve(b);
    x = x_k_plus_1;
    // x.segment(x.rows() - x_attached.rows(), x_attached.rows()) = x_attached;
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
void AdmmSolverEngine::update_velocity(Eigen::VectorXd &vel)
{
    v = vel;
}
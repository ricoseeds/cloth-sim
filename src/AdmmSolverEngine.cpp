#include "../includes/AdmmSolverEngine.h"

void AdmmSolverEngine::run(double d_t)
{
    Y = x + (d_t * v) + g * d_t * d_t;
    Eigen::VectorXd old_x = x;
    for (int i = 0; i < 20; i++)
    {
        admm_iter(d_t);
    }
    v = (x - old_x) * (1.0 / d_t);
    u.setZero(); // clear out dual variable
}
void AdmmSolverEngine::admm_iter(double d_t)
{
    // double w_i = 0.707107;
    // cout << z.rows();
    for (int i = 0; i < z.rows() / 3; i++)
    {
        double w_i = W.coeff(3 * i, 3 * i);
        // Take the unit vector along D_i x + u_i and multiply it by(w_i ^ 2 || D_i x + u_i || +k l_i) / (w_i ^ 2 + k)
        Eigen::VectorXd Dix_plus_ui = (D.block(3 * i, 0, 3, x.size()) * x) + u.segment(3 * i, 3);
        Eigen::VectorXd unit_vect = Dix_plus_ui;
        unit_vect.normalize();
        // std::cout << "Dix" << Dix_plus_ui << std::endl
        //           << "unit " << unit_vect << "" << std::endl;
        double multiplier = w_i * w_i * Dix_plus_ui.norm() + (l[i] * k[i]);
        multiplier = multiplier / (double)(k[i] + (w_i * w_i));
        z.segment(3 * i, 3) = multiplier * unit_vect;
        u.segment(3 * i, 3) += (D.block(3 * i, 0, 3, x.size()) * x) - z.segment(3 * i, 3);
    }
    // update x
    Eigen::VectorXd b = (M * Y) + (delta_t * delta_t * D.transpose() * W.transpose() * W * (z - u));
    Eigen::VectorXd x_k_plus_1 = solver.solve(b);
    x = x_k_plus_1;
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
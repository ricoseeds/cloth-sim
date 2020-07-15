#include "../includes/AdmmSolverEngine.h"
#include <chrono>
void AdmmSolverEngine::run(double d_t)
{
    Y = x + (d_t * v) + g * d_t * d_t;
    Eigen::VectorXd old_x = x;
    for (int i = 0; i < 20; i++)
    {
        auto start_ = chrono::steady_clock::now();
        admm_iter(d_t, i);
        auto end_ = chrono::steady_clock::now();
        // cout << "Time elapsed in ADMM iteration <" << i << "> :" << chrono::duration_cast<chrono::nanoseconds>(end_ - start_).count() << endl;
    }
    v = (x - old_x) * (1.0 / d_t);
    u.setZero(); // clear out dual variable
    count_--;
    if (count_ == 0)
    {
        cout << "local step over 10 iterations =  " << local_acc_n_steps << endl;
        cout << "global step over 10 iterations =  " << global_acc_n_steps << endl;
        local_acc_n_steps = 0.0;
        global_acc_n_steps = 0.0;
        count_ = 10;
    }
}
void AdmmSolverEngine::admm_iter(double d_t, int itr)
{
    auto start = chrono::steady_clock::now();
    for (int i = 0; i < z.rows() / 3; i++)
    {
        double w_i = W.coeff(3 * i, 3 * i);
        // Take the unit vector along D_i x + u_i and multiply it by(w_i ^ 2 || D_i x + u_i || +k l_i) / (w_i ^ 2 + k)
        Eigen::VectorXd Dix_plus_ui = (D.block(3 * i, 0, 3, x.size()) * x) + u.segment(3 * i, 3);
        Eigen::VectorXd Dix_plus_ui_minus_x_star = Dix_plus_ui - x_star.segment(3 * i, 3);
        Eigen::VectorXd unit_vect = Dix_plus_ui_minus_x_star;
        unit_vect.normalize();
        // std::cout << "Dix" << Dix_plus_ui << std::endl
        //           << "unit " << unit_vect << "" << std::endl;
        double multiplier = w_i * w_i * Dix_plus_ui_minus_x_star.norm() + (l[i] * k[i]);
        multiplier = multiplier / (double)(k[i] + (w_i * w_i));
        z.segment(3 * i, 3) = multiplier * unit_vect;
        u.segment(3 * i, 3) += (D.block(3 * i, 0, 3, x.size()) * x) - x_star.segment(3 * i, 3) - z.segment(3 * i, 3);
    }
    auto end = chrono::steady_clock::now();
    local_acc += chrono::duration_cast<chrono::nanoseconds>(end - start).count() / 1000000.0;
    if (itr == 19)
    {
        cout << "local step : " << local_acc << endl;
        local_acc_n_steps += local_acc;
        local_acc = 0.0;
    }

    // cout << "Time elapsed in local step : " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() / 1000000.0 << endl;
    // update x
    auto start_ = chrono::steady_clock::now();
    Eigen::VectorXd b = (M * Y) + (delta_t * delta_t * D.transpose() * W.transpose() * W * (x_star + z - u));
    x = solver.solve(b);
    auto end_ = chrono::steady_clock::now();
    // cout << "Time elapsed in Global step : " << chrono::duration_cast<chrono::nanoseconds>(end_ - start_).count() / 1000000.0 << endl;
    global_acc += chrono::duration_cast<chrono::nanoseconds>(end_ - start_).count() / 1000000.0;
    if (itr == 19)
    {
        cout << "global step : " << global_acc << endl;
        global_acc_n_steps += global_acc;
        global_acc = 0.0;
    }
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
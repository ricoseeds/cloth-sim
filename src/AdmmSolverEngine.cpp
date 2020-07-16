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
        // cout << "local step over 10 time steps =  " << local_acc_n_steps << endl;
        // cout << "global step over 10 time steps =  " << global_acc_n_steps << endl;
        myfile << "local step over 10 time steps =  " << local_acc_n_steps << endl;
        myfile << "global step over 10 time steps =  " << global_acc_n_steps << endl;
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
        auto start_line_1 = chrono::steady_clock::now();
        double w_i = W.coeff(3 * i, 3 * i);
        auto end_line_1 = chrono::steady_clock::now();
        line_1 += chrono::duration_cast<chrono::nanoseconds>(end_line_1 - start_line_1).count() / 1000000.0;
        // Take the unit vector along D_i x + u_i and multiply it by(w_i ^ 2 || D_i x + u_i || +k l_i) / (w_i ^ 2 + k)
        //imp
        auto start_line_2 = chrono::steady_clock::now();
        // Eigen::VectorXd Dix = (D.block(3 * i, 0, 3, x.size()) * x);
        Dix_plus_ui_minus_x_star = Dix.segment(3 * i, 3) + u.segment(3 * i, 3) - x_star.segment(3 * i, 3);
        auto end_line_2 = chrono::steady_clock::now();
        line_2 += chrono::duration_cast<chrono::nanoseconds>(end_line_2 - start_line_2).count() / 1000000.0;
        // Dix_plus_ui_minus_x_star = (sparseBlock(D, 3 * i, 0, 3, x.size()) * x) + u.segment(3 * i, 3) - x_star.segment(3 * i, 3);
        // cout << (D.block(3 * i, 0, 3, x.size())) << endl;
        // cout << D;
        // break;
        // Eigen::VectorXd Dix_plus_ui_minus_x_star = Dix_plus_ui - x_star.segment(3 * i, 3);
        auto start_line_3 = chrono::steady_clock::now();
        double multiplier = w_i * w_i * Dix_plus_ui_minus_x_star.norm() + (l[i] * k[i]);
        auto end_line_3 = chrono::steady_clock::now();
        line_3 += chrono::duration_cast<chrono::nanoseconds>(end_line_3 - start_line_3).count() / 1000000.0;
        // Eigen::VectorXd unit_vect = Dix_plus_ui_minus_x_star;
        // unit_vect.normalize();
        auto start_line_4 = chrono::steady_clock::now();
        Dix_plus_ui_minus_x_star.normalize();
        auto end_line_4 = chrono::steady_clock::now();
        line_4 += chrono::duration_cast<chrono::nanoseconds>(end_line_4 - start_line_4).count() / 1000000.0;
        // std::cout << "Dix" << Dix_plus_ui << std::endl
        //           << "unit " << unit_vect << "" << std::endl;
        auto start_line_5 = chrono::steady_clock::now();
        multiplier = multiplier / (double)(k[i] + (w_i * w_i));
        auto end_line_5 = chrono::steady_clock::now();
        line_5 += chrono::duration_cast<chrono::nanoseconds>(end_line_5 - start_line_5).count() / 1000000.0;

        auto start_line_6 = chrono::steady_clock::now();
        z.segment(3 * i, 3) = multiplier * Dix_plus_ui_minus_x_star;
        auto end_line_6 = chrono::steady_clock::now();
        line_6 += chrono::duration_cast<chrono::nanoseconds>(end_line_6 - start_line_6).count() / 1000000.0;

        auto start_line_7 = chrono::steady_clock::now();
        u.segment(3 * i, 3) += Dix.segment(3 * i, 3) - x_star.segment(3 * i, 3) - z.segment(3 * i, 3);
        auto end_line_7 = chrono::steady_clock::now();
        line_7 += chrono::duration_cast<chrono::nanoseconds>(end_line_7 - start_line_7).count() / 1000000.0;
    }
    auto end = chrono::steady_clock::now();
    local_acc += chrono::duration_cast<chrono::nanoseconds>(end - start).count() / 1000000.0;
    if (itr == 19)
    {
        // cout << "local step : " << local_acc << endl;
        myfile << "local step : " << local_acc << endl;
        myfile << "L1 : " << line_1 << endl;
        myfile << "L2 : " << line_2 << endl;
        myfile << "L3 : " << line_3 << endl;
        myfile << "L4 : " << line_4 << endl;
        myfile << "L5 : " << line_5 << endl;
        myfile << "L6 : " << line_6 << endl;
        myfile << "L7 : " << line_7 << endl;
        local_acc_n_steps += local_acc;
        local_acc = 0.0;
        line_1 = 0.0;
        line_2 = 0.0;
        line_3 = 0.0;
        line_4 = 0.0;
        line_5 = 0.0;
        line_6 = 0.0;
        line_7 = 0.0;
    }

    // cout << "Time elapsed in local step : " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() / 1000000.0 << endl;
    // update x
    auto start_ = chrono::steady_clock::now();
    b = (M * Y) + (delta_t * delta_t * D.transpose() * W.transpose() * W * (x_star + z - u));
    x = solver.solve(b);
    auto end_ = chrono::steady_clock::now();
    // cout << "Time elapsed in Global step : " << chrono::duration_cast<chrono::nanoseconds>(end_ - start_).count() / 1000000.0 << endl;
    global_acc += chrono::duration_cast<chrono::nanoseconds>(end_ - start_).count() / 1000000.0;
    if (itr == 19)
    {
        // cout << "global step : " << global_acc << endl;
        myfile << "global step : " << global_acc << endl;
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
Eigen::SparseMatrix<double> AdmmSolverEngine::sparseBlock(Eigen::SparseMatrix<double, Eigen::ColMajor> M,
                                                          int ibegin, int jbegin, int icount, int jcount)
{
    //only for ColMajor Sparse Matrix
    assert(ibegin + icount <= M.rows());
    assert(jbegin + jcount <= M.cols());
    int Mj, Mi, i, j, currOuterIndex, nextOuterIndex;
    vector<Tri> tripletList;
    tripletList.reserve(M.nonZeros());

    for (j = 0; j < jcount; j++)
    {
        Mj = j + jbegin;
        currOuterIndex = M.outerIndexPtr()[Mj];
        nextOuterIndex = M.outerIndexPtr()[Mj + 1];

        for (int a = currOuterIndex; a < nextOuterIndex; a++)
        {
            Mi = M.innerIndexPtr()[a];

            if (Mi < ibegin)
                continue;
            if (Mi >= ibegin + icount)
                break;

            i = Mi - ibegin;
            tripletList.push_back(Tri(i, j, M.valuePtr()[a]));
        }
    }
    Eigen::SparseMatrix<double> matS(icount, jcount);
    matS.setFromTriplets(tripletList.begin(), tripletList.end());
    return matS;
}
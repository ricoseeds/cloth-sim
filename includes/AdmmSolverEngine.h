#ifndef ADMM_SOLVER_ENGINE_H
#define ADMM_SOLVER_ENGINE_H
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

typedef Eigen::Triplet<double> Tri;
class AdmmSolverEngine
{
private:
    double delta_t;
    Eigen::SparseMatrix<double> M, D, W;
    Eigen::SparseMatrix<double> A;
    Eigen::VectorXd z;      // dynamic lengths l(i, j)
    Eigen::VectorXd u;      // scaled lagrangian multipliers
    Eigen::VectorXd l;      // rest lengths
    Eigen::VectorXd k;      // spring consts
    Eigen::VectorXd x;      // pos
    Eigen::VectorXd x_star; // pos

    Eigen::VectorXd x_attached; // attached pos
    Eigen::VectorXd g;          // gravity
    Eigen::VectorXd v;          // vel
    Eigen::VectorXd Y;          // x + d_t * v
    Eigen::VectorXd
        b;
    Eigen::VectorXd
        Dix;
    Eigen::VectorXd Dix_plus_ui_minus_x_star;
    Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>>
        solver;
    // for profiling
    int count_;
    double local_acc, global_acc;
    double local_acc_n_steps, global_acc_n_steps;
    std::ofstream myfile;
    double line_1, line_2, line_3, line_4, line_5, line_6, line_7;
    // double w_i, multiplier;

public:
    AdmmSolverEngine(double d_t, Eigen::SparseMatrix<double> &M, Eigen::SparseMatrix<double> &W, Eigen::SparseMatrix<double> &D, Eigen::VectorXd &l, Eigen::VectorXd &k, Eigen::VectorXd &x, Eigen::VectorXd &x_star, Eigen::VectorXd &v, double gravity, Eigen::VectorXd &attached)
    {
        // w_i = 0;
        // multiplier = 0;
        line_1 = 0.0;
        line_2 = 0.0;
        line_3 = 0.0;
        line_4 = 0.0;
        line_5 = 0.0;
        line_6 = 0.0;
        line_7 = 0.0;
        count_ = 10; // profile over 10 admm time steps
        local_acc = 0.0;
        global_acc = 0.0;
        local_acc_n_steps = 0.0;
        global_acc_n_steps = 0.0;
        delta_t = d_t;
        this->x_attached = attached;
        this->x_star = x_star;
        this->M = M;
        this->D = D;
        this->W = W;
        this->A = M + ((delta_t * delta_t) * D.transpose() * W.transpose() * W * D); // pre- computed A for global step
        // std::cout << "Here A" << A;
        // std::cout << "Here A TTTTT" << A.transpose();
        solver.compute(A);
        // std::cout << "Here BBBBB" << A;
        if (solver.info() != Eigen::Success)
        {
            std::cerr << "decomposition failed";
            return;
        }

        this->l = l;
        this->k = k;
        this->x = x;
        this->v = v;
        z = D * x;
        Dix = D * x;
        // std::cout << "X size : " << x.size() << std::endl;
        this->u = Eigen::VectorXd::Zero(z.size());
        this->g = Eigen::VectorXd::Zero(x.size());
        this->b = Eigen::VectorXd::Zero(M.rows());
        Dix_plus_ui_minus_x_star = Eigen::VectorXd::Zero(3);
        ;
        for (int i = 0; i < x.rows(); i += 3)
        {
            g[i] = 0.0;
            g[i + 1] = gravity / 2;
            g[i + 2] = 0.0;
        }
        myfile.open("profile_opt_try5.log");
        myfile.precision(17);
        // cout << "Gravity : " << g << endl;
        // cout
        //     << "x " << endl
        //     << x << endl;
        // cout << "v " << endl
        //      << v << endl;
        // cout << "M " << endl
        //      << M << endl;
        // cout << "D " << endl
        //      << D << endl;
        // cout << "W " << endl
        //      << W << endl;
        // cout << "l " << endl
        //      << l << endl;
        // cout << "K " << endl
        //      << k << endl;
        // cout << "A " << endl
        //      << A << endl;
    }
    void global_step();
    void admm_iter(double d_t, int itr);
    void run(double d_t);
    void resize_M(int, int);
    void resize_D(int, int);
    void set_M(Eigen::SparseMatrix<double> &);
    Eigen::VectorXd get_x();
    void set_D_and_compute_z(Eigen::SparseMatrix<double> &, Eigen::VectorXd &);
    void update_velocity(Eigen::VectorXd &);
    Eigen::SparseMatrix<double> sparseBlock(Eigen::SparseMatrix<double, Eigen::ColMajor> M,
                                            int ibegin, int jbegin, int icount, int jcount);
};

#endif
#ifndef ADMM_SOLVER_ENGINE_H
#define ADMM_SOLVER_ENGINE_H
#include <Eigen/Dense>
#include <Eigen/SparseCholesky>
#include <vector>
#include <iostream>
class AdmmSolverEngine
{
private:
    double rho;
    double delta_t;
    Eigen::SparseMatrix<double> M, D;
    Eigen::SparseMatrix<double> A;
    Eigen::VectorXd z; // dynamic lengths l(i, j)
    Eigen::VectorXd y; // lagrangian multipliers
    Eigen::VectorXd l; // rest lengths
    Eigen::VectorXd k; // spring consts
    Eigen::VectorXd x; // spring consts
    Eigen::VectorXd v; // spring consts
    Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>>
        solver;

public:
    AdmmSolverEngine(double rho, double d_t, Eigen::SparseMatrix<double> &M, Eigen::SparseMatrix<double> &D, Eigen::VectorXd &l, Eigen::VectorXd &k, Eigen::VectorXd &x, Eigen::VectorXd &v)
    {
        this->rho = rho;
        delta_t = d_t;
        this->M = M;
        this->D = D;
        this->A = M + ((rho * delta_t * delta_t) * D.transpose() * D); // pre- computed A for global step
        solver.compute(A);
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
        std::cout << "X size : " << x.size() << std::endl;
        this->y = Eigen::VectorXd::Zero(z.size());
        std::cout << z.size();
        Eigen::VectorXd Y = x + (d_t * v); //add gravity + 0.5  * g * t * t
        Eigen::VectorXd b = (M * Y) + (delta_t * delta_t * D.transpose() * y) + (rho * delta_t * delta_t * D.transpose() * z);
        Eigen::VectorXd x_k_plus_1 = solver.solve(b);
        if (solver.info() != Eigen::Success)
        {
            std::cerr << "solving failed";
            return;
        }
        // std::cout << "X_k+1 size : " << x_k_plus_1 << std::endl;
        // std::cout << "z : " << z << std::endl;

        // std::cout << x_k_plus_1.size();
        // std::cout << D.transpose() * D + M;
    }
    void admm_iter(double d_t);
    void run(double d_t);
    void resize_M(int, int);
    void resize_D(int, int);
    void set_M(Eigen::SparseMatrix<double> &);
    Eigen::VectorXd get_x();
    void set_D_and_compute_z(Eigen::SparseMatrix<double> &, Eigen::VectorXd &);
};

#endif
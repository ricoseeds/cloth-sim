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
    Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;

public:
    AdmmSolverEngine(double rho, double d_t, Eigen::SparseMatrix<double> &M, Eigen::SparseMatrix<double> &D, Eigen::VectorXd &l, Eigen::VectorXd &k)
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
    }
    void admm_iter(Eigen::VectorXd &x, Eigen::VectorXd &v, double d_t);
    void resize_M(int, int);
    void resize_D(int, int);
    void set_M(Eigen::SparseMatrix<double> &);
    void set_D_and_compute_z(Eigen::SparseMatrix<double> &, Eigen::VectorXd &);
};

#endif
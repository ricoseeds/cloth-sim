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
    }
    void build_connections()
    {
        int n_squared = size * size;
        int n = size;
        for (int i = 0, nRows = connx.rows(), nCols = connx.cols(); i < nCols; ++i)
        {
            if (i + 1 < n_squared)
            {
                if ((i + 1) % n != 0)
                {
                    // Horizontal connections
                    connx(i, i + 1) = 1;
                    connx(i + 1, i) = 1;
                }
                if (i < n_squared - n)
                {
                    //Vertical connections
                    connx(i, i + n) = 1;
                    connx(i + n, i) = 1;
                }
                if (i < n_squared - n - 1 && (i + 1) % n != 0)
                {
                    //right diagonal
                    connx(i, i + n + 1) = 1;
                    connx(i + n + 1, i) = 1;
                    connx(i + 1, i + n) = 1;
                    connx(i + n, i + 1) = 1;
                }
                if ((i + 2) % n != 0 && (i + 1) % n != 0)
                {
                    // horizontal flex
                    connx(i, i + 2) = 1;
                    connx(i + 2, i) = 1;
                }
                if (i + (2 * n) < n_squared)
                {
                    // verical flex
                    connx(i, i + (2 * n)) = 1;
                    connx(i + (2 * n), i) = 1;
                }
            }
        }
    }
    MatrixXd get_mat()
    {
        return connx;
    }
};
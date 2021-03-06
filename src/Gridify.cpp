#include "../includes/Gridify.h"
#include <cmath>
Gridify::Gridify(int n)
{
    size = n;
    connx.resize(n * n, n * n);
    connx.setZero(n * n, n * n);
}

/*
    For building the connections
*/
void Gridify::build_connections()
{
    int n_squared = size * size;
    int n = size;
    // create DAG
    for (int i = 0, nRows = connx.rows(), nCols = connx.cols(); i < nCols; ++i)
    {
        if (i + 1 < n_squared)
        {
            if ((i + 1) % n != 0)
            {
                // Horizontal connections
                connx(i, i + 1) = 1;
                // connx(i + 1, i) = 1;
            }
            if (i < n_squared - n)
            {
                //Vertical connections
                connx(i, i + n) = 1;
                // connx(i + n, i) = 1;
            }
            if (i < n_squared - n - 1 && (i + 1) % n != 0)
            {
                // right diagonal
                connx(i, i + n + 1) = sqrt(2.0);
                connx(i + n + 1, i) = sqrt(2.0);
                //left diagonal
                connx(i + 1, i + n) = sqrt(2.0);
                connx(i + n, i + 1) = sqrt(2.0);
            }
            if ((i + 2) % n != 0 && (i + 1) % n != 0)
            {
                // horizontal flex
                // connx(i, i + 2) = 2;
                // connx(i + 2, i) = 2;
            }
            if (i + (2 * n) < n_squared)
            {
                // verical flex
                // connx(i, i + (2 * n)) = 2;
                // connx(i + (2 * n), i) = 2;
            }
        }
    }
}

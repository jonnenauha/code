/* main.cpp -- main module
 *
 *			Ryan McDougall -- 20060101
 */

#include <iostream>
#include <cassert>
#include <cstdlib>
#include "util.hpp"
#include "linoperator.hpp"
#include "matrix.hpp"

using namespace ThreeDInc;
using namespace std;


int res = 0;
size_t num = 0;

typedef int Type;
typedef Type MyMatrix[16];

//=============================================================================
// Main entry point
int
main (int argc, char** argv)
{
    MyMatrix M1  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    MyMatrix Mres  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    print (M1, M1+16, cout);

    transpose (make_matrix_transpose_view <MyMatrix> (M1), make_matrix_view <MyMatrix> (Mres));
    print (Mres, Mres+16, cout);

    return 0;
}

/*!
 * \file vx_matrix_access.c
 */

#include <VX/vx.h>
#include <stdlib.h>

vx_matrix example_random_matrix(vx_context context)
{
    const vx_size columns = 3;
    const vx_size rows = 4;
    vx_matrix m = vxCreateMatrix(context, VX_TYPE_FLOAT32, columns, rows);
    if (m)
    {
        vx_int32 j, i;
        vx_float32 *mat = (vx_float32 *)malloc(rows*columns*sizeof(vx_float32));
        if (mat)
        {
            vxAccessMatrix(m, mat);
            for (j = 0; j < rows; j++)
            {
                for (i = 0; i < columns; i++)
                {
                    // assign random values <= 1.0
                    mat[j*columns + i] = (vx_float32)rand()/RAND_MAX;
                }
            }
            vxCommitMatrix(m, mat);
            free(mat);
        }
    }
    return m;
}

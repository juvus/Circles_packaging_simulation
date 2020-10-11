/*================================================================================*/
/* Realization of misc functions                                                  */
/*================================================================================*/

#define _USE_MATH_DEFINES

/* Standard incudes: */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/* Program includes: */
#include <utils.h>

f32
trim(f32 value, f32 low_bnd, f32 hight_bnd)
{
    /* Trim the value to the low and hight bound */
    if (value < low_bnd) {
        return low_bnd;
    }
    if (value > hight_bnd) {
        return hight_bnd;
    }
    return value;
}

u32
rnd_int_range(u32 left_bnd, u32 right_bnd)
{
    /* Function for generation random u32 number from the specific range */
    return (left_bnd + (rand() % (right_bnd - left_bnd)));
}

f32
rnd_f32(void)
{
    /* Function to generate the random number in range [0.0, 1.0) */
    f32 result;
    result = (f32)((rand() % 32767) / (f32)32767);
    return result;
}

f32
rnd_f32_range(f32 left_bnd, f32 right_bnd)
{
    /* Function for generation random f32 number from specific range */
    f32 result;
    result = left_bnd + rnd_f32() * (right_bnd - left_bnd);
    return result;
}

void
print_error_and_exit(char *text)
{
    /* Function for printing the error end exiting the program */

    printf("ERROR: %s", text);
    exit(1);
}

f32*
dynamic_1d_f32_array_alloc(u32 N)
{
    /* Function for dynamic allocation of 1d array of f32 elements
       N - number of elements */

    f32 *array = (f32 *) malloc (N * sizeof(f32));
    if (NULL == array) print_error_and_exit("Error in memory allocation!\n");
    return array;   
}

f32**
dynamic_2d_f32_array_alloc(u32 N, u32 M)
{
    /* Function for dynamic allocation of 2d arrays of f32 elements 
       N - number of rows
       M - number of columns 
       Return:
       array - name of the array (pointer to pointer to f32) */

    u32 i;
    f32 **array = (f32 **) malloc (N * sizeof(f32 *));
    if (NULL == array) print_error_and_exit("Error in memory allocation!\n");
    for (i = 0; i < N; ++i) {
        array[i] = (f32 *) malloc (M * sizeof(f32));
        if (NULL == array[i]) print_error_and_exit("Error in memory allocation!\n");
    }
    return array;    
}

void
dynamic_2d_f32_array_free(f32 **array, u32 N)
{
    /* Function for memory free of the 2d array
       array - name of the array (pointer to pointer to double)
       N - number of rows */

    u32 i;
    for (i = 0; i < N; ++i) {
        free(array[i]);
    }
    free(array);	
}

b32
rectangles_are_collide(V2_u32_t BL_1, V2_u32_t UR_1, V2_u32_t BL_2, V2_u32_t UR_2)
{
    /* Function for check the collision of two rectangles */
    /* Chech the verticals for the collision */
    if ((BL_2.x > UR_1.x) || (UR_2.x < BL_1.x)) {
        return false;
    }

    /* Check the horizontals for the collision */
    if ((BL_2.y > UR_1.y) || (UR_2.y < BL_1.y)) {
        return false;
    }
    return true;
}

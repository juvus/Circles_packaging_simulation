/*================================================================================*/
/* Header file for the misc functions                                             */
/*================================================================================*/
#ifndef MISC_H_
#define MISC_H_

#include <utils.h>
#include <stdlib.h>
#include <file_io.h>

/* Trim the value to the low and hight bound */
f32 trim(f32 value, f32 low_bnd, f32 hight_bnd);

/* Function for generation random u32 number from the specific range */
u32 rnd_int_range(u32 left_bnd, u32 right_bnd);

/* Function to generate the random number in range [0.0, 1.0) */
f32 rnd_f32(void);

/* Function for generation random f32 number from specific range */
f32 rnd_f32_range(f32 left_bnd, f32 right_bnd);

/* Function for printing the error end exiting the program */
void print_error_and_exit(char *text);

/* Function for dynamic allocation of 1d array of f32 elements */
f32* dynamic_1d_f32_array_alloc(u32 N);

/* Function for dynamic allocation of 2d arrays of f32 elements */
f32** dynamic_2d_f32_array_alloc(u32 N, u32 M);

/* Function for memory free of the 2d array */
void dynamic_2d_f32_array_free(f32 **array, u32 N);

/* Function for check the collision of two rectangles */
b32 rectangles_are_collide(V2_u32_t BL_1, V2_u32_t UR_1, V2_u32_t BL_2, V2_u32_t UR_2);

#endif //MISC_H_

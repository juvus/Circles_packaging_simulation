/*================================================================================*/
/* Header file for the world class.                                               */
/*================================================================================*/
#ifndef PSO_ALG_H_
#define PSO_ALG_H_

#include <utils.h>
#include <circle.h>
#include <circles_list.h>
#include <box.h>

/* Structure of the PSO algorithm */
struct PSO {
    /* Particle swarm optimization algorithm parameters */
    u32 n_var; /* Number of searching variables */
    f32 var_min; /* Minimum value of the variables (0.0...1.0) */
    f32 var_max; /* Maximum value of the variables (0.0...1.0) */
    b32 use_iter_limit; /* Flag whether to use iterations limit or no */
    u32 iter_limit; /* Limit of iterations */
    u32 n_pop; /* Population number */
    f32 w_init; /* Initial inertia coefficient */
    f32 w_damp; /* Damping ratio of the inertia coefficient */
    f32 c1; /* Personal acceleration coefficient */
    f32 c2; /* Social acceleration coefficient */
    u32 a; /* Additional particles randomization */
    u32 b; /* Additional swarm randomization */   

    u32 iteration; /* Total number of iterations */
    b32 is_solution_found; /* Flag to tetermine the result of PSO search */
    f32 **part_positions; /* 2D array for positions of all particles */
    f32 *tmp_position; /* Array for temporary position of one particle */
    f32 **part_best_positions; /* 2D array for best positions of all particles */
    f32 **part_velocities; /* 2D array for particles velocities */
    f32 *part_costs; /* Array for particles costs */
    f32 *part_best_costs; /* Array for best particles costs */
    f32 global_best_cost; /* Global closest distance to the bottom of the box */
    f32 *global_best_position; /* Best position is a vector of 2 float numbers */
    f32 w; /* Current inertia coefficient */
    f32 r1; /* Random numbers r1 */
    f32 r2; /* Random numbers r2 */   
};
typedef struct PSO PSO_t;

/* Structure of the PSO search result (return from the search) */
struct PSO_result {
    b32 is_solution_found; /* Flag to tetermine the result of PSO search */
    V2_u32_t global_best_coordinates; /* Best coordinates is a vector of 2 unsigned numbers */
};
typedef struct PSO_result PSO_result_t;

/* Methods of PSO class */
/* Method for creation of the pso algorithm object (malloc) */
PSO_t* pso_create(const u32 n_var, const u32 n_pop);

/* Method for deleting the pso algorithm object */
void pso_delete(PSO_t *pso);

/* Initialization of the PSO algorithm */
void pso_init(PSO_t *pso, const f32 var_min, const f32 var_max, const b32 use_iter_limit,
              const u32 iter_limit, const f32 w_init, const f32 w_damp, const f32 c1, const f32 c2,
              const u32 a, const u32 b);

/* Method for deleting the PSO algorithm object */
void pso_delete(PSO_t *pso);

/* Method to start the search of circle coordinates and return the results */
PSO_result_t pso_run_search(PSO_t *pso, const Box_t *box, const u32 radius); 

/* Method to calculate cost function for the current circle */
f32 pso_calculate_cost(PSO_t *pso, const Box_t *box, const u32 radius);

/* Method to calculate the coordinates from the found best solution */
V2_u32_t pso_calc_coordinates(PSO_t *pso, const Box_t *box, const u32 radius);

#endif /* PSO_ALG_H_ */

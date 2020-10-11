/*========================================================================*/ 
/* Realization of the pso_algorithm mathods                               */
/*========================================================================*/

/* Standard incudes: */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* Program includes: */
#include <pso_algorithm.h>
#include <utils.h>
#include <circle.h>
#include <circles_list.h>
#include <box.h>
#include <misc.h>

/* Defined large constant */
static const f32 large_f32 = 5000.0f;

PSO_t*
pso_create(const u32 n_var, const u32 n_pop)
{
    /* Method for creation of the pso algorithm object (malloc) */
    PSO_t *tmp_pso;
      
    /* Allocate the memory for the pso algorithm object */
    tmp_pso = (PSO_t *) calloc (1, sizeof(PSO_t));
    if (NULL == tmp_pso) print_error_and_exit("Error in memory allocation!\n");
    
    tmp_pso->n_var = n_var; /* Number of unknown (decision) variables */ 
    tmp_pso->n_pop = n_pop; /* Population size (swarm size) */
    
    /* Allocation memory for the additional fields */
    tmp_pso->part_positions = dynamic_2d_f32_array_alloc(n_pop, n_var);
    tmp_pso->tmp_position = dynamic_1d_f32_array_alloc(n_var);
    tmp_pso->part_best_positions = dynamic_2d_f32_array_alloc(n_pop, n_var);
    tmp_pso->part_velocities = dynamic_2d_f32_array_alloc(n_pop, n_var);
    tmp_pso->part_costs = dynamic_1d_f32_array_alloc(n_pop);
    tmp_pso->part_best_costs = dynamic_1d_f32_array_alloc(n_pop);
    tmp_pso->global_best_position = dynamic_1d_f32_array_alloc(n_var);
    
    return tmp_pso;
}

void
pso_delete(PSO_t *pso)
{
    /* Method for deleting the pso algorithm object */
    /* Delete dynamically allocated fields of the pso object */
    dynamic_2d_f32_array_free(pso->part_positions, pso->n_pop);
    free(pso->tmp_position);
    dynamic_2d_f32_array_free(pso->part_best_positions, pso->n_pop);
    dynamic_2d_f32_array_free(pso->part_velocities, pso->n_pop);
    free(pso->part_costs);
    free(pso->part_best_costs);
    free(pso->global_best_position);

    free(pso);    
}

void pso_init(PSO_t *pso, const f32 var_min, const f32 var_max, const b32 use_iter_limit,
              const u32 iter_limit, const f32 w_init, const f32 w_damp, const f32 c1, const f32 c2,
              const u32 a, const u32 b)
{
    /* Initialization of the PSO algorithm */

    /* Set the pso fields */
    pso->var_min = var_min; /* Lower bound of decision variables */
    pso->var_max = var_max; /* Upper bound of decision variables */
    pso->use_iter_limit = use_iter_limit; /* (bool) using of the iteration limit */
    pso->iter_limit = iter_limit; /* Search iteration limit */ 
    pso->w_init = w_init; /* Inertia coefficient */
    pso->w_damp = w_damp; /* Damping ratio of inertia coefficient */
    pso->c1 = c1; /* Personal acceleration coefficient */
    pso->c2 = c2; /* Social acceleration coefficient */
    pso->a = a; /* Additional randomization of a-th particle in swarm */
    pso->b = b; /* Additional randomization of all particles every b-th iteration */
}

PSO_result_t
pso_run_search(PSO_t *pso, const Box_t *box, const u32 radius)
{
    /* Method to start the search of circle coordinates and return the results. Here the radius is the
       radius of the tested circle */

    u32 i, j;
    PSO_result_t result;
    b32 do_search;

    /* ===== 1. INITIALIZATION OF THE PSO ALGORITHM ===== */
    pso->iteration = 1;
    pso->w = pso->w_init;
    pso->is_solution_found = false;
    do_search = true;
    
    /* Initialize the best costs with very high values */
    pso->global_best_cost = large_f32;
    for (i = 0; i < pso->n_pop; ++i) {
        pso->part_best_costs[i] = large_f32;
    }
    
    /* Randomize the position of the particles */
    for (i = 0; i < pso->n_pop; ++i) {
        for (j = 0; j < pso->n_var; ++j) {
            pso->part_positions[i][j] = pso->var_min + (pso->var_max - pso->var_min) * rnd_f32();
        }
    }

    /* Initialize the particle velocities with zeros */
    for (i = 0; i < pso->n_pop; ++i) {
        for (j = 0; j < pso->n_var; ++j) {
            pso->part_velocities[i][j] = 0.0f;
        }
    }
    
    /* Update the costs for every particle in the population */
    for (i = 0; i < pso->n_pop; ++i) {
        
        /* Copy the current particle position to time position */
        for (j = 0; j < pso->n_var; ++j) {
            pso->tmp_position[j] = pso->part_positions[i][j];
        }

        /* Update the current particle cost */
        pso->part_costs[i] = pso_calculate_cost(pso, box, radius);
        
        /* Update the particle best cost and position so far */
        if (pso->part_costs[i] < pso->part_best_costs[i]) {
            pso->part_best_costs[i] = pso->part_costs[i];
            for (j = 0; j < pso->n_var; ++j) {
                pso->part_best_positions[i][j] = pso->tmp_position[j];
            }
        }

        /* Update the global cost and global best position */
        if (pso->part_costs[i] < pso->global_best_cost) {
            pso->global_best_cost = pso->part_costs[i];
            for (j = 0; j < pso->n_var; ++j) {
                pso->global_best_position[j] = pso->tmp_position[j];
            }
        }
    }
    
    /* ===== 2. SEARCHING LOOP OF THE PSO ALGORITHM ===== */
    while(do_search) {
        for (i = 0; i < pso->n_pop; ++i) {
            for (j = 0; j < pso->n_var; ++j) {
                
                /* Randomize r1 and r2 parameters (0 - 1)*/
                pso->r1 = rnd_f32();
                pso->r2 = rnd_f32();

                /* Update the velocity */
                pso->part_velocities[i][j] = pso->w * pso->part_velocities[i][j] + 
                    pso->r1 * pso->c1 * (pso->part_best_positions[i][j] - pso->part_positions[i][j]) + 
                    pso->r2 * pso->c2 * (pso->global_best_position[j] - pso->part_positions[i][j]);

                /* Update the particle position */
                pso->part_positions[i][j] += pso->part_velocities[i][j];

                /* Making restrictions to the particles positions. Components of the position vector
                   should be in range 0.0 - 1.0. Reset the particle position to their best position. */
                if (pso->part_positions[i][j] > 1.0f) {
                    //pso->part_positions[i][j] = pso->var_min + (pso->var_max - pso->var_min) * rnd_f32();
                    pso->part_positions[i][j] = pso->part_best_positions[i][j];
                    pso->part_velocities[i][j] = 0.0f;
                }
                if (pso->part_positions[i][j] < 0.0f) {
                    //pso->part_positions[i][j] = pso->var_min + (pso->var_max - pso->var_min) * rnd_f32();
                    pso->part_positions[i][j] = pso->part_best_positions[i][j];
                    pso->part_velocities[i][j] = 0.0f;
                }              
            } /* for j */
			
            /* Additional randomization: every A-th particle should be randomized */
            if ((i % pso->a) == 0) {
                for (j = 0; j < pso->n_var; ++j) {
                    pso->part_positions[i][j] = pso->var_min + (pso->var_max - pso->var_min) * rnd_f32();
                }
            }
            
            /* Additional randomization: reset of particles every B-th iteration */
            if ((pso->iteration % pso->b) == 0) {
                for (j = 0; j < pso->n_var; ++j) {
                    pso->part_positions[i][j] = pso->var_min + (pso->var_max - pso->var_min) * rnd_f32();
                }
            }

            /* Copy the current particle position to time position */
            for (j = 0; j < pso->n_var; ++j) {
                pso->tmp_position[j] = pso->part_positions[i][j];
            }

            /* Update the current particle cost */
            pso->part_costs[i] = pso_calculate_cost(pso, box, radius);

            /* if there was in interception, then fuction return large_f32 -> change the position
               of the particle */
            if (pso->part_costs[i] >= large_f32) {
                for (j = 0; j < pso->n_var; ++j) {
                    pso->part_positions[i][j] = pso->part_best_positions[i][j];
                    pso->part_velocities[i][j] = 0.0f;
                }
            }

            /* Update the particle best cost and position so far */
            if (pso->part_costs[i] < pso->part_best_costs[i]) {
                pso->part_best_costs[i] = pso->part_costs[i];
                for (j = 0; j < pso->n_var; ++j) {
                    pso->part_best_positions[i][j] = pso->tmp_position[j];
                }
            }

            /* Update the global cost and global best position */
            if (pso->part_costs[i] < pso->global_best_cost) {
                pso->global_best_cost = pso->part_costs[i];
                for (j = 0; j < pso->n_var; ++j) {
                    pso->global_best_position[j] = pso->tmp_position[j];
                }
            }	
        } /* for i */
		
        /* Reduce the inertia coefficient */
        pso->w = pso->w * pso->w_damp;

        /* Reset the inertia coefficient every B-th iteration  */
        if ((pso->iteration % pso->b) == 0) {
            pso->w = pso->w_init;
        }

        /* Check the search termination by iteration limit */
        if ((pso->use_iter_limit) && (pso->iteration >= pso->iter_limit)) {
            do_search = false;

            /* Prepare and return the result structure */
            result.is_solution_found = pso->is_solution_found;
            result.global_best_coordinates = pso_calc_coordinates(pso, box, radius);
        }
        else {
            pso->iteration += 1;
        }                
    }

    return result;
}

f32
pso_calculate_cost(PSO_t *pso, const Box_t *box, const u32 radius)
{
    /* Method to calculate cost function for the current circle, taking into account the 
       box blocks containing the information about already packed circles */

    u32 i, j, n;
    u32 x0, y0; /* Coordinates (in pix) of the center of the tested circle */
    u32 x1, y1, r1; /* Coordinates (in pix) of the center of the already packed circle and radius*/
    u32 left_border, right_border, top_border, bottom_border;
    V2_u32_t block_BL, block_UR; /* BL and UR points of outside rectangle of the block */
    V2_u32_t circ_BL, circ_UR; /* BL and UR points of outside rectangle of the circle */ 
    f32 distance; /* Distance between the circles centers (in pix) */
    Circles_list_t* circles_list; /* Circles list from where we have to check the circles */
    Circles_list_elem_t *list_elem; /* Element of the circles list */
        
    /* 1. Determination of the center coordinates of circle to be tested (0-1 -> pixels) */
    /* Determination of boarders of the coordinates change */
    left_border = box->BL_in.x + radius;
    right_border = box->BR_in.x - radius;
    top_border = box->UL_in.y - radius;
    bottom_border = box->BL_in.y + radius;

    /* Calculation of the center coordinates */
    x0 = (u32)roundf(pso->tmp_position[0] * (right_border - left_border) + left_border);
    y0 = (u32)roundf(pso->tmp_position[1] * (top_border - bottom_border) + bottom_border);

    /* 2. Determination of the outer rectangle of the tested circle */
    circ_BL.x = x0 - radius;
    circ_BL.y = y0 - radius;
    circ_UR.x = x0 + radius;
    circ_UR.y = y0 + radius;

    /* 3. Test for interception with packed circles from the necessary box blocks */
    for (i = 0; i < box->blocks_side_num; ++i) {
        for (j = 0; j < box->blocks_side_num; ++j) {

            /* Determine the parameters of the outside rectangle of the current block */
            block_BL.x = box->blocks[i][j].BL.x;
            block_BL.y = box->blocks[i][j].BL.y;
            block_UR.x = box->blocks[i][j].UR.x;
            block_UR.y = box->blocks[i][j].UR.y;

            /* Check the collision of the outside rectangles (if not, check next block) */
            if (!rectangles_are_collide(circ_BL, circ_UR, block_BL, block_UR)) {
                continue;
            }

            /* Copy the pointer to the block circles list to the memporary variable */
            circles_list = box->blocks[i][j].circles_list;

            /* Check the circles from the block circles list */
            if (circles_list->size > 0) {
                list_elem = circles_list->head;
        
                for (n = 0; n < circles_list->size; ++n) {
                    /* Coordinates of the circle found in the list  */
                    x1 = list_elem->circle->center.x;
                    y1 = list_elem->circle->center.y;
                    r1 = list_elem->circle->radius;
            
                    /* Calculate distance between the centers of the circles */
                    distance = sqrtf(powf(((f32)x0 - (f32)x1), 2) + powf(((f32)y0 - (f32)y1), 2));

                    if (distance <= ((f32)radius + (f32)r1)) {
                        /* Interception is detected, returning INFINITY */
                        return large_f32;
                    }
                    /* Check the next circle for the collision */
                    list_elem = list_elem->next;
                }
            }
        } /* for j */
    } /* for i */

    /* 4. If interception check has passed then calculate the cost value distance from the circle 
          center to the middle bottom point*/
    pso->is_solution_found = true;
    x1 = (box->BL_in.x + box->BR_in.x) / 2;
    y1 = box->BL_in.y;

    /* Distance to the middle bottom point of the box */
    distance = sqrtf(powf(((f32)x0 - (f32)x1), 2) + powf(((f32)y0 - (f32)y1), 2));

    /* And distance to the bottom of the box */
    distance += (f32)y0;

    assert(distance > 0.0f);
    return distance;
}  

V2_u32_t
pso_calc_coordinates(PSO_t *pso, const Box_t *box, const u32 radius)
{
    /* Method to calculate the coordinates from the found best solution */

    u32 x, y;
    u32 left_border, right_border, top_border, bottom_border;
    V2_u32_t result;

    left_border = box->BL_in.x + radius;
    right_border = box->BR_in.x - radius;
    top_border = box->UL_in.y - radius;
    bottom_border = box->BL_in.y + radius;

    //printf("%f %f\n", pso->global_best_position[0],
    //       pso->global_best_position[1]);  
    
    x = (u32)roundf(pso->global_best_position[0] * (right_border - left_border) + left_border);
    y = (u32)roundf(pso->global_best_position[1] * (top_border - bottom_border) + bottom_border);

    result.x = x;
    result.y = y;

    return result;   
}

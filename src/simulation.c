/*=============================================================================
  Simulation of the circles packaging procedure                                   
  =============================================================================*/

/* Additional define for visual studio */
#define _USE_MATH_DEFINES

/* Standard incudes: */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

/* Program includes: */
#include <simulation.h>
#include <software_rendering.h>
#include <utils.h>
#include <file_io.h>
#include <font.h>

/* Simulation classes */
#include <misc.h>
#include <world.h>
#include <box.h>
#include <text_panel.h>
#include <circle.h>
#include <circles_list.h>
#include <pso_algorithm.h>

/* Loaded images */
static Loaded_img_t font_img; /* Image with font symbols */

/* Define different simulation constants */
/* Circles parameters as hard-coded circles parameters */
#define CIRCLES_TYPES 5 /* Number of different types of circles */
static const u32 CIRCLES_COL[CIRCLES_TYPES] = {0x6daf43, 0x4870c8, 0xea8036, 0xaa1036, 0xaaff36}; /* Circles colors */
static const f32 CIRCLES_PROB[CIRCLES_TYPES] = {0.20f, 0.20f, 0.20f, 0.20f, 0.20f}; /* Probabilities of appearence */ 
static const u32 CIRCLES_RADIUSES[CIRCLES_TYPES] = {3, 5, 3, 5, 3}; /* Circles radiuses in pixels */

/* World constants */
static const u32 WORLD_BKG_COLOR = 0xe0e0e0; /* Background color */
static const f32 WORLD_SIM_DELAY = 0.001f; /* Delay in seconds */

/* Box constants */
static const u32 BOX_WIDTH_OUT = 366;
static const u32 BOX_HEIGHT_OUT = 500;
static const u32 BOX_WALL_WIDTH = 15;
static const u32 BOX_WALL_CLR = 0x7a623e;
static const u32 BOX_INTERIOR_CLR = 0x443723;
static const u32 BOX_BLOCKS_SIDE_NUM = 10;

/* Font constants */
static const u32 SYM_ROWS = 6; /* Amount of symbol rows in font.png */
static const u32 SYM_COLS = 20; /* Amount of sumbol columns in font.png */

/* Text panel constants */
static const u32 TEXT_PANEL_WIDTH = 366;
static const u32 TEXT_PANEL_HEIGHT = 150;
static const u32 TEXT_PANEL_BKG_CLR = 0xffffff;
static const u32 TEXT_PANEL_SHADOW_CLR = 0xcccccc;
static const u32 TEXT_PANEL_TEXT_CLR = 0x000000;

/* PSO algorithm parameters */
static const u32 PSO_N_VAR = 2;
static const f32 PSO_VAR_MIN = 0.0f;
static const f32 PSO_VAR_MAX = 1.0f;
static const b32 PSO_USE_ITER_LIMIT = true;
static const u32 PSO_ITER_LIMIT = 1000;
static const u32 PSO_N_POP = 10;
static const f32 PSO_W_INIT = 1.0;
static const f32 PSO_W_DAMP = 0.99;
static const f32 PSO_C1 = 2.0;
static const f32 PSO_C2 = 2.0;
static const u32 PSO_A = 5;
static const u32 PSO_B = 200;

/* Define different object and variables necessary for the simulation */
static World_t *world; /* World */
static Box_t *box; /* Box where circles are being packed */
static Text_panel_t* text_panel; /* Text panel with simulation information */
static PSO_t *pso; /* PSO algorithm for searching the circle place in box */
static PSO_result_t pso_result; /* Result from the PSO search */
static f32 sim_delay_time; /* Actual time that passed since the last position search */
static Circle_t *tmp_circle; /* Temporary pointer to circle object */
static V2_u32_t tmp_center; /* Temporary center point */
static u32 simulation_state = 1; /* Current simulation state */
static u32 tmp_color; /* Temporary color of the circle */
static u32 tmp_radius; /* Temporary radius of the circle */
static Symbol_data_t *font_symbols; /* Pointer to the array of all font symbols */

void
simulate_packaging(Input_t *user_input, f32 dtime, Render_Buffer_t *render_buffer)
{
    /* Main function for the simulation of circles packaging */

    /* Set the constant value for dtime for testing purposes */
    dtime = 0.01;
    
    switch (simulation_state) {
    case 1: {    
        /*===========================================================================================
          Loading the different resource data IMAGES                                              
          ===========================================================================================*/      

        /* Seed the random numbers */
        srand(time(NULL));
        
        /* Symbols of the font */
        font_img.raw_data = read_file_to_memory("data\\font.png");
        uncompress_png_image(&font_img);       
        
        /* Jump to the next simulation stage */
        simulation_state = 2;
        break;
    }
    case 2: {
        /*===========================================================================================
          Dynamic allocation of different objects                                                   
          ===========================================================================================*/

        font_symbols = (Symbol_data_t *) calloc ((u64)SYM_ROWS * (u64)SYM_COLS, sizeof(Symbol_data_t));
        font_extract_symbols(font_symbols, &font_img);

        world = world_create();
        box = box_create(BOX_BLOCKS_SIDE_NUM);       
        text_panel = text_panel_create();
        pso = pso_create(PSO_N_VAR, PSO_N_POP);

        /* Jump to the next simulation stage */
        simulation_state = 3;
        break;
    }
    case 3: {
        /*===========================================================================================
          Simulation initialization and reset procedure
          ===========================================================================================*/
              
        /* Initialization of the world object */
        world_init(world, render_buffer, WORLD_BKG_COLOR, SIM_STOPPED, WORLD_SIM_DELAY);

        /* Initialization of the box object */
        box_init(box, world, BOX_WIDTH_OUT, BOX_HEIGHT_OUT, BOX_WALL_WIDTH, TEXT_PANEL_HEIGHT, BOX_WALL_CLR,
                 BOX_INTERIOR_CLR);

        /* Initialization of the text_panel object */
        text_panel_init(text_panel, world, box, TEXT_PANEL_WIDTH, TEXT_PANEL_HEIGHT, TEXT_PANEL_BKG_CLR,
                        TEXT_PANEL_SHADOW_CLR, TEXT_PANEL_TEXT_CLR);
       
        /* Initialization of the pso object */
        pso_init(pso, PSO_VAR_MIN, PSO_VAR_MAX, PSO_USE_ITER_LIMIT, PSO_ITER_LIMIT, PSO_W_INIT, PSO_W_DAMP,
                 PSO_C1, PSO_C2, PSO_A, PSO_B);

        /* Reset the structure with pso results */
        pso_result.is_solution_found = false;
        pso_result.global_best_coordinates.x = 0;
        pso_result.global_best_coordinates.y = 0;

        /* Reset misc parameters */
        tmp_color = 0x000000;
        tmp_radius = 0;            

        /* Jump to the next simulation stage */
	simulation_state = 4;
        break;
    }
    case 4: {
        /*===========================================================================================
          Initial render of the objects                                                              
          ===========================================================================================*/
        
        world_render(world, render_buffer);
        box_render(box, render_buffer);
        text_panel_render(text_panel, font_symbols, render_buffer);

        /* Jump to the next simulation stage */
        simulation_state = 5;
        break;
    }
    case 5: {
        /*===========================================================================================
          Simulation run logic procedure                                                             
          ===========================================================================================*/
 
        /* Treat the buttons 1 press */
        if (pressed_iterative(BUTTON_1)) {
            world->sim_mode = SIM_NORMAL;
            text_panel_set_sim_status_value(text_panel, SIM_NORMAL);
            text_panel_render(text_panel, font_symbols, render_buffer);
            sim_delay_time = 0.0f;
        }

        /* Treat the buttons 2 press */
        if (pressed_iterative(BUTTON_2)) {
            world->sim_mode = SIM_PAUSED;
            text_panel_set_sim_status_value(text_panel, SIM_PAUSED);
            text_panel_render(text_panel, font_symbols, render_buffer);
        }    
        
        /* Search for new circles possition only if SIM_NORMAL mode is activated */
        if (world->sim_mode == SIM_NORMAL) {
            if (sim_delay_time >= world->sim_delay) {
               
                /* Generate the parameters of a new circle */
                circle_generate_params(CIRCLES_TYPES, CIRCLES_PROB, CIRCLES_COL, CIRCLES_RADIUSES,
                                       &tmp_color, &tmp_radius);
                
                /* Create new circle object with known and initial params */
                tmp_center.x = 0;
                tmp_center.y = 0;
                tmp_circle = circle_create(tmp_center, tmp_radius, tmp_color);                           
                
                /* Try to find the optimal position of the circle */
                pso_result = pso_run_search(pso, box, tmp_radius);
               
                /* Check the pso search results */
                if (pso_result.is_solution_found) {

                    /* Set the position of the circle */
                    tmp_center = pso_result.global_best_coordinates;                   
                    
                    circle_set_position(tmp_circle, tmp_center);

                    /* Add circle to the appropriate box blocks */
                    box_add_circle(box, tmp_circle);                    

                    /* Render the circle */
                    circle_render(tmp_circle, render_buffer);

                    /* Render the modified text pannel (with new circles count) */               
                    text_panel_set_packed_circles_value(text_panel, box->packed_circles_num);
                    text_panel_render(text_panel, font_symbols, render_buffer);
                }
                else {

                    /* Stop the simulation and move to final simulation stage */
                    world->sim_mode = SIM_STOPPED;
                    text_panel_set_sim_status_value(text_panel, SIM_STOPPED);
                    text_panel_render(text_panel, font_symbols, render_buffer);
                    simulation_state = 6;
                }

                /* Reset the delay time */
                sim_delay_time = 0.0;
            }
            else {
                sim_delay_time += dtime;
            }
        }     
        break;
    }
    case 6: { /* Simulation is over */
        /*===========================================================================================
          Simulation finished procesure                                                              
          ===========================================================================================*/
        
        /* Waiting for the new simulation procedure initiated by pressing button 1 */
        if (pressed_iterative(BUTTON_1)) {  
            simulation_state = 3;
        }
        break;
    }}
}

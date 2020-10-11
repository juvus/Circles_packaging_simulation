#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <utils.h>
#include <file_io.h>

/* Main entry point to the game logic procedure */
void
simulate_packaging(Input_t *user_input, f32 dtime, Render_Buffer_t *render_buffer);
                   
#endif // SIMULATION_H_

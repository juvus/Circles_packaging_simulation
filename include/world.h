/*================================================================================*/
/* Header file for the world class.                                               */
/*================================================================================*/

#ifndef WORLD_H_
#define WORLD_H_

#include <utils.h>
#include <software_rendering.h>

/* Enumerator for the different simulation modes */
enum Sim_Mode {
    SIM_STOPPED, /* Simulation has not been started yet */
    SIM_NORMAL, /* Normal simulation mode (button 1)*/
    SIM_PAUSED /* Pause in the packaging simulation (button 2) */
};
typedef enum Sim_Mode Sim_Mode_t;

/* Structure of the world with data */
struct World {
    u32 width; /* World width (paintable part) */
    u32 height; /* World height (paintable part) */
    u32 bkg_color; /* Color of the background */
    Sim_Mode_t sim_mode; /* Current simulation mode */
    f32 sim_time; /* Simulation time (in seconds) */
    f32 sim_delay; /* Delay in seconds between circles packaging */
};
typedef struct World World_t;

/* Methods of world class */
/* Method for creation of the world object (malloc) */
World_t *world_create(void);

/* Method for deleting the world object */
void world_delete(World_t *world);

/* Initialization of the world object */
void world_init(World_t *world, Render_Buffer_t *render_buffer, const u32 bkg_color,
                const Sim_Mode_t sim_mode, f32 sim_delay);

/* Method for render the world */
void world_render(const World_t *world, Render_Buffer_t *render_buffer);

#endif //WORLD_H

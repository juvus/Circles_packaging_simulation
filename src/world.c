/*================================================================================*/
/* Realization of world methods                                                   */
/*================================================================================*/

#define _USE_MATH_DEFINES

/* Standard incudes: */
#include <math.h>

/* Program includes: */
#include <world.h>
#include <utils.h>
#include <software_rendering.h>

World_t*
world_create(void)
{
    /* Method for creation of the world object (malloc) */

    World_t *tmp_world;
    
    tmp_world = (World_t *) calloc (1, sizeof(World_t));   
    return tmp_world;
}

void
world_delete(World_t *world)
{
    /* Method for deleting the world object */
    free(world);
}

void
world_init(World_t *world, Render_Buffer_t *render_buffer, const u32 bkg_color,
           const Sim_Mode_t sim_mode, f32 sim_delay)
{
    /* Initialization of the world object fields (constructor) */

    world->width = render_buffer->width; /* Not including the window frame */
    world->height = render_buffer->height; /* Not including the window frame */
    world->bkg_color = bkg_color;

    world->sim_mode = sim_mode;
    world->sim_delay = sim_delay;
    world->sim_time = 0.0f;
}

void
world_render(const World_t *world, Render_Buffer_t *render_buffer)
{
    /* Method for render the world */

    draw_rect(0, 0, world->width, world->height, world->bkg_color, render_buffer);
}

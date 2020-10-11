/*================================================================================*/
/* Realization of circle methods                                                  */
/*================================================================================*/

#define _USE_MATH_DEFINES

/* Standard incudes: */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Program includes: */
#include <circle.h>
#include <utils.h>
#include <misc.h>
#include <software_rendering.h>

Circle_t*
circle_create(V2_u32_t center, u32 radius, u32 color)
{
    /* Method for creation of the circle object (malloc)*/
    Circle_t *temp_circle_p;
    temp_circle_p = (Circle_t *) calloc (1, sizeof(Circle_t));

    temp_circle_p->center = center;
    temp_circle_p->radius = radius;
    temp_circle_p->color = color;

    return temp_circle_p;
}

void
circle_delete(Circle_t *circle)
{
    /* Method for deleting the circle from the memory */
    free(circle);
}

void
circle_set_position(Circle_t *circle, V2_u32_t center)
{
    /* Method for setting the position of the circle (position of the center point) */

    circle->center.x = center.x;
    circle->center.y = center.y;   
}

void
circle_render(Circle_t *circle, Render_Buffer_t *render_buffer)
{
    /* Method for render the circle */
    
    draw_circle(circle->center.x, circle->center.y, circle->radius, true, circle->color, render_buffer);
    draw_circle(circle->center.x, circle->center.y, circle->radius, false, 0x000000, render_buffer);
}

void
circle_render_marker(V2_u32_t marker_center, Render_Buffer_t *render_buffer)
{
    /* Method for render the circle marker (last added circle) */

    draw_circle(marker_center.x, marker_center.y, 3, true, 0xee0000, render_buffer);
}

void
circle_generate_params(const u32 circles_types, const f32 *circles_prob, const u32 *circles_col,
                       const u32 *circles_radiuses, u32 *color, u32 *radius)
{
    /* Method for generation of the radius from the probability distribution */

    f32 rnd_number;
    f32 left_brd, right_brd;
    u32 i;

    rnd_number = rnd_f32();
    left_brd = 0;
    right_brd = circles_prob[1];
    
    for (i = 0; i < circles_types; ++i) {
        
        if ((rnd_number >= left_brd) && (rnd_number < right_brd)) {
            *color = circles_col[i];
            *radius = circles_radiuses[i];
            return;
        }
        else {
            left_brd = right_brd;
            right_brd += circles_prob[i + 1];
        }
    }
}

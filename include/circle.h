/*================================================================================*/
/* Header file for the circle class.                                              */
/*================================================================================*/
#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <utils.h>
#include <software_rendering.h>

/* Structure of the circle */
struct Circle {
    V2_u32_t center; /* Coordinates of the center */
    u32 radius; /* Radius of the circle */
    u32 color; /* Color of the circle */
};
typedef struct Circle Circle_t;

/* Methods of circle class */
/* Method for creation of the circle object (malloc)*/
Circle_t* circle_create(V2_u32_t center, u32 radius, u32 color);

/* Method for deleting the circle from the memory */
void circle_delete(Circle_t *circle);

/* Method for setting the position of the circle (position of the center point) */
void circle_set_position(Circle_t *circle, V2_u32_t center);

/* Method for render the circle */
void circle_render(Circle_t *circle, Render_Buffer_t *render_buffer);

/* Method for render the circle marker (last added circle) */
void circle_render_marker(V2_u32_t marker_center, Render_Buffer_t *render_buffer);

/* Method for generation of the radius from the probability distribution */
void circle_generate_params(const u32 circles_types, const f32 *circles_prob, const u32 *circles_col,
                            const u32 *circles_radiuses, u32 *color, u32 *radius);

#endif //CIRCLE_H

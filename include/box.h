/*================================================================================*/
/* Header file for the box class.                                                 */
/*================================================================================*/

#ifndef BOX_H_
#define BOX_H_

#include <utils.h>
#include <software_rendering.h>
#include <world.h>
#include <circle.h>
#include <circles_list.h>

/* Structure of the single block if the box */
struct Box_block {
    V2_u32_t BL; /* Coordinates of the block bottom-left point */
    V2_u32_t UR; /* Coordinates of the block up-right point */
    u32 width; /* Width of the block in pix */
    u32 height; /* Height of the block in pix */
    Circles_list_t *circles_list; /* Pointer to the corresponding circles list */    
};
typedef struct Box_block Box_block_t;

/* Structure of the box class */
struct Box {
    V2_u32_t BL_out; /* Coordinates of the bottom-left outer corner */
    V2_u32_t BL_in; /* Coordinates of the bottom-left inner corner */
    V2_u32_t UL_out; /* Coordinates of the up-left outer corner */
    V2_u32_t UL_in; /* Coordinates of the up-left inner corner */
    V2_u32_t BR_out; /* Coordinates of the bottom-right outer corner */
    V2_u32_t BR_in; /* Coordinates of the bottom-right inner corner */
    V2_u32_t UR_out; /* Coordinates of the up-right outer corner */
    V2_u32_t UR_in; /* Coordinates of the up-right inner corner */
    u32 width_out; /* Outer width of the box */
    u32 width_in; /* Inner width of the box */
    u32 height_out; /* Outer height of the box */
    u32 height_in; /* Inner height of the box */
    u32 wall_width; /* Width of the box wall */
    u32 wall_clr; /* Color of the box wall */
    u32 interior_clr; /* Color of the box interior */
    Circles_list_t *circles_list; /* List of the all packed circles (need to delete them) */
    u32 packed_circles_num; /* Number of already packed circles */
    f32 occupied_fraction; /* Occupied area fraction of the box */
    u32 blocks_side_num; /* Number of blocks along one side of the box_blocks */
    Box_block_t **blocks; /* Pointer to the array of box blocks */
};
typedef struct Box Box_t;

/* Methods of box class */
/* Method for creation of the box object (malloc) */
Box_t *box_create(const u32 blocks_side_num);

/* Method for deleting the box object (free) */
void box_delete(Box_t *box);

/* Initialization of the box object fields */
void box_init(Box_t *box, World_t *world, const u32 box_width_out, const u32 box_height_out,
              const u32 box_wall_width, const u32 text_panel_height, const u32 box_wall_clr,
              const u32 box_interior_clr);

/* Method for adding new circle to the corresponding box block */
void box_add_circle(Box_t *box, Circle_t *circle);

/* Method for render the box */
void box_render(Box_t *box, Render_Buffer_t *render_buffer);

#endif //BOX_H

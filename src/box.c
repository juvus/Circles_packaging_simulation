/*================================================================================*/
/* Realization of box methods                                                     */
/*================================================================================*/

#define _USE_MATH_DEFINES

/* Standard incudes: */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Program includes: */
#include <box.h>
#include <utils.h>
#include <world.h>
#include <circles_list.h>
#include <software_rendering.h>
#include <misc.h>

/* Static functions */
/* Function for calculation the box blocks parameters */
static void box_calc_blocks_params(Box_t *box);

Box_t*
box_create(const u32 blocks_side_num)
{
    /* Method for creation of the box object (malloc) */
    Box_t *tmp_box;
    u32 i, j;

    /* Allocate the memory for the box itself */
    tmp_box = (Box_t*) calloc (1, sizeof(Box_t));
    if (NULL == tmp_box) print_error_and_exit("Error in memory allocation!\n");

    /* Allocate the memory for box blocks */
    tmp_box->blocks_side_num = blocks_side_num;
    tmp_box->blocks = (Box_block_t**) calloc (blocks_side_num, sizeof(Box_block_t*));
    if (NULL == tmp_box->blocks) print_error_and_exit("Error in memory allocation!\n");

    for (i = 0; i < blocks_side_num; ++i) {
        tmp_box->blocks[i] = (Box_block_t*) calloc (blocks_side_num, sizeof(Box_block_t));
        if (NULL == tmp_box->blocks[i]) print_error_and_exit("Error in memory allocation!\n");
    }
   
    /* Create circles_list for every created block */
    for (i = 0; i < blocks_side_num; ++i) {
        for (j = 0; j < blocks_side_num; ++j) {
            tmp_box->blocks[i][j].circles_list = circles_list_create();
        }
    }

    /* Allocate memory for the list of all packed circles */
    tmp_box->circles_list = circles_list_create();
    
    return tmp_box;
}

void
box_delete(Box_t *box)
{
    /* Method for deleting the box object (free). Delete in reverse order */

    u32 i, j;
    
    /* Clear all the packed circle objects */
    circles_list_empty(box->circles_list, 1);

    /* 1. Delete the circles lists, and dont touch circles object, because their pointers can double in 
       the lists */
    for (i = 0; i < box->blocks_side_num; ++i) {
        for (j = 0; j < box->blocks_side_num; ++j) {
            circles_list_delete(box->blocks[i][j].circles_list, 0);
        }
    }

    /* 2. Delete the blocks array */
    for (i = 0; i < box->blocks_side_num; ++i) {
        free(box->blocks[i]);
    }
    free(box->blocks);

    /* 3. Delete the box */
    free(box);
}

void
box_init(Box_t *box, World_t *world, const u32 box_width_out, const u32 box_height_out,
         const u32 box_wall_width, const u32 text_panel_height, const u32 box_wall_clr,
         const u32 box_interior_clr)
{
    /* Initialization of the box object fields */

    u32 i, j;
    
    /* Initialization of fields from the parameter list */
    box->width_out = box_width_out;
    box->height_out = box_height_out;
    box->wall_width = box_wall_width;
    box->wall_clr = box_wall_clr;
    box->interior_clr = box_interior_clr;

    box->BL_out.x = (world->width - box->width_out) / 2;
    box->BL_out.y = (world->height - box->height_out - text_panel_height) / 3;
    
    box->width_in = box->width_out - 2 * box->wall_width;
    box->height_in = box->height_out - box->wall_width;
    box->BL_in.x = box->BL_out.x + box->wall_width;
    box->BL_in.y = box->BL_out.y + box->wall_width;
    box->UL_out.x = box->BL_out.x; 
    box->UL_out.y = box->BL_out.y + box->height_out;
    box->UL_in.x = box->UL_out.x + box->wall_width; 
    box->UL_in.y = box->UL_out.y;
    box->BR_out.x = box->BL_out.x + box->width_out;
    box->BR_out.y = box->BL_out.y;
    box->BR_in.x = box->BR_out.x - box->wall_width;
    box->BR_in.y = box->BR_out.y + box->wall_width;
    box->UR_out.x = box->BR_out.x;
    box->UR_out.y = box->BR_out.y + box->height_out;
    box->UR_in.x = box->UR_out.x - box->wall_width;
    box->UR_in.y = box->UR_out.y;

    /* Initialization of non-constant fields */
    box->packed_circles_num = 0;
    box->occupied_fraction = 0.0f;

    /* Calculate the parameters of the each block */
    box_calc_blocks_params(box);

    /* Clear all the packed circle objects */
    circles_list_empty(box->circles_list, 1);

    /* Clear the possible contents of the box blocks */
    for (i = 0; i < box->blocks_side_num; ++i) {
        for (j = 0; j < box->blocks_side_num; ++j) {
            circles_list_empty(box->blocks[i][j].circles_list, 0);
        }
    }
}

static void
box_calc_blocks_params(Box_t *box)
{
    /* Function for calculation of the blocks parameters like coordinates of the BL point, 
       width and height */
       
    u32 i, j;
    u32 block_width_length; /* Length of the block width */
    u32 block_height_length; /* Length of the block height */

    /* Calculate the blocks side length */
    block_width_length = (u32)roundf((f32)box->width_in / (f32)box->blocks_side_num);
    block_height_length = (u32)roundf((f32)box->height_in / (f32)box->blocks_side_num);
    
    for (i = 0; i < box->blocks_side_num; ++i) {
        for (j = 0; j < box->blocks_side_num; ++j) {
            
            /* Calculate the BL coordinates of the blocks */
            box->blocks[i][j].BL.x = box->BL_in.x + i * block_width_length;
            box->blocks[i][j].BL.y = box->BL_in.y + j * block_height_length;
            
            /* Calculate the width of the blocks */
            if (i == (box->blocks_side_num - 1)) {
                /* Special case for the last block in block columns */
                box->blocks[i][j].width = box->width_in - block_width_length * (box->blocks_side_num - 1);
            }
            else {
                /* Normal block */
                box->blocks[i][j].width = block_width_length;
            }

            /* Calculate the height of the block */
            if (j == (box->blocks_side_num - 1)) {
                /* Special case for the last block in block rows */
                box->blocks[i][j].height = box->height_in - block_height_length * (box->blocks_side_num - 1);
            }
            else {
                /* Normal block */
                box->blocks[i][j].height = block_height_length;
            }

            /* Calculate the UR coordinates of the block */
            box->blocks[i][j].UR.x = box->blocks[i][j].BL.x + block_width_length;
            box->blocks[i][j].UR.y = box->blocks[i][j].BL.y + block_height_length;
        }
    }   
}

void
box_add_circle(Box_t *box, Circle_t *circle)
{
    /* Method for adding new circle to the corresponding box block */
    
    u32 i, j;
    V2_u32_t block_BL, block_UR; /* BL and UR points of outside rectangle of the block */
    V2_u32_t circ_BL, circ_UR; /* BL and UR points of outside rectangle of the circle */

    /* Determine the parameters of the outside rectangle of the circle */
    circ_BL.x = circle->center.x - circle->radius;
    circ_BL.y = circle->center.y - circle->radius;
    circ_UR.x = circle->center.x + circle->radius;
    circ_UR.y = circle->center.y + circle->radius;

    /* Check every box block for the colision with circle outside rectangle */
    for (i = 0; i < box->blocks_side_num; ++i) {
        for (j = 0; j < box->blocks_side_num; ++j) {

            /* Determine the parameters of the outside rectangle of the current block */
            block_BL.x = box->blocks[i][j].BL.x;
            block_BL.y = box->blocks[i][j].BL.y;
            block_UR.x = box->blocks[i][j].UR.x;
            block_UR.y = box->blocks[i][j].UR.y;

            /* Check the colision of the outside rectangles of block and circle */
            if (rectangles_are_collide(block_BL, block_UR, circ_BL, circ_UR)) {
                /* Add circle to the current box block */
                circles_list_push(box->blocks[i][j].circles_list, circle);               
            }
        }
    }

    circles_list_push(box->circles_list, circle);
    box->packed_circles_num += 1;
}

void
box_render(Box_t *box, Render_Buffer_t *render_buffer)
{
    /* Method for render the box */
    /* Draw inner part */
    draw_rect(box->BL_in.x, box->BL_in.y, box->width_in, box->height_in, box->interior_clr, render_buffer);
    
    /* Draw walls */
    draw_rect(box->BL_out.x, box->BL_out.y, box->wall_width, box->height_out, box->wall_clr, render_buffer);
    draw_rect(box->BL_out.x, box->BL_out.y, box->width_out, box->wall_width, box->wall_clr, render_buffer);
    draw_rect(box->BR_in.x, box->BR_in.y, box->wall_width, box->height_in, box->wall_clr, render_buffer);
}

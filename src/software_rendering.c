#define _USE_MATH_DEFINES

/* Standard incudes: */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Program includes: */
#include <software_rendering.h>
#include <utils.h>
#include <file_io.h>

/* Static (global in the file) variables and data structures */
static u32 LSX_array[WINDOW_WIDTH]; /* Left Side X array */
static u32 RSX_array[WINDOW_WIDTH]; /* Rigth Side X array */
static u32 LSX_i = 0; /* Left Side X array index */
static u32 RSX_i = 0; /* Right Side X array index */

/* Static functions */
static void draw_line_extended(V2_u32_t v0, V2_u32_t v1, u32 color, Render_Buffer_t *render_buffer,
                               u32 *SX_array, u32 *SX_i);
static void draw_fill_bottom_flat_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, u32 color,
                                          Render_Buffer_t *render_buffer);
static void draw_fill_top_flat_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, u32 color,
                                        Render_Buffer_t *render_buffer);
u32 convert_RGBA_to_ARGB(u32 color);
u32 get_gray_color(u8 value);

/* Manually defined image of digits consisting of large pixels */
static const u8 DIGITS[11][7][5] =
    {
     /* Digit 0: */
     {{0, 1, 1, 1, 0}, {1, 1, 0, 0, 1}, {1, 1, 0, 0, 1}, {1, 1, 0, 0, 1},
      {1, 1, 0, 0, 1}, {1, 1, 0, 0, 1}, {0, 1, 1, 1, 0}},
     /* Digit 1: */
     {{0, 1, 1, 0, 0}, {1, 1, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 1, 1, 0, 0},
      {0, 1, 1, 0, 0}, {0, 1, 1, 0, 0}, {1, 1, 1, 1, 0}},
     /* Digit 2: */
     {{0, 1, 1, 1, 0}, {1, 0, 0, 1, 1}, {0, 0, 0, 1, 1}, {0, 0, 1, 1, 0},
      {0, 1, 1, 0, 0}, {1, 1, 0, 0, 1}, {1, 1, 1, 1, 1}},
     /* Digit 3: */
     {{0, 1, 1, 1, 0}, {1, 0, 0, 1, 1}, {0, 0, 0, 1, 1}, {0, 1, 1, 1, 0},
      {0, 0, 0, 1, 1}, {1, 0, 0, 1, 1}, {0, 1, 1, 1, 0}},
     /* Digit 4: */
     {{1, 0, 0, 1, 1}, {1, 0, 0, 1, 1}, {1, 0, 0, 1, 1}, {1, 1, 1, 1, 1},
      {0, 0, 0, 1, 1}, {0, 0, 0, 1, 1}, {0, 0, 0, 1, 1}},
     /* Digit 5: */
     {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1},
      {0, 0, 0, 1, 1}, {1, 0, 0, 1, 1}, {1, 1, 1, 1, 1}},
     /* Digit 6: */
     {{1, 1, 1, 1, 1}, {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 1, 1, 1},
      {1, 1, 0, 0, 1}, {1, 1, 0, 0, 1}, {1, 1, 1, 1, 1}},
     /* Digit 7: */
     {{1, 1, 1, 1, 1}, {0, 0, 0, 1, 1}, {0, 0, 1, 1, 0}, {0, 0, 1, 1, 0},
      {0, 0, 1, 1, 0}, {0, 1, 1, 0, 0}, {0, 1, 1, 0, 0}},
     /* Digit 8: */
     {{1, 1, 1, 1, 1}, {1, 1, 0, 0, 1}, {1, 1, 0, 0, 1}, {1, 1, 1, 1, 1},
      {1, 1, 0, 0, 1}, {1, 1, 0, 0, 1}, {1, 1, 1, 1, 1}},
     /* Digit 9: */
     {{1, 1, 1, 1, 1}, {1, 1, 0, 0, 1}, {1, 1, 0, 0, 1}, {1, 1, 1, 1, 1},
      {0, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}},
     /* Symbol x */
     {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0},
      {0, 1, 0, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
    };

void
clear_full_screen(u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for clear the screen with the desired color */

    u32 *pixel = render_buffer->pixels;
    u32 x, y;

    for (y = 0; y < render_buffer->height; y++) {
        for (x = 0; x < render_buffer->width; x++) {
            *(pixel + x + render_buffer->width * y) = color;
        }
    }
}

void
draw_pixel(u32 x, u32 y, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing a single pixel on the screen */

    u32 *pixel = render_buffer->pixels;
    *(pixel + x + render_buffer->width * y) = color;
}

void
draw_line(u32 x0, u32 y0, u32 x1, u32 y1, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing a pixelized line from one point to another */
    V2_u32_t v0 = {.x = x0, .y = y0};
    V2_u32_t v1 = {.x = x1, .y = y1};
    draw_line_extended(v0, v1, color, render_buffer, LSX_array, &LSX_i);
}

void
draw_hor_line(u32 x0, u32 y0, u32 length, u32 width, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing the horisontal line */
    u32 *pixel = render_buffer->pixels;
    u32 x, y;

    for (x = x0; x < (x0 + length); ++x) {
        for (y = y0; y < (y0 + width); ++y) {
            *(pixel + x + render_buffer->width * y) = color;
        }
    }
}

void
draw_ver_line(u32 x0, u32 y0, u32 length, u32 width, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing the horisontal line */
    u32 *pixel = render_buffer->pixels;
    u32 x, y;

    for (y = y0; y < (y0 + length); ++y) {
        for (x = x0; x < (x0 + width); ++x) {
            *(pixel + x + render_buffer->width * y) = color;
        }
    }
}

static void
draw_line_extended(V2_u32_t v0, V2_u32_t v1, u32 color, Render_Buffer_t *render_buffer,
                   u32 *SX_array, u32 *SX_i)
{
    /* Function for drawing a pixelized line from one point to another by using
       Brezenham algorithm. This version also record the y coordinates for
       further rasterization filling of the triangle */

    u32 *pixel = render_buffer->pixels;
    s32 xerr = 0, yerr = 0;
    s32 dx = v1.x - v0.x;
    s32 dy = v1.y - v0.y;
    s32 incX, incY;
    s32 d, x, y;
    s32 i;

    *SX_i = 0; /* Initialize the side array index to zero */

    /* Deterime increment */
    if (dx > 0) /* I or IV quarter */ 
        incX = 1;
    else 
        if (dx < 0) /* II or III quarter */
            incX = -1;
        else /* Oy axis */ 
            incX = 0;
 
    if (dy > 0) /* I or II quarter */
        incY = 1;
    else 
        if (dy < 0) /* III or IV quarter */
            incY = -1;
        else /* Ox axis */
            incY = 0;

    /* Detrmine delta argument */
    dx = abs(dx); 
    dy = abs(dy);
 
    if (dx > dy) /* Line is closer to Ox axis */
        d = dx; 
    else /* Line is closer to Oy axis */
        d = dy;

    /* Render the line */
    x = v0.x; 
    y = v0.y; 
    for (i = 0; i <= d; i++) {
              
        xerr = xerr + dx;
        yerr = yerr + dy;

        if (xerr > d) { 
            xerr -= d; 
            x += incX; 
        }

        if (yerr > d) { 
            yerr -= d; 
            y += incY;
            SX_array[(*SX_i)++] = x; /* Record x when y has shanged */
        }

        *(pixel + x + render_buffer->width * y) = color;
    }
    (*SX_i)--;
}

void
draw_rect(u32 x0, u32 y0, u32 width, u32 height, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing the rectangle */

    u32 x, y;
    u32 *pixel = render_buffer->pixels;

    for(y = y0; y < height + y0; y++) {
        for (x = x0; x < width + x0; x++) {
            *(pixel + x + render_buffer->width * y) = color;
        }
    }
}

void
draw_rect_with_brd(u32 x0, u32 y0, u32 width, u32 height, u32 brd_width, u32 color, u32 brd_color,
                   Render_Buffer_t *render_buffer)
{
    /* Function for drawing the rectangle with boarder */

    /* Render the inner rectangle */
    draw_rect(x0 + brd_width, y0 + brd_width, width - brd_width * 2, height - brd_width * 2,
              color, render_buffer);

    /* Render the boarders with desired width and color */
    if (brd_width) {
        draw_rect(x0, y0, width, brd_width, brd_color, render_buffer);
        draw_rect(x0, y0 + height - brd_width, width, brd_width, brd_color, render_buffer);
        draw_rect(x0, y0, brd_width, height, brd_color, render_buffer);
        draw_rect(x0 + width -brd_width, y0, brd_width, height, brd_color, render_buffer);
    }
}

static void
draw_fill_bottom_flat_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, u32 color,
                               Render_Buffer_t *render_buffer)
{
    /* Function for drawing filled bottom flat triangle. Vertices are the following:
       v1 - top most vertice, v2 - left most vertice, v3 - right most vertice */

    u32 y;
    u32 index;
    u32 length;
    
    /* Draw the side lines of the triangle */
    draw_line_extended(v1, v2, color, render_buffer, LSX_array, &LSX_i);
    draw_line_extended(v1, v3, color, render_buffer, RSX_array, &RSX_i);

    /* Fill the rectangle with horisontal lines */
    index = 0;
    y = v1.y - 1; /* Start from the 1 line lower then top point */
    for (index = 0; index <= LSX_i; ++index) {
        length = RSX_array[index] - LSX_array[index];
        draw_hor_line(LSX_array[index], y, length, 1, color, render_buffer);
        y--;
    }
}

static void
draw_fill_top_flat_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, u32 color,
                            Render_Buffer_t *render_buffer)
{
    /* Function for drawing filled top flat triangle. Vertices are the following:
       v1 - bottom most vertice, v2 - left most vertice, v3 - right most vertic */

    u32 y;
    u32 index;
    u32 length;

    /* Draw the side lines of the triangle */
    draw_line_extended(v1, v2, color, render_buffer, LSX_array, &LSX_i);
    draw_line_extended(v1, v3, color, render_buffer, RSX_array, &RSX_i);

    /* Fill the rectangle with horisontal lines */
    index = 0;
    y = v1.y + 1; /* Start from the 1 line lower then top point */
    for (index = 0; index <= LSX_i; ++index) {
        length = RSX_array[index] - LSX_array[index];
        draw_hor_line(LSX_array[index], y, length, 1, color, render_buffer);
        y++;
    }    
}

void draw_fill_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, u32 color,
                        Render_Buffer_t *render_buffer)
{
    /* Function for drawing filled triangle */

    V2_u32_t v_temp; /* Temp vertice */
    f32 temp; /* temp value */
    V2_u32_t v4; /* Additional vertice */
    
    /* Sort the three vertices by y-coordinate so that v1 is the topmost vertice */
    if (v1.y < v2.y) {
        if (v2.y < v3.y) {
            /* v3 is topmost, swap */
            v_temp.x = v1.x; v_temp.y = v1.y;
            v1.x = v3.x; v1.y = v3.y;
            v3.x = v_temp.x; v3.y = v_temp.y;
        }
        else {
            /* v2 is topmost or equal to v3, swap*/
            v_temp.x = v1.x; v_temp.y = v1.y;
            v1.x = v2.x; v1.y = v2.y;
            v2.x = v_temp.x; v2.y = v_temp.y;
        }
    }
    else {
        if (v1.y < v3.y) {
            /* v3 is topmost, swap */
            v_temp.x = v1.x; v_temp.y = v1.y;
            v1.x = v3.x; v1.y = v3.y;
            v3.x = v_temp.x; v3.y = v_temp.y;
        }
    }

    /* Sort v2 and v3 by y coordinate so that v3 is bottommost */
    if (v2.y < v3.y) { /* v3 is bottommost, swap */
        v_temp.x = v2.x; v_temp.y = v2.y;
        v2.x = v3.x; v2.y = v3.y;
        v3.x = v_temp.x; v3.y = v_temp.y;
    }

    /* Check for different trial and general cases: */
    /* Case 1. Simple point */
    if ((v1.y == v2.y) && (v2.y  == v3.y) && (v1.x == v2.x) && (v2.x == v3.x)) {
        draw_pixel(v1.x, v1.y, color, render_buffer);
    }
    /* Case 2. Horisontal or vertical line */
    else if (((v1.y == v2.y) && (v2.y  == v3.y)) || ((v1.x == v2.x) && (v2.x == v3.x))) {
        draw_line(v1.x, v1.y, v2.x, v2.y, color, render_buffer);
        draw_line(v2.x, v2.y, v3.x, v3.y, color, render_buffer);       
    }
    /* Case 3. Bottom-flat triangle */
    else if (v2.y == v3.y) {
        if (v2.x < v3.x) { /* v2 - left, v3 - right */
            draw_fill_bottom_flat_triangle(v1, v2, v3, color, render_buffer);
        }
        else {  /* v3 - left, v2 - right */
            draw_fill_bottom_flat_triangle(v1, v3, v2, color, render_buffer);
        }      
    }
    /* Case 4. Top-flat triangle */
    else if (v1.y == v2.y) {
        if (v1.x < v2.x) { /* v1 - left, v2 - right */
            draw_fill_top_flat_triangle(v3, v1, v2, color, render_buffer);
        }
        else { /* v2 - left, v1 - right */
            draw_fill_top_flat_triangle(v3, v2, v1, color, render_buffer);
        }      
    }
    /* Case 5. General case, split the triangle in a top-flat and bottom-flat */
    else {
        temp = ((f32)v3.x - (f32)v1.x) * ((f32)v1.y - (f32)v2.y) / ((f32)v1.y - (f32)v3.y);
        v4.x = (u32)(roundf(temp + (f32)v1.x));
        v4.y = v2.y;

        /* Draw top triangle */
        if (v2.x < v4.x) {
            draw_fill_bottom_flat_triangle(v1, v2, v4, color, render_buffer);
        }
        else {
            draw_fill_bottom_flat_triangle(v1, v4, v2, color, render_buffer);
        }
        
        /* Draw bottom triangle */
        if (v2.x < v4.x) {
            draw_fill_top_flat_triangle(v3, v2, v4, color, render_buffer);
        }
        else {
            draw_fill_top_flat_triangle(v3, v4, v2, color, render_buffer);
        }
    }

    /* Clean-up. Draw additional lines to to each vertices */
    draw_line(v1.x, v1.y, v2.x, v2.y, color, render_buffer);
    draw_line(v2.x, v2.y, v1.x, v1.y, color, render_buffer);

    draw_line(v2.x, v2.y, v3.x, v3.y, color, render_buffer);
    draw_line(v3.x, v3.y, v2.x, v2.y, color, render_buffer);

    draw_line(v1.x, v1.y, v3.x, v3.y, color, render_buffer);
    draw_line(v3.x, v3.y, v1.x, v1.y, color, render_buffer);  
}

void
draw_circle(u32 x0, u32 y0, u32 radius, b32 is_filled ,u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing a circle (using Brezenhame algorithm) */

    s32 x = 0;
    s32 y = (s32)radius;
    s32 gap = 0;
    s32 delta = (2 - 2 * (s32)radius);

    while (y >= 0) {
        /* Draw points lying on a circle */
        draw_pixel(x0 + (u32)x, y0 + (u32)y, color, render_buffer);
        draw_pixel(x0 + (u32)x, y0 - (u32)y, color, render_buffer);
        draw_pixel(x0 - (u32)x, y0 - (u32)y, color, render_buffer);
        draw_pixel(x0 - (u32)x, y0 + (u32)y, color, render_buffer);

        /* Fill the circle if it is necessary (with lines from top to bottom)*/
        if (is_filled) {    
            draw_line(x0 + (u32)x, y0 + (u32)y, x0 + (u32)x, y0 - (u32)y,  color, render_buffer);
            draw_line(x0 - (u32)x, y0 + (u32)y, x0 - (u32)x, y0 - (u32)y,  color, render_buffer);
        }

        gap = 2 * (delta + y) - 1;
        if ((delta < 0) && (gap <= 0)) {
            x++;
            delta += 2 * x + 1;
            continue;
        }

        if ((delta > 0) && (gap > 0)) {
            y--;
            delta -= 2 * y + 1;
            continue;
        }

        x++;
        delta += 2 * (x - y);
        y--;
    }   
}



void draw_rotated_rect(u32 x0, u32 y0, u32 width, u32 height, f32 angle, u32 color,
                       Render_Buffer_t *render_buffer)
{
    /* Function for drawing the rectangle rotated by angle degrees */
    
    f32 angle_rad; /* Angle in radians */
    f32 xc, yc; /* Coordinates of the center point of rectangle */
        
    V2_s32_t BL, BR, UL, UR; /* Initial points of the rectangle */
    V2_s32_t BL_c, BR_c, UL_c, UR_c; /* Coordinates with respect to the rect center */
    V2_u32_t BL_tr, BR_tr, UL_tr, UR_tr; /* Points after rotation transformation */

    /* Initialize the points with the data */
    BL.x = x0; BL.y = y0;
    BR.x = x0 + width; BR.y = y0;
    UL.x = x0; UL.y = y0 + height;
    UR.x = x0 + width; UR.y = y0 + height;
    
    /* Transform degrees to radians */
    angle_rad = angle * (f32)M_PI / 180.0f;

    /* Determine the center point of the rectangle */
    xc = x0 + width / 2.0f;
    yc = y0 + height / 2.0f;

    /* Determine new points coordinates (with recpect to the rect center) */
    BL_c.x = BL.x - (s32)roundf(xc); BL_c.y = BL.y - (s32)roundf(yc);
    BR_c.x = BR.x - (s32)roundf(xc); BR_c.y = BR.y - (s32)roundf(yc);
    UL_c.x = UL.x - (s32)roundf(xc); UL_c.y = UL.y - (s32)roundf(yc);
    UR_c.x = UR.x - (s32)roundf(xc); UR_c.y = UR.y - (s32)roundf(yc);

    /* Transform the points by rotation */
    BL_tr.x = (u32)roundf(xc + (BL_c.x * cosf(angle_rad) - BL_c.y * sinf(angle_rad)));
    BL_tr.y = (u32)roundf(yc + (BL_c.x * sinf(angle_rad) + BL_c.y * cosf(angle_rad)));
    BR_tr.x = (u32)roundf(xc + (BR_c.x * cosf(angle_rad) - BR_c.y * sinf(angle_rad)));
    BR_tr.y = (u32)roundf(yc + (BR_c.x * sinf(angle_rad) + BR_c.y * cosf(angle_rad)));
    UL_tr.x = (u32)roundf(xc + (UL_c.x * cosf(angle_rad) - UL_c.y * sinf(angle_rad)));
    UL_tr.y = (u32)roundf(yc + (UL_c.x * sinf(angle_rad) + UL_c.y * cosf(angle_rad)));
    UR_tr.x = (u32)roundf(xc + (UR_c.x * cosf(angle_rad) - UR_c.y * sinf(angle_rad)));
    UR_tr.y = (u32)roundf(yc + (UR_c.x * sinf(angle_rad) + UR_c.y * cosf(angle_rad)));

    /* Draw the rectangle (by drawing two triangles) */
    draw_fill_triangle(BL_tr, BR_tr, UR_tr, color, render_buffer);
    draw_fill_triangle(BL_tr, UL_tr, UR_tr, color, render_buffer);        
}

void
draw_digit(u32 x, u32 y, u8 digit, u32 size, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing a single digit symbol
       x - x position of the BL corner of the box
       y - y position of the BL corner of the box
       digit - digit to draw (0 - 9)
       size - size of the "pixels of a drawing". size = 1 representing normal pixels
       color - color of the digit
       render_buffer - render buffer with pixels */

    u32 i, j, x_pos, y_pos;
    
    for (i = 0; i < 7; ++i) {
        for (j = 0; j < 5; ++j) {
            /* Calculate the possition taking into account the with of large pixels */
            x_pos = x + j * size;
            y_pos = y + i * size;

            /* Draw the digit 0 - 9 */
            if (DIGITS[digit][6 - i][j]) {
                    draw_rect(x_pos, y_pos, size, size, color, render_buffer);
            }            
        }
    }
}

void
draw_number(u32 x, u32 y, u32 number, u32 size, u32 color, Render_Buffer_t *render_buffer)
{
    /* Function for drawing an insigned int number with pixelized digits
       x - x position of the BL corner of the box
       y - y position of the BL corner of the box
       number - number to draw (of the unsigned int 32 bit type) 
       size - size of the "pixels of a drawing". size = 1 representing normal pixels
       color - color of the digit
       render_buffer - render buffer with pixels */

    u32 digits_array[10] = {0};
    u32 i, x_pos, y_pos;
    u32 temp_number;

    /* Create the array of number digits */
    temp_number = number;
    for (i = 0; i < 10; ++i) {
        digits_array[9 - i] = temp_number % 10;
        temp_number = (u32)(temp_number / 10); 
    }

    /* Draw the number digit by digit */
    x_pos = x;
    y_pos = y;
    for (i = 0; i < 10; ++i) {
        if ((i != 0) && (digits_array[i - 1] == 1)) {
            /* After digit "1" indent should be one size smaller */
            x_pos += size * 5;
        } else {
            x_pos += size * 6;
        }
        draw_digit(x_pos, y_pos, digits_array[i], size, color, render_buffer);
    }
}

void
draw_bitmap(u32 x, u32 y, Loaded_img_t* image, u32 scale, Render_Buffer_t *render_buffer)
{
    /* Function for rendering the loaded png image */
    u32 color;
    u32 x_scaled, y_scaled; 
    u32 i, j; /* Coordinates of the downloaded image */
    u32 m, n; /* Coordinates inside a big pixel  */
    u32 index;
    u32 *pixel = render_buffer->pixels;

    x_scaled = x;
    y_scaled = y;
    index = 0;
    for (i = 0; i < image->height; ++i) {
        for (j = 0; j < image->width; ++j) {

            /* Get the color of the pixel */
            color = convert_RGBA_to_ARGB(*(image->data + index));

            /* Check the alpha channel */
            if (get_color_alpha(color) != 0x00) {

                /* Draw scaled pixel */
                for (n = 0; n < scale; ++n) {
                    for (m = 0; m < scale; ++m) {
                        x_scaled = x + j * scale + m;
                        y_scaled = y + i * scale + n;
                        *(pixel + x_scaled + render_buffer->width * y_scaled) = color;                    
                    }        
                }
            }
            index++;
        }
    }
}

void
draw_object_by_mask(u32 x, u32 y, Loaded_img_t *mask, u32 color, u32 scale, Render_Buffer_t *render_buffer)
{
    /* Function for rendering the colored object determined by mask */

    u32 mask_color;
    u32 x_scaled, y_scaled;
    u32 i, j; /* Coordinates of the downloaded mask image */
    u32 m, n; /* Coordinates inside a big pixel  */
    u32 index;
    u32 *pixel = render_buffer->pixels;

    x_scaled = x;
    y_scaled = y;
    index = 0;
    for (i = 0; i < mask->height; ++i) {
        for (j = 0; j < mask->width; ++j) {

            /* Get the color of the pixel on the mask */
            mask_color = convert_RGBA_to_ARGB(*(mask->data + index));

            /* Skip the pixels with zero alpha (transparent) */
            if (get_color_alpha(mask_color) != 0x00) {

                /* Draw scaled pixel */
                for (n = 0; n < scale; ++n) {
                    for (m = 0; m < scale; ++m) {
                        x_scaled = x + j * scale + m;
                        y_scaled = y + i * scale + n;
                        *(pixel + x_scaled + render_buffer->width * y_scaled) = color;                    
                    }        
                }
            }
            index++;
        }
    }
}

u32
convert_RGBA_to_ARGB(u32 rgba_color) //@rewrite without the structure
{
    /* Function to convert RGBA color to the ARGB color */

    u32 argb_color;
    u8 alpha, red, green, blue;
    Color_bytes_t read_color;

    read_color.color_word = rgba_color;
    alpha = read_color.a;
    red = read_color.r;
    green = read_color.g;
    blue = read_color.b;
   
    set_color(&argb_color, alpha, red, green, blue);
 
    return argb_color;
}

u32
get_gray_color(u8 value)
{
    /* Function to return 32bit gray color from 8bit value */
    u32 gray_color;
    gray_color = ((value << 0) | (value << 8) | (value << 16));
    return gray_color;
}

u8
get_color_alpha(u32 color)
{
    /* Function to get the alpha of the color */
    return (u8)((color & 0xff000000) >> 24);    
}

void
set_color_alpha(u32 *color, u8 alpha)
{
    /* Function to set the alpha of the color
       color - ARGB 32 bit color
       alpha - u8 (0 - 255) value of the new alpha channel */

    *color = *color | (0xff << 24); /* Set the alpha channel to 0xff */
    *color = *color ^ (0xff << 24); /* Set the alpha channel to 0x00 */
    *color = *color | (alpha << 24); /* Set the value of a new alpha channel */
}

u8
get_color_red(u32 color)
{
    /* Function to get the red channel of the color */
    return (u8)((color & 0x00ff0000) >> 16);
}

void
set_color_red(u32 *color, u8 red)
{
    /* Function to set the red channel of the color
       color - ARGB 32 bit color
       red - u8 (0 - 255) value of the new red channel */

    *color = *color | (0xff << 16); /* Set the R channel to 0xff */
    *color = *color ^ (0xff << 16); /* Set the R channel to 0x00 */
    *color = *color | (red << 16); /* Set the value of a new red channel */
}

u8
get_color_green(u32 color)
{
    /* Function to get the green channel of the color */
    return (u8)((color & 0x0000ff00) >> 8);
}

void
set_color_green(u32 *color, u8 green)
{
    /* Function to set the green channel of the color
       color - ARGB 32 bit color
       green - u8 (0 - 255) value of the new green channel */

    *color = *color | (0xff << 8); /* Set the R channel to 0xff */
    *color = *color ^ (0xff << 8); /* Set the R channel to 0x00 */
    *color = *color | (green << 8); /* Set the value of a new green channel */
}

u8
get_color_blue(u32 color)
{
    /* Function to get the blue channel of the color */
    return (u8)((color & 0x000000ff) >> 0);
}

void
set_color_blue(u32 *color, u8 blue)
{
    /* Function to set the blue channel of the color
       color - ARGB 32 bit color
       blue - u8 (0 - 255) value of the new blue channel */

    *color = *color | (0xff << 0); /* Set the R channel to 0xff */
    *color = *color ^ (0xff << 0); /* Set the R channel to 0x00 */
    *color = *color | (blue << 0); /* Set the value of a new blue channel */
}

void
set_color(u32 *color, u8 alpha, u8 red, u8 green, u8 blue)
{
    /* Function to set the color in ARGB format */
    set_color_alpha(color, alpha);
    set_color_red(color, red);
    set_color_green(color, green);
    set_color_blue(color, blue);   
}

u32
get_alpha_blending_color(u32 color_1, u32 color_2, f32 alpha)
{
    /* Function to get the transparent color (linear interpolation algorithm)
       color_1 - background color
       color_2 - object color 
       alpha - level of the object alpha (0.0 ... 1.0). 0.0 - color_1, 1.0 - clolor_2*/

    UColor_t temp_color_1;
    UColor_t temp_color_2;
    UColor_t return_color;
    u8 R_1, G_1, B_1, R_2, G_2, B_2, R_out, G_out, B_out;
    f32 delta;
    
    /* Determine the color components */
    temp_color_1.color = color_1;
    temp_color_2.color = color_2;
    R_1 = temp_color_1.components[2];
    G_1 = temp_color_1.components[1];
    B_1 = temp_color_1.components[0];
    R_2 = temp_color_2.components[2];
    G_2 = temp_color_2.components[1];
    B_2 = temp_color_2.components[0];
       
    /* Determine the output color components (after mixing) */
    if (R_1 >= R_2) {
        delta = (f32)(R_1 - R_2);
        R_out = R_2 + (u8)(delta * (1.0 - alpha));
    } else {
        delta = (f32)(R_2 - R_1);
        R_out = R_1 + (u8)(delta * alpha); 
    }

    if (G_1 >= G_2) {
        delta = (f32)(G_1 - G_2);
        G_out = G_2 + (u8)(delta * (1.0 - alpha));
    } else {
        delta = (f32)(G_2 - G_1);
        G_out = G_1 + (u8)(delta * alpha); 
    }

    if (B_1 >= B_2) {
        delta = (f32)(B_1 - B_2);
        B_out = B_2 + (u8)(delta * (1.0 - alpha));
    } else {
        delta = (f32)(B_2 - B_1);
        B_out = B_1 + (u8)(delta * alpha); 
    }

    /* Return in format ARGB */
    return_color.components[0] = B_out;
    return_color.components[1] = G_out;
    return_color.components[2] = R_out;
    return_color.components[3] = 0xff;

    return return_color.color;
}


#ifndef SOFTWARE_RENDERING_H_
#define SOFTWARE_RENDERING_H_

#include <utils.h>
#include <file_io.h>

/* Union for packing and unpacking color double word (32 bits) */
union UColor {
    u32 color;
    u8 components[4];
};
typedef union UColor UColor_t;

/* Function for clear the screen with the desired color */
void clear_full_screen(u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing a single pixel on the screen */
void draw_pixel(u32 x, u32 y, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing a pixelized line from one point to another */
void draw_line(u32 x0, u32 y0, u32 x1, u32 y1, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing the horisontal line */
void draw_hor_line(u32 x0, u32 y0, u32 length, u32 width, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing the vertical line */
void draw_ver_line(u32 x0, u32 y0, u32 length, u32 width, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing the simple rectangle */
void draw_rect(u32 x0, u32 y0, u32 width, u32 height,
               u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing the rectangle with boarder */
void draw_rect_with_brd(u32 x0, u32 y0, u32 width, u32 height, u32 brd_width, u32 color, u32 brd_color,
                        Render_Buffer_t *render_buffer);

/* Function for drawing the rectangle rotated by angle degrees */
void draw_rotated_rect(u32 x0, u32 y0, u32 width, u32 height, f32 angle, u32 color,
                       Render_Buffer_t *render_buffer);

/* Function for drawing the filled triangle */
void draw_fill_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing a circle (using Brezenhame algorithm) */
void draw_circle(u32 x0, u32 y0, u32 radius, b32 is_filled ,u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing a single digit symbol */
/* @ Rewise later */
void draw_digit(u32 x, u32 y, u8 digit, u32 size, u32 color, Render_Buffer_t *render_buffer);

/* Function for drawing an insigned int number with pixelized digits */
/* @ Rewise later */
void draw_number(u32 x, u32 y, u32 number, u32 size, u32 color, Render_Buffer_t *render_buffer);

/* Function for rendering the loaded png image */
void draw_bitmap(u32 x, u32 y, Loaded_img_t* image, u32 scale, Render_Buffer_t *render_buffer);

/* Function for rendering the colored object determined by mask */
void draw_object_by_mask(u32 x, u32 y, Loaded_img_t *mask, u32 color, u32 scale,
                         Render_Buffer_t *render_buffer);

/* Function to convert RGBA color to the ARGB color */
u32 convert_RGBA_to_ARGB(u32 color);

/* Function to get the alpha of the color */
u8 get_color_alpha(u32 color);

/* Function for setting the alpha channel to the color */
void set_color_alpha(u32 *color, u8 alpha);

/* Function to get the red channel of the color */
u8 get_color_red(u32 color);

/* Function for setting the red channel to the color */
void set_color_red(u32 *color, u8 red);

/* Function to get the green channel of the color */
u8 get_color_green(u32 color);

/* Function for setting the green channel to the color */
void set_color_green(u32 *color, u8 green);

/* Function to get the blue channel of the color */
u8 get_color_blue(u32 color);

/* Function for setting the blue channel to the color */
void set_color_blue(u32 *color, u8 blue);

/* Function to set the color in ARGB format */
void set_color(u32 *color, u8 alpha, u8 red, u8 green, u8 blue);

/* Function to get the transparent color (linear interpolation algorithm) */
u32 get_alpha_blending_color(u32 color_1, u32 color_2, f32 alpha);

/* Function to get the gray color */
u32 get_gray_color(u8 value);

#endif /* SOFTWARE_RENDERING_H_ */

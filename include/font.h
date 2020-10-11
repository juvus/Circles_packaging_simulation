/*================================================================================*/
/* Header file for the font class                                                 */                                   
/*================================================================================*/

#ifndef FONT_H_
#define FONT_H_

#include <utils.h>
#include <file_io.h>

/* Structure to store the symbol information */
struct Symbol_data {
    u8 symbol; /* ASCII representation of the symbol (for 32 - 126 codes)*/
    u8 symbol_array[42]; /* Array to storing the symbol pixel data */
    u8 shift_left; /* How much to shift the next symbol to the left */
    u8 shift_bottom; /* How much to thift the symbol to bottom */
};
typedef struct Symbol_data Symbol_data_t;

/* Function to exctract symbols from the font.png image */
void font_extract_symbols(Symbol_data_t *font_symbols, Loaded_img_t *font_img);

/* Function to render the string or part of the string */
void font_draw_string(char *str, s32 str_max_width, u32 x, u32 y, u32 size, u32 color,
                      Symbol_data_t *font_symbols, Render_Buffer_t *render_buffer);

#endif // FONT_H_

/*================================================================================*/
/* Realization of font methods                                                    */
/*================================================================================*/

/* Standard incudes: */

/* Program includes: */
#include <font.h>
#include <utils.h>
#include <file_io.h>
#include <software_rendering.h>

/* Global static variables */
static const u32 SYM_ROWS = 6; /* Amount of symbol rows in font.png */
static const u32 SYM_COLS = 20; /* Amount of sumbol columns in font.png */

/* Realization of the interface functions */
void
font_extract_symbols(Symbol_data_t *font_symbols, Loaded_img_t *font_img)
{
    /* Function to exctract symbols from the font.png image */

    u32 color;
    s32 ASCII_code; /* Current ASCII code of the reading symbol */
    s32 sym_num = 0; /* Number of reading symbol (0 is the Upper-Left symbol) */
    s32 i_0, j_0; /* Pixel coordinates of the BL corner of the symbol */
    s32 i, j; /* Coordinates of the pixels in downloaded image */
    s32 sym_i, sym_j; /* Coordinates of symbols in the symbols table in font_img */
    s32 index; /* Index of the point in font_img pixel array */
    s32 p; /* Index of the symbol_array in font_symbols */
    s32 width_max; /* Maximum width of the current symbol */
    s32 width_on_line; /* Width of the symbol on current line */
    
    for (sym_i = (SYM_ROWS - 1); sym_i >= 0; --sym_i) {
        for (sym_j = 0; sym_j < SYM_COLS; ++sym_j) {

            /* Determine the current symbol number (0 is the Upper-Left symbol) */
            sym_num = (SYM_ROWS * SYM_COLS) - sym_i * SYM_COLS - (SYM_COLS - sym_j);

            /* Determine the ASCII equivalent code for the symbol */
            ASCII_code = 32 + sym_num;
            font_symbols[sym_num].symbol = (u8)ASCII_code;

            /* determine the pixels coordinate of the BL corner of the symbol */
            i_0 = font_img->height - 7 - 8 * (sym_num / SYM_COLS);
            j_0 = 7 * (sym_num % SYM_COLS);

            /* Copying the symbol from font_img to font_symbols */
            p = 0;
            width_max = 0;
            for (i = i_0; i < (i_0 + 7); ++i) {
                width_on_line = 0;
                for (j = j_0; j < (j_0 + 6); ++j) {

                    /* Index of the pixel in font_img pixel array */
                    index = i * font_img->width + j;

                    /* Write the color to font_symbols array */
                    color = convert_RGBA_to_ARGB(*(font_img->data + index));

                    if (color == 0xff000000) { /* Black color */
                        font_symbols[sym_num].symbol_array[p] = (u8)1;
                        width_on_line = j - j_0 + 1;
                    }
                    else { /* Other color (light yellow in the image) */
                        font_symbols[sym_num].symbol_array[p] = (u8)0;
                    }

                    /* Save the maximum line width of the symbol */
                    if (width_on_line > width_max) {
                        width_max = width_on_line;
                    }
                    p++;
                }
            }
            
            /* Calculate the shift_left */
            if (width_max == 0) { /* Only for the Space symbol */
                font_symbols[sym_num].shift_left = 3;
            }
            else {
                font_symbols[sym_num].shift_left = 6 - width_max;
            }

            /* Calculate the shift_bottom for several symbols */
            switch (font_symbols[sym_num].symbol) {
            case 'g': case 'p': case 'q': case 'y': {
                font_symbols[sym_num].shift_bottom = 2;
                break;
            }
            case 'j': {
                font_symbols[sym_num].shift_bottom = 1;
                break;
            }
            default:
                font_symbols[sym_num].shift_bottom = 0;
                break;
            }
        }
    }
}

void
font_draw_string(char *str, s32 str_max_width, u32 x, u32 y, u32 size, u32 color,
                 Symbol_data_t *font_symbols, Render_Buffer_t *render_buffer)
{
    /* Function to render the string or part of the string */

    u32 x_init; /* Initial x position of the very first symbol */
    s32 str_width; /* Current width of the string in pixels */
    u32 char_index; /* Index of the char in str */
    s32 p; /* Index of the symbol_array in font_symbols */
    u32 i, j; /* Coordinates of pixels in symbol box  */
    u32 x_pos, y_pos; /* Coordinates of pixels taking into account the pixel size */
    Symbol_data_t *font_symbol; /* Current symbol data */
    b32 stop_print; /* Flag to stop print the symbols */

    x_init = x;
    char_index = 0;
    str_width = 0;
    stop_print = false;
    
    while (str[char_index] != '\0') {

        if ((str_max_width > 0) && (str_width >= str_max_width - 7)) {
            /* Print "..." and then stop */
            font_symbol = &(font_symbols[95]);
            stop_print = true;
        }
        else {
            font_symbol = &(font_symbols[(str[char_index] - 32)]);
        }

        /* Add necessary bottom shift */
        y -= (font_symbol->shift_bottom) * size;
        
        p = 0;
        for (i = 0; i < 7; ++i) {
            for (j = 0; j < 6; ++j) {

                x_pos = x + j * size;
                y_pos = y + i * size;

                if (font_symbol->symbol_array[p]) {
                    draw_rect(x_pos, y_pos, size, size, color, render_buffer);
                }
                p++;
            }
        }

        /* Restore the normal y position */
        y += (font_symbol->shift_bottom) * size;

        /* Determine the x position of the next symbol in str */
        x += (7 - font_symbol->shift_left) * size;
        str_width = x - x_init;
        char_index++;
        
        /* Check to exit the printing procedure */
        if (stop_print) {
            break;
        }
    }  
}

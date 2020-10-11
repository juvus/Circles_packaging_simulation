#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <utils.h>

/* Structure for the string data (inclueds string and size of the string) */
struct String_data {
    char *data;
    u64 size;
};
typedef struct String_data String_data_t; 

/* Structure for the loaded uncompressed image */
struct Loaded_img {
    String_data_t raw_data; /* Data for compressed loaded png file */
    u32 *data; /* Uncompressed data for image */
    u32 width; /* Width of the uncompresed image */
    u32 height; /* Height of the uncompressed image */
    u32 channels; /* Number of channeld of uncompressed image (4) */
};
typedef struct Loaded_img Loaded_img_t;

/* Structure for the color */
struct Color_bytes {
    union {
        struct {
            u8 r; u8 g; u8 b; u8 a;
        };
        u32 color_word;
    };
};
typedef struct Color_bytes Color_bytes_t;

/* Function to read the entire file */
String_data_t read_file_to_memory(char *file_path);

/* Function to uncopress the loaded png image */
void uncompress_png_image(Loaded_img_t *image);

#endif // FILE_IO_H_


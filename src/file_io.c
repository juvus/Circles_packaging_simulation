/* Standard incudes: */
#include <windows.h>
#include <assert.h>

/* Program includes: */
#include <file_io.h>
#include <utils.h>

#define STB_IMAGE_IMPLEMENTATION /* Necessary definition for the stb_image.h */
#define STBI_ONLY_PNG /* Use only PNG images */
#define STBI_NO_STDIO /* Don't use stdio.h */
#define STBI_NO_FAILURE_STRINGS
#include <stb_image.h> /* single file library stb_image*/

String_data_t
read_file_to_memory(char *file_path)
{
    /* Function to read the entire file to the memory for further processing */
    String_data_t result = {0};
    HANDLE file_handle;
    DWORD file_size;
    DWORD bytes_read;
    
    /* Create the file handle */
    file_handle = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle == INVALID_HANDLE_VALUE) {
        assert(0);
        return result;
    }

    /* Determine the file size */
    file_size = GetFileSize(file_handle, 0);
    result.size = file_size;

    /* Allocate the memory buffer for the file data  */
    result.data = VirtualAlloc(0, file_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    
    /* Read the file to the memory */
    if ((ReadFile(file_handle, result.data, file_size, &bytes_read, 0))
        && (file_size == bytes_read)) {
        /* Successful file reading */
    }
    else {
        assert(0);
    }
    return result;
}

void
uncompress_png_image(Loaded_img_t *image)
{
    /* Function to uncopress the loaded png image */

    String_data_t raw_data; /* Compressed png image data */
    u64 size; /* Compressed png image size */
    int img_width; /* Width of the uncomressed image */
    int img_height; /* Height of the uncompressed image */
    int img_channels; /* Number of channels of uncompressed image */
    u32 *img_ptr; /* Pointer to the uncompressed image im memory */
    
    raw_data = image->raw_data;
    size = image->raw_data.size;

    /* Convert compressed png image to uncompressed */
    stbi_set_flip_vertically_on_load(1); /* start from the BL */
    img_ptr = (u32*)stbi_load_from_memory((void*)raw_data.data, (int)size, &img_width,
                                          &img_height, &img_channels, 4);

    image->data = img_ptr;
    image->width = (u32)img_width;
    image->height = (u32)img_height;
    image->channels = (u32)img_channels;   
}

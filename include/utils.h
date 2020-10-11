#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include <windows.h>

/* Type synonims for easy use */
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef int b32;
typedef float f32;
typedef double f64;

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 720

// Macro to suppress "unused function parameter"
#define UNUSED(x) (void)(x)

/* Boolean synonims */
#define true 1
#define false 0

/* Vector of two f32 elements with iterative possibility */
struct V2_f32 {
    union {
        struct {
            f32 x;
            f32 y;
        };
        f32 e[2];
    };
};
typedef struct V2_f32 V2_f32_t;

/* Vector of two u32 elements with iterative possibility */
struct V2_u32 {
    union {
        struct {
            u32 x;
            u32 y;
        };
        u32 e[2];
    };
};
typedef struct V2_u32 V2_u32_t;

/* Vector of two s32 elements with iterative possibility */
struct V2_s32 {
    union {
        struct {
            s32 x;
            s32 y;
        };
        s32 e[2];
    };
};
typedef struct V2_s32 V2_s32_t;

/* Addition of two vectors of V2_f32 type */
inline V2_f32_t add_V2_f32(V2_f32_t a, V2_f32_t b) {
    V2_f32_t result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

/* Element-wise multiplication of vector V2_f32 with number */
inline V2_f32_t mul_V2_f32(V2_f32_t a, f32 num) {
    V2_f32_t result;
    result.x = a.x * num;
    result.y = a.y * num;
    return result;
}

/* Structure for render buffer */
struct Render_Buffer {
    /* Platform non-specific part */
    u32 width;
    u32 height;
    u32 *pixels;
    /* Platform specific part */
    BITMAPINFO bitmap_info;
};
typedef struct Render_Buffer Render_Buffer_t;

/* Enumerator for button treatment */
enum Btn_names {
    BUTTON_1, /* Button for start the packaging simulation */
    BUTTON_2, /* Button for pause the packaging simulation */
    BUTTON_COUNT
};
typedef enum Btn_names Btn_names_t;

/* Structure for storing the key button state in nice form */
struct Button {
    u32 vk_code;
    b32 is_pressed;
    b32 is_changed;
};
typedef struct Button Button_t;

/* Structure for input from mouse and keyboard */
struct Input {
    Button_t buttons[BUTTON_COUNT];
};
typedef struct Input Input_t;

#define pressed_smooth(b) (user_input->buttons[b].is_pressed)
#define pressed_iterative(b) (user_input->buttons[b].is_pressed && user_input->buttons[b].is_changed)

#endif // UTILS_H_

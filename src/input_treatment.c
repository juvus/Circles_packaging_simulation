/* Standard incudes: */
#include <windows.h>
#include <stdio.h>

/* Program includes: */
#include <input_treatment.h>
#include <utils.h>

#ifndef VK_KEY_1
#define VK_KEY_1 0x31
#endif

#ifndef VK_KEY_2
#define VK_KEY_2 0x32
#endif

/* Function for prepare the particular key */
static void prepare_key(u32 vk_code, b32 was_down, b32 is_down, int key, int button, Input_t *user_input);

void prepare_user_input(u32 vk_code, b32 was_down, b32 is_down, Input_t *user_input) {
    /* Function for the raw input treatment and preparing user input
       List of virtual keys: http://www.kbdedit.com/manual/low_level_vk_list.html */

    prepare_key(vk_code, was_down, is_down, VK_KEY_1, BUTTON_1, user_input);
    prepare_key(vk_code, was_down, is_down, VK_KEY_2, BUTTON_2, user_input);
}

void reset_changed_state(Input_t *user_input) {
    /* Reset the is_changed state of all buttons */
    int i;
    for (i = 0; i < BUTTON_COUNT; i++) {
        user_input->buttons[i].is_changed = false;
    }
}

static void prepare_key(u32 vk_code, b32 was_down, b32 is_down, int key, int button, Input_t *user_input) {
    /* Function for prepare the particular key */
    b32 new_state;
    b32 is_changed = false;

    if (vk_code == key) {
        /* If button is just pressed (1, 0) */
        if ((is_down) && (!was_down)) {
            new_state = true;
            is_changed = true;
        }
        /* If button is just released (0, 1) */
        if ((!is_down) && (was_down)) {
            new_state = false;
        }
        /* If button is pressed and holding (1, 1) */
        if ((is_down) && (was_down)) {
            new_state = true;
            is_changed = true;
        }
        /* Update the is_pressed button state */
        user_input->buttons[button].is_pressed = new_state;
        user_input->buttons[button].is_changed = is_changed;
    }
}

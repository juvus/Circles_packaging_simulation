#ifndef INPUT_TREATMENT_H_
#define INPUT_TREATMENT_H_

#include <utils.h>

/* Function for the raw input treatment and preparing user input */
void prepare_user_input(u32 vk_code, b32 was_down, b32 is_down, Input_t *user_input);

/* Reset the is_changed state of all buttons */
void reset_changed_state(Input_t *user_input);

#endif // INPUT_TREATMENT_H_

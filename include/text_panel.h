/*================================================================================*/
/* Header file for the text_panel class.                                          */
/*================================================================================*/
#ifndef TEXT_PANEL_H_
#define TEXT_PANEL_H_

#include <utils.h>
#include <software_rendering.h>
#include <world.h>
#include <font.h>
#include <box.h>

/* Structure of the text_panel */
struct Text_panel {
    V2_u32_t BL; /* Coordinates of the bottom-left corner */
    V2_u32_t UR; /* Coordinates of the up-right corner */
    u32 width; /* Width of the panel */
    u32 height; /* Height of the panel */
    u32 bkg_clr; /* Color of panel background */
    u32 shadow_clr; /* Color of the panel inner shadow */
    u32 text_clr; /* Color of panel text */
    char txt_title[50]; /* Title text */
    char txt_help_start_sim[50]; /* Text for help to start the simulation */
    char txt_help_pause_sim[50]; /* Text for help to pause the simulation */
    char txt_sim_status_label[50]; /* Label text for current simulation status */
    char txt_sim_status_value[50]; /* Value text for current simulation status */
    char txt_packed_circles_label[50]; /* Label text for packed circles information */
    char txt_packed_circles_value[50]; /* Value text for packed circles information */
};
typedef struct Text_panel Text_panel_t;

/* Methods of text_panel class */
/* Method for creation of the text panel object (malloc) */
Text_panel_t* text_panel_create(void);

/* Method for deleting the text_panel object */
void text_panel_delete(Text_panel_t *text_panel);

/* Initialization of the text_panel object fields */
void text_panel_init(Text_panel_t *text_panel, const World_t *world, const Box_t *box, const u32 TEXT_PANEL_WIDTH,
                     const u32 TEXT_PANEL_HEIGHT, const u32 TEXT_PANEL_BKG_CLR, const u32 TEXT_PANEL_SHADOW_CLR,
                     const u32 TEXT_PANEL_TEXT_CLR);

/* Method for update the simulation status text */
void text_panel_set_sim_status_value(Text_panel_t *text_panel, Sim_Mode_t sim_mode);

/* Method for update the packed circles number text */
void text_panel_set_packed_circles_value(Text_panel_t *text_panel, u32 number);

/* Method for render the text_panel */
void text_panel_render(Text_panel_t *text_panel, Symbol_data_t *font_symbols, Render_Buffer_t *render_buffer);

#endif //TEXT_PANEL_H

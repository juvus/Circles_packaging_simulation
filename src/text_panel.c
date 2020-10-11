/*================================================================================*/
/* Realization of text_panel methods                                              */
/*================================================================================*/

#define _USE_MATH_DEFINES

/* Standard incudes: */
#include <stdio.h>

/* Program includes: */
#include <text_panel.h>
#include <utils.h>
#include <software_rendering.h>
#include <world.h>
#include <font.h>
#include <box.h>

Text_panel_t*
text_panel_create(void)
{
    /* Method for creation of the text_panel object (malloc) */
    Text_panel_t* tmp_text_panel;

    /* Allocate the memory for the text_panel */
    tmp_text_panel = (Text_panel_t*) calloc (1, sizeof(Text_panel_t));
    return tmp_text_panel;    
}

void
text_panel_delete(Text_panel_t *text_panel)
{
    /* Method for deleting the text_panel object */
    free(text_panel);    
}

void
text_panel_init(Text_panel_t *text_panel, const World_t *world, const Box_t *box,
                const u32 TEXT_PANEL_WIDTH, const u32 TEXT_PANEL_HEIGHT, const u32 TEXT_PANEL_BKG_CLR,
                const u32 TEXT_PANEL_SHADOW_CLR, const u32 TEXT_PANEL_TEXT_CLR)
{
    /* Initialization of the text_panel object fields */
    u32 tmp; /* Temporary parameter */

    /* Initialization of the text_panel object fields (constructor) */
    text_panel->width = TEXT_PANEL_WIDTH;
    text_panel->height = TEXT_PANEL_HEIGHT;
    text_panel->bkg_clr = TEXT_PANEL_BKG_CLR;
    text_panel->shadow_clr = TEXT_PANEL_SHADOW_CLR;
    text_panel->text_clr = TEXT_PANEL_TEXT_CLR;
    
    text_panel->BL.x = (world->width - text_panel->width) / 2;
    tmp = (world->height - box->height_out - text_panel->height) / 3;
    text_panel->BL.y = tmp *2 + box->height_out;
    text_panel->UR.x = text_panel->BL.x + text_panel->width;
    text_panel->UR.y = text_panel->BL.y + text_panel->height;

    sprintf_s(text_panel->txt_title, 50, "Circles packaging simulation");
    sprintf_s(text_panel->txt_help_start_sim, 50, "For start the simulation, press \"1\"");
    sprintf_s(text_panel->txt_help_pause_sim, 50, "For pause the simulation, press \"2\"");
    sprintf_s(text_panel->txt_sim_status_label, 50, "Simulation status: ");
    text_panel_set_sim_status_value(text_panel, SIM_STOPPED);
    sprintf_s(text_panel->txt_packed_circles_label, 50, "Packed particles: ");
    text_panel_set_packed_circles_value(text_panel, 0);    
}

void
text_panel_set_sim_status_value(Text_panel_t *text_panel, Sim_Mode_t sim_mode)
{
    /* Method for update the simulation status text */

    switch (sim_mode) {
    case SIM_STOPPED: {
        sprintf_s(text_panel->txt_sim_status_value, 50, "Stopped");
        break;
    }
    case SIM_NORMAL: {
        sprintf_s(text_panel->txt_sim_status_value, 50, "In process");
        break;
    }
    case SIM_PAUSED: {
        sprintf_s(text_panel->txt_sim_status_value, 50, "Paused");
        break;
    }}
}

void
text_panel_set_packed_circles_value(Text_panel_t *text_panel, u32 number)
{
    /* Method for update the packed circles number text */
    sprintf_s(text_panel->txt_packed_circles_value, 50, "%d", number);
}

void
text_panel_render(Text_panel_t *text_panel, Symbol_data_t *font_symbols, Render_Buffer_t *render_buffer)
{
    /* Method for render the text_panel */
    u32 x = text_panel->BL.x + 15;
    u32 y = text_panel->UR.y - 30;
    u32 font_size = 2;
    
    /* Render the background panel */
    draw_rect_with_brd(text_panel->BL.x, text_panel->BL.y, text_panel->width, text_panel->height,
                       2, text_panel->bkg_clr, text_panel->shadow_clr, render_buffer);

    /* Render text labels */
    /* Text of the title */
    x += 40;
    font_draw_string(text_panel->txt_title, -1, x, y, font_size, 0xee0000, font_symbols, render_buffer);

    /* Text of the help with starting the simulation */
    x -= 40; y -= 25;
    font_draw_string(text_panel->txt_help_start_sim, -1, x, y, font_size, text_panel->text_clr,
                     font_symbols, render_buffer);

    /* Text of the help with pausing the simulation */
    y -= 25;
    font_draw_string(text_panel->txt_help_pause_sim, -1, x, y, font_size, text_panel->text_clr,
                     font_symbols, render_buffer);

    /* Text with the label of current simulatin status */
    y -= 25;
    font_draw_string(text_panel->txt_sim_status_label, -1, x, y, font_size, text_panel->text_clr,
                     font_symbols, render_buffer);

    /* Text with the value of current simulation status */
    x += 180;
    font_draw_string(text_panel->txt_sim_status_value, -1, x, y, font_size, 0x0000ee, font_symbols,
                     render_buffer);

    /* Text with the label of packed circles so far */
    x -= 180; y -= 25;
    font_draw_string(text_panel->txt_packed_circles_label, -1, x, y, font_size, text_panel->text_clr,
                     font_symbols, render_buffer);

    /* Text with the value of packed circles so far */
    x += 180;
    font_draw_string(text_panel->txt_packed_circles_value, -1, x, y, font_size, 0x0000ee,
                     font_symbols, render_buffer);
}

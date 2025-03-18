#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "lvgl/lvgl.h"
#include "lvgl/src/drivers/wayland/lv_wayland.h"

#define H_RES (800)
#define V_RES (600)

// SIGTERM handler
static void sigterm_handler(int signum)
{
    exit(0);
}

static void * tick_thread(void * data)
{
    (void) data;
    while(true) {
        usleep(5 * 1000);
        lv_tick_inc(5);
    }
}

int main(int argc, char **argv)
{
    printf("Starting...\n");

    // Set SIGTERM handler
    signal(SIGTERM, sigterm_handler);

    // LVGL init
    lv_init();

    // lv_wayland_init();
    /* Create a display */
    lv_disp_t * disp = lv_wayland_window_create(H_RES, V_RES, "Demo LVGL", NULL);

    lv_wayland_window_set_fullscreen(disp, false);

    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include "lvgl/lvgl.h"
#include "lvgl/src/drivers/wayland/lv_wayland.h"
#include "examples.h"

#define H_RES (800)
#define V_RES (600)

static pthread_t tick_thread_id;

// SIGTERM handler
static void sigterm_handler(int signum)
{
    printf("Received SIGTERM\n");
    pthread_cancel(tick_thread_id);
    exit(0);
}

static void * tick_thread(void * data)
{
    (void) data;
    printf("Tick thread started\n");
    while(true) {
        usleep(5 * 1000);
        lv_tick_inc(5);
    }
    printf("Tick thread ended\n");
    return NULL;
}

int main(int argc, char **argv)
{
    printf("Starting...\n");

    // Set SIGTERM handler
    signal(SIGTERM, sigterm_handler);// kill -15 <pid>

    // LVGL init
    lv_init();

    // lv_wayland_init();
    /* Create a display */
    LV_LOG_INFO("Creating display");
    lv_disp_t * disp = lv_wayland_window_create(H_RES, V_RES, "Demo LVGL", NULL);
    if(disp == NULL) {
        LV_LOG_ERROR("Failed to create display");
        return 1;
    }

    // lv_wayland_window_set_fullscreen(disp, false);

    /*Change the active screen's background color*/
    LV_LOG_INFO("Setting screen background color");
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

    // Demo part. Change called method according to the desired example.
    simple_label();

    pthread_create(&tick_thread_id, NULL, tick_thread, NULL);

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        // lv_timer_handler();
        lv_wayland_timer_handler();
        usleep(5000);
    }
    
    return 0;
}
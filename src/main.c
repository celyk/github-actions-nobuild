#include <stdio.h>

#include "platform_defs.h"


// #if defined(PLATFORM_MACOS)
// #define SOKOL_METAL
// #elif defined(PLATFORM_IOS)
// #define SOKOL_GLCORE
// #elif defined(PLATFORM_WEB)
// #define SOKOL_GLES3
// #else
// #define SOKOL_GLCORE
// #endif

#define SOKOL_IMPL
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_glue.h"
#include "sokol/sokol_log.h"

void init() {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });
}

void frame() {
    sg_begin_pass(&(sg_pass){
        .action.colors[0] = {
            .load_action = SG_LOADACTION_CLEAR,
            .clear_value = { 0.25f, 0.5f, 0.75f, 1.0f }
        },
        .swapchain = sglue_swapchain()
    });
    
    sg_end_pass();
    sg_commit();
}

void cleanup() {
    
}

void input(const sapp_event* event) {
    
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = input,
        .width = 800,
        .height = 600,
        .sample_count = 4,
        .window_title = "cube-sapp.c",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
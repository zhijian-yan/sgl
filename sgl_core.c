// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_core.h"
#include "sgl_private.h"
#include <string.h>

sgl_screen_handle_t sgl_create_screen(void *buffer, uint32_t buffer_size,
                                      uint32_t hor_res, uint32_t ver_res) {
    sgl_screen_handle_t screen;
    if (!buffer)
        return NULL;
    screen = sgl_malloc(sizeof(struct sgl_screen));
    if (!screen)
        return NULL;
    memset(screen, 0, sizeof(struct sgl_screen));
    screen->buffer = buffer;
    screen->buffer_size = buffer_size;
    screen->hor_res = hor_res;
    screen->ver_res = ver_res;
    screen->max_x = hor_res - 1;
    screen->max_y = ver_res - 1;
    screen->visible.left = 0;
    screen->visible.top = 0;
    screen->visible.right = screen->max_x;
    screen->visible.bottom = screen->max_y;
    screen->invalidate = screen->visible;
    screen->rotate = SGL_ROTATE_DEFAULT;
    screen->fcount = 0;
    return screen;
}

void sgl_delete_screen(sgl_screen_handle_t new_screen) {
    if (__act_scr)
        sgl_free(__act_scr);
    __act_scr = new_screen;
}

void sgl_handler(void) {
    __act_scr->paint();
    __act_scr->flush(__act_scr->buffer, __act_scr->buffer_size);
    ++__act_scr->fcount;
}

void sgl_set_screen(sgl_screen_handle_t screen) {
    if (screen)
        __act_scr = screen;
}

void sgl_set_buffer(void *buffer, uint32_t buffer_size) {
    if (buffer) {
        __act_scr->buffer = buffer;
        __act_scr->buffer_size = buffer_size;
    }
}

void sgl_set_paint(void (*paint)()) { __act_scr->paint = paint; }

void sgl_set_flush(void (*flush)(void *buffer, uint32_t buffer_size)) {
    __act_scr->flush = flush;
}

void sgl_set_draw_pixel(void (*draw_pixel)(int32_t x, int32_t y,
                                           uint32_t color)) {
    __act_scr->draw_pixel = draw_pixel;
}

void sgl_set_visible(int32_t left, int32_t top, int32_t right, int32_t bottom) {
    if (left < 0 || top < 0 || right > __act_scr->max_x ||
        bottom > __act_scr->max_y)
        return;
    __act_scr->visible.left = left;
    __act_scr->visible.top = top;
    __act_scr->visible.right = right;
    __act_scr->visible.bottom = bottom;
}

uint32_t sgl_get_fcount(void) {
    if (__act_scr)
        return __act_scr->fcount;
    return 0;
}

void sgl_reset_fcount(void) {
    if (__act_scr)
        __act_scr->fcount = 0;
}

void sgl_clear_screen(uint8_t value) {
    memset(__act_scr->buffer, value, __act_scr->buffer_size);
}

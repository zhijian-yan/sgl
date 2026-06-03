// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_core.h"
#include "sgl_common.h"
#include "sgl_pixel.h"
#include <string.h>

int sgl_init(sgl_screen_t *scr, void *buffer, uint32_t buffer_size,
             uint32_t hor_res, uint32_t ver_res) {
    if (!scr || !buffer)
        return -1;
    memset(scr, 0, sizeof(sgl_screen_t));
    scr->buffer = buffer;
    scr->buffer_size = buffer_size;
    scr->hor_res = hor_res;
    scr->ver_res = ver_res;
    sgl_set_draw_pixel(scr, sgl_draw_pixel_mono);
    sgl_set_screen_rotation(scr, SGL_ROTATE_DEFAULT);
    scr->invalidate = scr->visible;
    return 0;
}

void sgl_handler(sgl_screen_t *scr) {
    scr->paint(scr);
    scr->flush(scr->buffer, &scr->visible);
    ++scr->fcount;
}

void sgl_set_buffer(sgl_screen_t *scr, void *buffer, uint32_t buffer_size) {
    scr->buffer = buffer;
    scr->buffer_size = buffer_size;
}

void sgl_set_paint(sgl_screen_t *scr, void (*paint)(sgl_screen_t *scr)) {
    scr->paint = paint;
}

void sgl_set_flush(sgl_screen_t *scr,
                   void (*flush)(void *buffer, sgl_rect_t *visible)) {
    scr->flush = flush;
}

void sgl_set_draw_pixel(sgl_screen_t *scr,
                        void (*draw_pixel)(sgl_screen_t *scr, int32_t x,
                                           int32_t y, uint32_t color)) {
    scr->draw_pixel = draw_pixel;
}

void sgl_set_visible(sgl_screen_t *scr, int32_t left, int32_t top,
                     int32_t right, int32_t bottom) {
    if (left < 0)
        left = 0;
    if (top < 0)
        top = 0;
    if (right > scr->max_x)
        right = scr->max_x;
    if (bottom > scr->max_y)
        bottom = scr->max_y;
    sgl_set_rect(&scr->visible, left, top, right, bottom);
}

void sgl_reset_visible(sgl_screen_t *scr) {
    sgl_set_rect(&scr->visible, 0, 0, scr->max_x, scr->max_y);
}

void sgl_set_screen_rotation(sgl_screen_t *scr, sgl_rotate_t rotate) {
    scr->rotate = rotate;
    switch (rotate) {
    case SGL_ROTATE_0:
    case SGL_ROTATE_180:
        scr->max_x = scr->hor_res - 1;
        scr->max_y = scr->ver_res - 1;
        break;
    case SGL_ROTATE_90:
    case SGL_ROTATE_270:
        scr->max_x = scr->ver_res - 1;
        scr->max_y = scr->hor_res - 1;
        break;
    }
    sgl_set_rect(&scr->visible, 0, 0, scr->max_x, scr->max_y);
}

uint32_t sgl_get_fcount(sgl_screen_t *scr) { return scr->fcount; }

void sgl_reset_fcount(sgl_screen_t *scr) { scr->fcount = 0; }

void sgl_clear_screen(sgl_screen_t *scr, uint8_t value) {
    memset(scr->buffer, value, scr->buffer_size);
}

// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "../inc/sgl_core.h"
#include "../inc/sgl_common.h"
#include "../inc/sgl_pixel.h"
#include <string.h>

static void sgl_set_color_format(sgl_screen_t *scr, uint32_t buffer_size,
                                 sgl_color_format_t color_format);

int sgl_init(sgl_screen_t *scr, void *buffer, uint32_t buffer_size,
             sgl_color_format_t color_format, uint32_t hor_res,
             uint32_t ver_res) {
    if (!scr || !buffer)
        return -1;
    memset(scr, 0, sizeof(sgl_screen_t));
    scr->buffer = buffer;
    scr->hor_res = hor_res;
    scr->ver_res = ver_res;
    sgl_set_color_format(scr, buffer_size, color_format);
    sgl_set_screen_rotation(scr, SGL_ROTATE_DEFAULT);
    scr->dirty = scr->drawable;
    return 0;
}

static void sgl_buffer_slice(sgl_screen_t *scr) {
    uint32_t w_cake, h_cake, w_piece, h_piece;
    switch (scr->slice_status) {
    case SGL_SLICE_STATUS_IDLE:
    case SGL_SLICE_STATUS_START:
        scr->slice_status = SGL_SLICE_STATUS_RUNNIG;
        scr->slice_count = 0;
        scr->w_dirty = scr->dirty.right - scr->dirty.left + 1;
        scr->h_dirty = scr->dirty.bottom - scr->dirty.top + 1;
    case SGL_SLICE_STATUS_RUNNIG:
        scr->offset_x = scr->dirty.left;
        scr->offset_y = scr->dirty.top + scr->slice_count;
        w_cake = scr->w_dirty;
        h_cake = scr->h_dirty;
        w_piece = w_cake;
        h_piece = scr->pixel_num / w_cake;
        h_cake -= scr->slice_count;
        if (h_piece > h_cake)
            h_piece = h_cake;
        scr->slice_count += h_piece;
        if (scr->slice_count == scr->h_dirty)
            scr->slice_status = SGL_SLICE_STATUS_IDLE;
        sgl_set_rect(&scr->slice, scr->offset_x, scr->offset_y,
                     scr->offset_x + w_piece - 1, scr->offset_y + h_piece - 1);
        scr->drawable = scr->slice;
        break;
    }
}

static void sgl_rotate_drawable_rect(sgl_screen_t *scr) {
    int32_t temp;
    sgl_rotate_point_cw(scr, &scr->drawable.left, &scr->drawable.top);
    sgl_rotate_point_cw(scr, &scr->drawable.right, &scr->drawable.bottom);
    if (scr->drawable.left > scr->drawable.right) {
        temp = scr->drawable.left;
        scr->drawable.left = scr->drawable.right;
        scr->drawable.right = temp;
    }
    if (scr->drawable.top > scr->drawable.bottom) {
        temp = scr->drawable.top;
        scr->drawable.top = scr->drawable.bottom;
        scr->drawable.bottom = temp;
    }
}

void sgl_handler(sgl_screen_t *scr) {
    sgl_buffer_slice(scr);
    // sgl_rotate_drawable_rect(scr);
    scr->draw(scr);
    scr->flush(scr->buffer, &scr->slice);
    if (scr->slice_status == SGL_SLICE_STATUS_IDLE)
        ++scr->frame_count;
}

void sgl_set_draw(sgl_screen_t *scr, void (*draw)(sgl_screen_t *scr)) {
    scr->draw = draw;
}

void sgl_set_flush(sgl_screen_t *scr,
                   void (*flush)(void *buffer, sgl_rect_t *refresh)) {
    scr->flush = flush;
}

int sgl_set_draw_pixel(sgl_screen_t *scr,
                       void (*draw_pixel)(sgl_screen_t *scr, int32_t x,
                                          int32_t y, uint32_t color),
                       uint32_t buffer_size, uint32_t pixel_size) {
    if (buffer_size == 0 || pixel_size == 0)
        return -1;
    scr->draw_pixel = draw_pixel;
    scr->pixel_num = buffer_size / pixel_size;
    scr->buffer_size = scr->pixel_num * pixel_size;
    return 0;
}

static void sgl_set_color_format(sgl_screen_t *scr, uint32_t buffer_size,
                                 sgl_color_format_t color_format) {
    switch (color_format) {
    case SGL_COLOR_FORMAT_MONO:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_mono, buffer_size, 1);
        break;
    case SGL_COLOR_FORMAT_RGB332:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_rgb332, buffer_size, 1);
        break;
    case SGL_COLOR_FORMAT_RGB565:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_rgb565, buffer_size, 2);
        break;
    case SGL_COLOR_FORMAT_RGB565SWAP:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_rgb565swap, buffer_size, 2);
        break;
    case SGL_COLOR_FORMAT_BGR565:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_bgr565, buffer_size, 2);
        break;
    case SGL_COLOR_FORMAT_RGB888:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_rgb888, buffer_size, 3);
        break;
    case SGL_COLOR_FORMAT_BGR888:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_bgr888, buffer_size, 3);
        break;
    case SGL_COLOR_FORMAT_XRGB8888:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_xrgb8888, buffer_size, 4);
        break;
    case SGL_COLOR_FORMAT_XBGR8888:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_xbgr8888, buffer_size, 4);
        break;
    case SGL_COLOR_FORMAT_ARGB8888:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_argb8888, buffer_size, 4);
        break;
    case SGL_COLOR_FORMAT_ABGR8888:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_abgr8888, buffer_size, 4);
        break;
    case SGL_COLOR_FORMAT_RGBA8888:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_rgba8888, buffer_size, 4);
        break;
    case SGL_COLOR_FORMAT_BGRA8888:
        sgl_set_draw_pixel(scr, sgl_draw_pixel_bgra8888, buffer_size, 4);
        break;
    }
}

void sgl_set_drawable(sgl_screen_t *scr, int32_t left, int32_t top,
                      int32_t right, int32_t bottom) {
    if (left < 0)
        left = 0;
    if (top < 0)
        top = 0;
    if (right > scr->max_x)
        right = scr->max_x;
    if (bottom > scr->max_y)
        bottom = scr->max_y;
    sgl_set_rect(&scr->drawable, left, top, right, bottom);
}

void sgl_reset_drawable(sgl_screen_t *scr) {
    sgl_set_rect(&scr->drawable, 0, 0, scr->max_x, scr->max_y);
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
    sgl_set_rect(&scr->drawable, 0, 0, scr->max_x, scr->max_y);
}

uint32_t sgl_get_frame_count(sgl_screen_t *scr) { return scr->frame_count; }

void sgl_reset_frame_count(sgl_screen_t *scr) { scr->frame_count = 0; }

void sgl_clear_screen(sgl_screen_t *scr, uint8_t value) {
    memset(scr->buffer, value, scr->buffer_size);
}

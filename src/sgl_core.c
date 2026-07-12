// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "../inc/sgl_core.h"
#include "../inc/sgl_common.h"
#include "../inc/sgl_pixel.h"
#include <string.h>

static int sgl_set_color_format(sgl_screen_t *scr, uint32_t buffer_size,
                                sgl_color_format_t color_format);

int sgl_init(sgl_screen_t *scr, void *buffer, uint32_t buffer_size,
             sgl_color_format_t color_format, sgl_rotate_t rotate,
             uint32_t hor_res, uint32_t ver_res) {
    if (!scr || !buffer)
        return -1;
    memset(scr, 0, sizeof(sgl_screen_t));
    scr->buffer = buffer;
    scr->hor_res = hor_res;
    scr->ver_res = ver_res;
    if (sgl_set_color_format(scr, buffer_size, color_format))
        return -1;
    sgl_set_screen_rotation(scr, rotate);
    sgl_reset_dirty_area(scr);
    return 0;
}

static void sgl_buffer_slice(sgl_screen_t *scr) {
    uint32_t w_cake, h_cake, w_piece, h_piece;
    sgl_rect_t temp;
    if (scr->slice_state == SGL_SLICE_STATE_IDLE) {
        scr->slice_state = SGL_SLICE_STATE_START;
    }
    if (scr->slice_state == SGL_SLICE_STATE_START) {
        scr->slice_state = SGL_SLICE_STATE_RUNNIG;
        scr->slice_count = 0;
        sgl_area2rect(&scr->dirty_area, &scr->dirty_rect);
        sgl_rotate_rect_ccw(scr, &scr->dirty_rect.x, &scr->dirty_rect.y,
                            &scr->dirty_rect.w, &scr->dirty_rect.h);
    }
    if (scr->slice_state == SGL_SLICE_STATE_RUNNIG) {
        scr->offset_x = scr->dirty_rect.x;
        scr->offset_y = scr->dirty_rect.y + scr->slice_count;
        w_cake = scr->dirty_rect.w;
        h_cake = scr->dirty_rect.h;
        w_piece = w_cake;
        h_piece = scr->pixel_num / w_cake;
        h_cake -= scr->slice_count;
        if (h_piece > h_cake)
            h_piece = h_cake;
        scr->slice_count += h_piece;
        sgl_set_rect(&scr->slice_rect, scr->offset_x, scr->offset_y, w_piece,
                     h_piece);
        temp = scr->slice_rect;
        sgl_rotate_rect_cw(scr, &temp.x, &temp.y, &temp.w, &temp.h);
        sgl_rect2area(&temp, &scr->slice_area);
        sgl_reset_drawable_area(scr);
        if (scr->slice_count == scr->dirty_rect.h)
            scr->slice_state = SGL_SLICE_STATE_IDLE;
    }
}

void sgl_handler(sgl_screen_t *scr) {
    sgl_buffer_slice(scr);
    scr->draw(scr);
    scr->flush(scr->buffer, &scr->slice_rect);
    if (scr->slice_state == SGL_SLICE_STATE_IDLE)
        ++scr->frame_count;
}

void sgl_set_draw(sgl_screen_t *scr, void (*draw)(sgl_screen_t *scr)) {
    scr->draw = draw;
}

void sgl_set_flush(sgl_screen_t *scr,
                   void (*flush)(void *buffer, sgl_rect_t *refresh)) {
    scr->flush = flush;
}

int sgl_set_draw_pixel(sgl_screen_t *scr, uint32_t hor_res,
                       uint32_t buffer_size, uint32_t pixel_size,
                       void (*draw_pixel)(sgl_screen_t *scr, int32_t x,
                                          int32_t y, uint32_t color)) {
    uint32_t pixel_num;
    if (buffer_size == 0 || pixel_size == 0)
        return -1;
    pixel_num = buffer_size / pixel_size;
    if (pixel_num < hor_res)
        return -1;
    scr->draw_pixel = draw_pixel;
    scr->pixel_num = pixel_num;
    scr->buffer_size = pixel_num * pixel_size;
    return 0;
}

static int sgl_set_color_format(sgl_screen_t *scr, uint32_t buffer_size,
                                sgl_color_format_t color_format) {
    int ret = 0;
    switch (color_format) {
    case SGL_COLOR_FORMAT_MONO:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 1,
                                 sgl_draw_pixel_mono);
        break;
    case SGL_COLOR_FORMAT_RGB332:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 1,
                                 sgl_draw_pixel_rgb332);
        break;
    case SGL_COLOR_FORMAT_RGB565:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 2,
                                 sgl_draw_pixel_rgb565);
        break;
    case SGL_COLOR_FORMAT_RGB565SWAP:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 2,
                                 sgl_draw_pixel_rgb565swap);
        break;
    case SGL_COLOR_FORMAT_BGR565:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 2,
                                 sgl_draw_pixel_bgr565);
        break;
    case SGL_COLOR_FORMAT_RGB888:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 3,
                                 sgl_draw_pixel_rgb888);
        break;
    case SGL_COLOR_FORMAT_BGR888:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 3,
                                 sgl_draw_pixel_bgr888);
        break;
    case SGL_COLOR_FORMAT_XRGB8888:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 4,
                                 sgl_draw_pixel_xrgb8888);
        break;
    case SGL_COLOR_FORMAT_XBGR8888:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 4,
                                 sgl_draw_pixel_xbgr8888);
        break;
    case SGL_COLOR_FORMAT_ARGB8888:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 4,
                                 sgl_draw_pixel_argb8888);
        break;
    case SGL_COLOR_FORMAT_ABGR8888:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 4,
                                 sgl_draw_pixel_abgr8888);
        break;
    case SGL_COLOR_FORMAT_RGBA8888:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 4,
                                 sgl_draw_pixel_rgba8888);
        break;
    case SGL_COLOR_FORMAT_BGRA8888:
        ret = sgl_set_draw_pixel(scr, scr->hor_res, buffer_size, 4,
                                 sgl_draw_pixel_bgra8888);
        break;
    }
    return ret;
}

void sgl_set_dirty_area(sgl_screen_t *scr, int32_t left, int32_t top,
                        int32_t right, int32_t bottom) {
    sgl_set_area_within(&scr->dirty_area, &scr->screen_area, left, top, right,
                        bottom);
}

void sgl_set_drawable_area(sgl_screen_t *scr, int32_t left, int32_t top,
                           int32_t right, int32_t bottom) {
    sgl_set_area_within(&scr->drawable_area, &scr->slice_area, left, top, right,
                        bottom);
}

void sgl_reset_dirty_area(sgl_screen_t *scr) {
    scr->dirty_area = scr->screen_area;
}

void sgl_reset_drawable_area(sgl_screen_t *scr) {
    scr->drawable_area = scr->slice_area;
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
    sgl_set_area(&scr->screen_area, 0, 0, scr->max_x, scr->max_y);
}

uint32_t sgl_get_frame_count(sgl_screen_t *scr) { return scr->frame_count; }

void sgl_reset_frame_count(sgl_screen_t *scr) { scr->frame_count = 0; }

void sgl_clear_screen(sgl_screen_t *scr, uint8_t value) {
    memset(scr->buffer, value, scr->buffer_size);
}

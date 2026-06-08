// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_bitmap.h"
#include "sgl_common.h"
#include "sgl_line.h"

static inline void sgl_rotate_bitmap(sgl_screen_t *scr, int32_t *x, int32_t *y,
                                     int32_t *dx, int32_t *dy, int32_t w,
                                     int32_t h, uint32_t bmp_w, uint32_t bmp_h,
                                     sgl_dir_t *dir) {
    int32_t temp;
    sgl_rotate_point_ccw(scr, x, y);
    temp = *dx;
    switch (scr->rotate) {
    case SGL_ROTATE_0:
        break;
    case SGL_ROTATE_90:
        if (*dir == SGL_DIR_UP || *dir == SGL_DIR_DOWN) {
            *x -= h - 1;
            *dx = bmp_h - *dy - h;
            *dy = temp;
        } else {
            *x -= w - 1;
            *dx = bmp_w - *dy - w;
            *dy = temp;
        }
        break;
    case SGL_ROTATE_180:
        if (*dir == SGL_DIR_UP || *dir == SGL_DIR_DOWN) {
            *x -= w - 1;
            *y -= h - 1;
            *dx = bmp_w - *dx - w;
            *dy = bmp_h - *dy - h;
        } else {
            *x -= h - 1;
            *y -= w - 1;
            *dx = bmp_h - *dx - h;
            *dy = bmp_w - *dy - w;
        }
        break;
    case SGL_ROTATE_270:
        if (*dir == SGL_DIR_UP || *dir == SGL_DIR_DOWN) {
            *y -= w - 1;
            *dx = *dy;
            *dy = bmp_w - temp - w;
        } else {
            *y -= h - 1;
            *dx = *dy;
            *dy = bmp_h - temp - h;
        }
        break;
    }
    *dir = (*dir + scr->rotate) & 3;
}

void sgl_show_mono_bitmap(sgl_screen_t *scr, int32_t x, int32_t y, int32_t w,
                          int32_t h, const uint8_t *bitmap, sgl_dir_t dir,
                          uint32_t color) {
    int32_t dx = x, dy = y, temp;
    uint32_t bmp_w = w, bmp_h = h, bmp_x, bmp_y, mask, index, i, j;
    if (dir == SGL_DIR_UP || dir == SGL_DIR_DOWN) {
        if (sgl_clip_line(&x, &w, scr->visible.left, scr->visible.right))
            return;
        if (sgl_clip_line(&y, &h, scr->visible.top, scr->visible.bottom))
            return;
    } else {
        if (sgl_clip_line(&x, &h, scr->visible.left, scr->visible.right))
            return;
        if (sgl_clip_line(&y, &w, scr->visible.top, scr->visible.bottom))
            return;
    }
    dx = x - dx;
    dy = y - dy;
    sgl_rotate_bitmap(scr, &x, &y, &dx, &dy, w, h, bmp_w, bmp_h, &dir);
    x -= scr->offset_x;
    y -= scr->offset_y;
    switch (dir) {
    case SGL_DIR_UP:
        for (i = 0; i < h; ++i) {
            bmp_y = dy + i;
            index = (bmp_y >> 3) * bmp_w;
            mask = 1U << (bmp_y & 7);
            temp = y + i;
            for (j = 0; j < w; ++j) {
                bmp_x = dx + j;
                if (bitmap[index + bmp_x] & mask)
                    scr->draw_pixel(scr, x + j, temp, color);
            }
        }
        break;
    case SGL_DIR_RIGHT:
        for (i = 0; i < h; ++i) {
            bmp_y = (bmp_h - 1) - dx - i;
            index = (bmp_y >> 3) * bmp_w;
            mask = 1U << (bmp_y & 7);
            temp = x + i;
            for (j = 0; j < w; ++j) {
                bmp_x = dy + j;
                if (bitmap[index + bmp_x] & mask)
                    scr->draw_pixel(scr, temp, y + j, color);
            }
        }
        break;
    case SGL_DIR_LEFT:
        for (i = 0; i < h; ++i) {
            bmp_y = dx + i;
            index = (bmp_y >> 3) * bmp_w;
            mask = 1U << (bmp_y & 7);
            temp = x + i;
            for (j = 0; j < w; ++j) {
                bmp_x = (bmp_w - 1) - dy - j;
                if (bitmap[index + bmp_x] & mask)
                    scr->draw_pixel(scr, temp, y + j, color);
            }
        }
        break;
    case SGL_DIR_DOWN:
        for (i = 0; i < h; ++i) {
            bmp_y = (bmp_h - 1) - dy - i;
            index = (bmp_y >> 3) * bmp_w;
            mask = 1U << (bmp_y & 7);
            temp = y + i;
            for (j = 0; j < w; ++j) {
                bmp_x = (bmp_w - 1) - dx - j;
                if (bitmap[index + bmp_x] & mask)
                    scr->draw_pixel(scr, x + j, temp, color);
            }
        }
        break;
    }
}

void sgl_show_rgb565_bitmap(sgl_screen_t *scr, int32_t x, int32_t y, int32_t w,
                            int32_t h, const uint16_t *bitmap, sgl_dir_t dir) {
    int32_t dx = x, dy = y, temp;
    uint32_t bmp_w = w, bmp_h = h, bmp_x, bmp_y, index, i, j;
    if (dir == SGL_DIR_UP || dir == SGL_DIR_DOWN) {
        if (sgl_clip_line(&x, &w, scr->visible.left, scr->visible.right))
            return;
        if (sgl_clip_line(&y, &h, scr->visible.top, scr->visible.bottom))
            return;
    } else {
        if (sgl_clip_line(&x, &h, scr->visible.left, scr->visible.right))
            return;
        if (sgl_clip_line(&y, &w, scr->visible.top, scr->visible.bottom))
            return;
    }
    dx = x - dx;
    dy = y - dy;
    sgl_rotate_bitmap(scr, &x, &y, &dx, &dy, w, h, bmp_w, bmp_h, &dir);
    x -= scr->offset_x;
    y -= scr->offset_y;
    switch (dir) {
    case SGL_DIR_UP:
        for (i = 0; i < h; ++i) {
            bmp_y = dy + i;
            index = bmp_y * bmp_w;
            temp = y + i;
            for (j = 0; j < w; ++j) {
                bmp_x = dx + j;
                scr->draw_pixel(scr, x + j, temp, bitmap[index + bmp_x]);
            }
        }
        break;
    case SGL_DIR_RIGHT:
        for (i = 0; i < h; ++i) {
            bmp_y = (bmp_h - 1) - dx - i;
            index = bmp_y * bmp_w;
            temp = x + i;
            for (j = 0; j < w; ++j) {
                bmp_x = dy + j;
                scr->draw_pixel(scr, temp, y + j, bitmap[index + bmp_x]);
            }
        }
        break;
    case SGL_DIR_LEFT:
        for (i = 0; i < h; ++i) {
            bmp_y = dx + i;
            index = bmp_y * bmp_w;
            temp = x + i;
            for (j = 0; j < w; ++j) {
                bmp_x = (bmp_w - 1) - dy - j;
                scr->draw_pixel(scr, temp, y + j, bitmap[index + bmp_x]);
            }
        }
        break;
    case SGL_DIR_DOWN:
        for (i = 0; i < h; ++i) {
            bmp_y = (bmp_h - 1) - dy - i;
            index = bmp_y * bmp_w;
            temp = y + i;
            for (j = 0; j < w; ++j) {
                bmp_x = (bmp_w - 1) - dx - j;
                scr->draw_pixel(scr, x + j, temp, bitmap[index + bmp_x]);
            }
        }
        break;
    }
}

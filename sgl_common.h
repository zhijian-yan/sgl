// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_COMMON_H
#define __SGL_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_core.h"

static inline void sgl_rotate_point_ccw(sgl_screen_t *scr, int32_t *x,
                                        int32_t *y) {
    int32_t temp = *x;
    switch (scr->rotate) {
    case SGL_ROTATE_0:
        break;
    case SGL_ROTATE_90:
        *x = scr->max_y - *y;
        *y = temp;
        break;
    case SGL_ROTATE_180:
        *x = scr->max_x - *x;
        *y = scr->max_y - *y;
        break;
    case SGL_ROTATE_270:
        *x = *y;
        *y = scr->max_x - temp;
        break;
    }
}

static inline void sgl_rotate_point_cw(sgl_screen_t *scr, int32_t *x,
                                       int32_t *y) {
    int32_t temp = *x;
    switch (scr->rotate) {
    case SGL_ROTATE_0:
        break;
    case SGL_ROTATE_90:
        *x = *y;
        *y = scr->max_y - temp;
        break;
    case SGL_ROTATE_180:
        *x = scr->max_x - *x;
        *y = scr->max_y - *y;
        break;
    case SGL_ROTATE_270:
        *x = scr->max_x - *y;
        *y = temp;
        break;
    }
}

static inline void sgl_rotate_rect_ccw(sgl_screen_t *scr, int32_t *x,
                                       int32_t *y, int32_t *w, int32_t *h) {
    int32_t temp1 = *x;
    int32_t temp2 = *w;
    switch (scr->rotate) {
    case SGL_ROTATE_0:
        break;
    case SGL_ROTATE_90:
        *x = scr->max_y - *y;
        *y = temp1;
        *w = -*h;
        *h = temp2;
        break;
    case SGL_ROTATE_180:
        *x = scr->max_x - *x;
        *y = scr->max_y - *y;
        *w = -*w;
        *h = -*h;
        break;
    case SGL_ROTATE_270:
        *x = *y;
        *y = scr->max_x - temp1;
        *w = *h;
        *h = -temp2;
        break;
    }
}

static inline void sgl_rotate_rect_cw(sgl_screen_t *scr, int32_t *x, int32_t *y,
                                      int32_t *w, int32_t *h) {
    int32_t temp1 = *x;
    int32_t temp2 = *w;
    switch (scr->rotate) {
    case SGL_ROTATE_0:
        break;
    case SGL_ROTATE_90:
        *x = *y;
        *y = scr->max_y - temp1;
        *w = *h;
        *h = -temp2;
        break;
    case SGL_ROTATE_180:
        *x = scr->max_x - *x;
        *y = scr->max_y - *y;
        *w = -*w;
        *h = -*h;
        break;
    case SGL_ROTATE_270:
        *x = scr->max_x - *y;
        *y = temp1;
        *w = -*h;
        *h = temp2;
        break;
    }
}

static inline void sgl_align(int32_t *x, int32_t *y, int32_t w, int32_t h,
                             sgl_align_t align) {
    switch (align) {
    case SGL_ALIGN_UP_LEFT:
        break;
    case SGL_ALIGN_UP_RIGHT:
        *x -= w - 1;
        break;
    case SGL_ALIGN_DOWN_LEFT:
        *y -= h - 1;
        break;
    case SGL_ALIGN_DOWN_RIGHT:
        *x -= w - 1;
        *y -= h - 1;
        break;
    case SGL_ALIGN_CENTER:
        *x -= w / 2;
        *y -= h / 2;
        break;
    case SGL_ALIGN_UP_CENTER:
        *x -= w / 2;
        break;
    case SGL_ALIGN_DOWN_CENTER:
        *x -= w / 2;
        *y -= h - 1;
        break;
    case SGL_ALIGN_LEFT_CENTER:
        *y -= h / 2;
        break;
    case SGL_ALIGN_RIGHT_CENTER:
        *x -= w - 1;
        *y -= h / 2;
        break;
    }
}

static inline void sgl_draw_hpixel(sgl_screen_t *scr, int32_t x, int32_t y,
                                   int32_t len, uint32_t color) {
    int32_t x1 = x + len;
    for (len = (len > 0) ? 1 : -1; x != x1; x += len)
        scr->draw_pixel(scr, x, y, color);
}

static inline void sgl_draw_vpixel(sgl_screen_t *scr, int32_t x, int32_t y,
                                   int32_t len, uint32_t color) {
    int32_t y1 = y + len;
    for (len = (len > 0) ? 1 : -1; y != y1; y += len)
        scr->draw_pixel(scr, x, y, color);
}

static inline int sgl_check_rect(sgl_screen_t *scr, int32_t left, int32_t top,
                                 int32_t right, int32_t bottom) {
    if (left > scr->visible.right || right < scr->visible.left ||
        top > scr->visible.bottom || bottom < scr->visible.top)
        return -1;
    return 0;
}

static inline void sgl_set_rect(sgl_rect_t *rect, int32_t left, int32_t top,
                                int32_t right, int32_t bottom) {
    rect->left = left;
    rect->top = top;
    rect->right = right;
    rect->bottom = bottom;
}

static inline int sgl_clip_line(int32_t *start, int32_t *len, int32_t min,
                                int32_t max) {
    int32_t end;
    if (*len > 0) {
        if (*start > max)
            return -1;
        end = *start + *len - 1;
        if (end < min)
            return -1;
        if (*start < min)
            *start = min;
        if (end > max)
            end = max;
        *len = end - *start + 1;
    } else if (*len < 0) {
        if (*start < min)
            return -1;
        end = *start + *len + 1;
        if (end > max)
            return -1;
        if (end < min)
            end = min;
        if (*start > max)
            *start = max;
        *len = end - *start - 1;
    }
    return 0;
}

void sgl_draw_circle_section(sgl_screen_t *scr, int32_t xc, int32_t yc,
                             int32_t r, int32_t offset_x, int32_t offset_y,
                             uint32_t color);
void sgl_draw_filled_circle_section(sgl_screen_t *scr, int32_t xc, int32_t yc,
                                    int32_t r, int32_t offset_x,
                                    int32_t offset_y, uint32_t color);
void sgl_draw_ellipse_section(sgl_screen_t *scr, int32_t xc, int32_t yc,
                              int32_t rx, int32_t ry, uint32_t color);
void sgl_draw_filled_ellipse_section(sgl_screen_t *scr, int32_t xc, int32_t yc,
                                     int32_t rx, int32_t ry, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif

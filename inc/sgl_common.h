// SPDX-License-Identifier: MIT
// Copyright (c) 2025-2026 Zhijian Yan

#ifndef __SGL_COMMON_H
#define __SGL_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_types.h"

static inline void sgl_set_rect(sgl_rect_t *rect, int32_t x, int32_t y,
                                int32_t w, int32_t h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

static inline void sgl_set_area(sgl_area_t *area, int32_t left, int32_t top,
                                int32_t right, int32_t bottom) {
    area->left = left;
    area->top = top;
    area->right = right;
    area->bottom = bottom;
}

static inline void sgl_area2rect(const sgl_area_t *area, sgl_rect_t *rect) {
    rect->x = area->left;
    rect->y = area->top;
    rect->w = area->right - area->left + 1;
    rect->h = area->bottom - area->top + 1;
}

static inline void sgl_rect2area(const sgl_rect_t *rect, sgl_area_t *area) {
    area->left = rect->x;
    area->top = rect->y;
    if (rect->w > 0)
        area->right = rect->x + rect->w - 1;
    else
        area->right = rect->x - rect->w - 1;
    if (rect->h > 0)
        area->bottom = rect->y + rect->h - 1;
    else
        area->bottom = rect->y - rect->h - 1;
}

static inline void sgl_set_area_within(sgl_area_t *area,
                                       const sgl_area_t *within, int32_t left,
                                       int32_t top, int32_t right,
                                       int32_t bottom) {
    if (left < within->left)
        left = within->left;
    if (top < within->top)
        top = within->top;
    if (right > within->right)
        right = within->right;
    if (bottom > within->bottom)
        bottom = within->bottom;
    if (left > right)
        left = right;
    if (top > bottom)
        top = bottom;
    sgl_set_area(area, left, top, right, bottom);
}

static inline int sgl_check_area(sgl_area_t *bounds, int32_t left, int32_t top,
                                 int32_t right, int32_t bottom) {
    if (left > bounds->right || right < bounds->left || top > bounds->bottom ||
        bottom < bounds->top)
        return -1;
    return 0;
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
    if (*w < 0) {
        *x += *w + 1;
        *w = -*w;
    }
    if (*h < 0) {
        *y += *h + 1;
        *h = -*h;
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
    if (*w < 0) {
        *x += *w + 1;
        *w = -*w;
    }
    if (*h < 0) {
        *y += *h + 1;
        *h = -*h;
    }
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

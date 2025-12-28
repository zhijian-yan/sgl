// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_PRIVATE_H
#define __SGL_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_core.h"

struct sgl_screen {
    void *buffer;
    uint32_t buffer_size;
    uint32_t hor_res;
    uint32_t ver_res;
    uint32_t max_x;
    uint32_t max_y;
    uint32_t max_fps;
    uint32_t fcount;
    uint32_t ticks;
    sgl_rect_t visible;
    sgl_rect_t invalidate;
    sgl_rotate_t rotate;
    void (*paint)(void);
    void (*flush)(void *buffer, uint32_t buffer_size);
    void (*draw_pixel)(int32_t x, int32_t y, uint32_t color);
};

typedef enum {
    SGL_CIRCLE_UPPER_RIGHT = 0x01,
    SGL_CIRCLE_UPPER_LEFT = 0x02,
    SGL_CIRCLE_LOWER_RIGHT = 0x04,
    SGL_CIRCLE_LOWER_LEFT = 0x08,
    SGL_CIRCLE_ALL = 0xFF,
} sgl_circle_t;

extern struct sgl_screen *__act_scr;

void __sgl_rotate_point(int32_t *x, int32_t *y);
int __sgl_clip_line(int32_t *start, int32_t *len, int32_t min, int32_t max);
void __sgl_align(int32_t *x, int32_t *y, int32_t w, int32_t h,
                 sgl_align_t align);

static inline int __sgl_check_rect(int32_t left, int32_t top, int32_t right,
                                   int32_t bottom) {
    if (left > __act_scr->visible.right || right < __act_scr->visible.left ||
        top > __act_scr->visible.bottom || bottom < __act_scr->visible.top)
        return -1;
    return 0;
}

static inline void __sgl_draw_hpixel(int32_t x, int32_t y, int32_t len,
                                     uint32_t color) {
    int32_t x1 = x + len;
    for (len = (len > 0) ? 1 : -1; x != x1; x += len)
        __act_scr->draw_pixel(x, y, color);
}

static inline void __sgl_draw_vpixel(int32_t x, int32_t y, int32_t len,
                                     uint32_t color) {
    int32_t y1 = y + len;
    for (len = (len > 0) ? 1 : -1; y != y1; y += len)
        __act_scr->draw_pixel(x, y, color);
}

void __sgl_draw_outlined_circle_section(int32_t xc, int32_t yc, int32_t r,
                                        sgl_circle_t dir, uint32_t color);
void __sgl_draw_filled_circle_section(int32_t xc, int32_t yc, int32_t r,
                                      sgl_circle_t dir, uint32_t color);
void __sgl_draw_outlined_ellipse_section(int32_t xc, int32_t yc, int32_t rx,
                                         int32_t ry, sgl_circle_t dir,
                                         uint32_t color);
void __sgl_draw_filled_ellipse_section(int32_t xc, int32_t yc, int32_t rx,
                                       int32_t ry, sgl_circle_t dir,
                                       uint32_t color);

#ifdef __cplusplus
}
#endif

#endif

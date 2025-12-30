// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_COLOR_H
#define __SGL_COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_core.h"

typedef enum {
    SGL_MONO_BLACK = 0,
    SGL_MONO_WHITE = 1,
    SGL_MONO_INVERT = 2,
} sgl_mono_color_t;

#define SGL_COLOR_GET_A(color) ((uint8_t)((color) >> 24))
#define SGL_COLOR_GET_R(color) ((uint8_t)((color) >> 16))
#define SGL_COLOR_GET_G(color) ((uint8_t)((color) >> 8))
#define SGL_COLOR_GET_B(color) ((uint8_t)(color))
#define SGL_COLOR_SET_A(color, a)                                              \
    (((color) & 0x00FFFFFF) | ((uint32_t)(a) << 24))
#define SGL_COLOR_ARGB(a, r, g, b)                                             \
    ((uint32_t)(a) << 24 | (uint32_t)(r) << 16 | (uint32_t)(g) << 8 |          \
     (uint32_t)(b))
#define SGL_COLOR_RGB(r, g, b) SGL_COLOR_ARGB(0xFF, r, g, b)
#define SGL_COLOR_RGB565(r, g, b)                                              \
    ((uint16_t)(((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) |    \
                ((uint16_t)(b & 0xF8) >> 3)))

void sgl_color_hsv2rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g,
                       uint8_t *b);
void sgl_color_rgb2hsv(uint8_t r, uint8_t g, uint8_t b, uint16_t *h, uint8_t *s,
                       uint8_t *v);
static inline uint32_t sgl_color_to_rgb(uint16_t h, uint8_t s, uint8_t v) {
    uint8_t r, g, b;
    sgl_color_hsv2rgb(h, s, v, &r, &g, &b);
    return SGL_COLOR_RGB(r, g, b);
}
static inline void sgl_color_to_hsv(uint32_t color, uint16_t *h, uint8_t *s,
                                    uint8_t *v) {
    sgl_color_rgb2hsv((uint8_t)(color >> 16), (uint8_t)(color >> 8),
                      (uint8_t)(color), h, s, v);
}
static inline uint32_t sgl_color_set_v(uint32_t color, uint8_t v) {
    uint16_t h;
    uint8_t s, old_v;
    sgl_color_rgb2hsv(SGL_COLOR_GET_R(color), SGL_COLOR_GET_G(color),
                      SGL_COLOR_GET_B(color), &h, &s, &old_v);
    return sgl_color_to_rgb(h, s, v);
}
static inline uint16_t sgl_color_to_rgb565(uint32_t color) {
    return (uint16_t)(((color >> 8) & 0xF800) | ((color >> 5) & 0x07E0) |
                      ((color >> 3) & 0x001F));
}
static inline uint32_t sgl_color_from_rgb565(uint16_t color) {
    uint8_t r5 = (color >> 11) & 0x1F;
    uint8_t g6 = (color >> 5) & 0x3F;
    uint8_t b5 = color & 0x1F;
    uint8_t r = (r5 << 3) | (r5 >> 2);
    uint8_t g = (g6 << 2) | (g6 >> 4);
    uint8_t b = (b5 << 3) | (b5 >> 2);
    return SGL_COLOR_RGB(r, g, b);
}
static inline uint32_t sgl_color_gray(uint32_t color) {
    uint8_t r = SGL_COLOR_GET_R(color);
    uint8_t g = SGL_COLOR_GET_G(color);
    uint8_t b = SGL_COLOR_GET_B(color);
    uint8_t y = (r * 77 + g * 150 + b * 29) >> 8;
    return SGL_COLOR_RGB(y, y, y);
}
static inline uint32_t sgl_color_blend(uint32_t fg, uint32_t bg) {
    uint8_t a = SGL_COLOR_GET_A(fg);
    if (a == 255)
        return fg;
    if (a == 0)
        return bg;
    uint8_t r =
        (SGL_COLOR_GET_R(fg) * a + SGL_COLOR_GET_R(bg) * (255 - a)) >> 8;
    uint8_t g =
        (SGL_COLOR_GET_G(fg) * a + SGL_COLOR_GET_G(bg) * (255 - a)) >> 8;
    uint8_t b =
        (SGL_COLOR_GET_B(fg) * a + SGL_COLOR_GET_B(bg) * (255 - a)) >> 8;
    return SGL_COLOR_RGB(r, g, b);
}

#ifdef __cplusplus
}
#endif

#endif

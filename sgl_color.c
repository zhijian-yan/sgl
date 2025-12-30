// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_color.h"

void sgl_color_hsv2rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g,
                       uint8_t *b) {
    if (s == 0) {
        *r = *g = *b = v;
        return;
    }
    uint8_t region = h / 60;
    uint8_t f = (uint8_t)((h - region * 60) * 255 / 60);
    uint16_t sv = (uint16_t)s * v;
    uint8_t p = (uint8_t)((v * (255 - s) + 128) >> 8);
    uint8_t q = (uint8_t)((v * 255 - ((sv * f + 128) >> 8) + 128) >> 8);
    uint8_t t = (uint8_t)((v * 255 - ((sv * (255 - f) + 128) >> 8) + 128) >> 8);
    switch (region) {
    case 0:
        *r = v;
        *g = t;
        *b = p;
        break;
    case 1:
        *r = q;
        *g = v;
        *b = p;
        break;
    case 2:
        *r = p;
        *g = v;
        *b = t;
        break;
    case 3:
        *r = p;
        *g = q;
        *b = v;
        break;
    case 4:
        *r = t;
        *g = p;
        *b = v;
        break;
    default:
        *r = v;
        *g = p;
        *b = q;
        break;
    }
}

void sgl_color_rgb2hsv(uint8_t r, uint8_t g, uint8_t b, uint16_t *h, uint8_t *s,
                       uint8_t *v) {
    uint8_t max, min, delta;
    max = r > g ? (r > b ? r : b) : (g > b ? g : b);
    min = r < g ? (r < b ? r : b) : (g < b ? g : b);
    *v = max;
    delta = max - min;
    if (delta == 0) {
        *h = 0;
        *s = 0;
        return;
    }
    *s = (uint8_t)((uint16_t)delta * 255 / max);
    if (max == r) {
        *h = 60 * (int16_t)(g - b) / delta;
        if ((int16_t)*h < 0)
            *h += 360;
    } else if (max == g) {
        *h = 60 * (int16_t)(b - r) / delta + 120;
    } else {
        *h = 60 * (int16_t)(r - g) / delta + 240;
    }
}

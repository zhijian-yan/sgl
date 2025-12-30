// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_pixel.h"
#include "sgl_color.h"
#include "sgl_private.h"

void sgl_draw_pixel_mono(int32_t x, int32_t y, uint32_t color) {
    switch (color) {
    case SGL_MONO_BLACK:
        ((uint8_t *)__act_scr->buffer)[(y >> 3) * __act_scr->hor_res + x] &=
            ~(1 << (y & 7));
        break;
    case SGL_MONO_WHITE:
        ((uint8_t *)__act_scr->buffer)[(y >> 3) * __act_scr->hor_res + x] |=
            (1 << (y & 7));
        break;
    case SGL_MONO_INVERT:
        ((uint8_t *)__act_scr->buffer)[(y >> 3) * __act_scr->hor_res + x] ^=
            (1 << (y & 7));
        break;
    }
}

void sgl_draw_pixel_rgb565(int32_t x, int32_t y, uint32_t color) {
    ((uint16_t *)__act_scr->buffer)[x + __act_scr->hor_res * y] =
        (color << 8) | (color >> 8);
}

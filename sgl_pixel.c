// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_pixel.h"
#include "sgl_common.h"

void sgl_draw_pixel_mono(sgl_screen_t *scr, int32_t x, int32_t y,
                         uint32_t color) {
    switch (color) {
    case SGL_MONO_BLACK:
        ((uint8_t *)scr->buffer)[(y >> 3) * scr->hor_res + x] &=
            ~(1U << (y & 7));
        break;
    case SGL_MONO_WHITE:
        ((uint8_t *)scr->buffer)[(y >> 3) * scr->hor_res + x] |=
            (1U << (y & 7));
        break;
    case SGL_MONO_INVERT:
        ((uint8_t *)scr->buffer)[(y >> 3) * scr->hor_res + x] ^=
            (1U << (y & 7));
        break;
    }
}

void sgl_draw_pixel_rgb565(sgl_screen_t *scr, int32_t x, int32_t y,
                           uint32_t color) {
    ((uint16_t *)scr->buffer)[x + scr->hor_res * y] = color;
}

void sgl_draw_pixel_rgb565_swap(sgl_screen_t *scr, int32_t x, int32_t y,
                                uint32_t color) {
    ((uint16_t *)scr->buffer)[x + scr->hor_res * y] =
        (color << 8) | (color >> 8);
}

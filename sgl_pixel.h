// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_PIXEL_H
#define __SGL_PIXEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_core.h"

void sgl_draw_pixel_mono(sgl_screen_t *scr, int32_t x, int32_t y,
                         uint32_t color);
void sgl_draw_pixel_rgb565(sgl_screen_t *scr, int32_t x, int32_t y,
                           uint32_t color);
void sgl_draw_pixel_rgb565_swap(sgl_screen_t *scr, int32_t x, int32_t y,
                                uint32_t color);

#ifdef __cplusplus
}
#endif

#endif

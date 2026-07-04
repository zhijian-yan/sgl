// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_LINE_H
#define __SGL_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_core.h"

void sgl_draw_point(sgl_screen_t *scr, int32_t x, int32_t y, uint32_t color);
void sgl_draw_hline(sgl_screen_t *scr, int32_t x, int32_t y, int32_t len,
                    uint32_t color);
void sgl_draw_vline(sgl_screen_t *scr, int32_t x, int32_t y, int32_t len,
                    uint32_t color);
void sgl_draw_line(sgl_screen_t *scr, int32_t x0, int32_t y0, int32_t x1,
                   int32_t y1, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif

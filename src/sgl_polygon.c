// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "../inc/sgl_polygon.h"
#include "../inc/sgl_line.h"

void sgl_draw_trian(sgl_screen_t *scr, int32_t x0, int32_t y0, int32_t x1,
                    int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
    sgl_draw_line(scr, x0, y0, x1, y1, color);
    sgl_draw_line(scr, x1, y1, x2, y2, color);
    sgl_draw_line(scr, x2, y2, x0, y0, color);
}

void sgl_draw_polygon() {}

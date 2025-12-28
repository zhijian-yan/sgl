// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_CIRCLE_H
#define __SGL_CIRCLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_core.h"

void sgl_draw_circle(int32_t xc, int32_t yc, int32_t r, int32_t is_filled,
                     uint32_t color);
void sgl_draw_ellipse(int32_t xc, int32_t yc, int32_t rx, int32_t ry,
                      int32_t is_filled, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif

// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_RECT_H
#define __SGL_RECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_core.h"

void sgl_draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, int is_filled,
                   uint32_t color);
void sgl_draw_round_rect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r,
                         int is_filled, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif

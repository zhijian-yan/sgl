// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_BITMAP_H
#define __SGL_BITMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_core.h"

void sgl_show_bitmap(int32_t x, int32_t y, int32_t w, int32_t h,
                     const char *bitmap, sgl_dir_t dir, uint32_t color);

#ifdef __cplusplus
}
#endif

#endif

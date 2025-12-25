// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_private.h"

struct sgl_screen *__act_scr;

void __sgl_rotate_point(int32_t *x, int32_t *y) {
    int32_t temp = *x;
    switch (__act_scr->rotate) {
    case SGL_ROTATE_0:
        break;
    case SGL_ROTATE_90:
        *x = __act_scr->max_x - *y;
        *y = temp;
        break;
    case SGL_ROTATE_180:
        *x = __act_scr->max_x - *x;
        *y = __act_scr->max_y - *y;
        break;
    case SGL_ROTATE_270:
        *x = *y;
        *y = __act_scr->max_y - temp;
        break;
    }
}

int __sgl_clip_line(int32_t *start, int32_t *len, int32_t min, int32_t max) {
    int32_t end;
    if (*len > 0) {
        if (*start > max)
            return -1;
        end = *start + *len - 1;
        if (end < min)
            return -1;
        if (*start < min)
            *start = min;
        if (end > max)
            end = max;
        *len = end - *start + 1;
    } else if (*len < 0) {
        if (*start < min)
            return -1;
        end = *start + *len + 1;
        if (end > max)
            return -1;
        if (end < min)
            end = min;
        if (*start > max)
            *start = max;
        *len = end - *start - 1;
    }
    return 0;
}

void __sgl_align(int32_t *x, int32_t *y, int32_t w, int32_t h,
                 sgl_align_t align) {
    switch (align) {
    case SGL_ALIGN_UP_LEFT:
        break;
    case SGL_ALIGN_UP_RIGHT:
        *x -= w - 1;
        break;
    case SGL_ALIGN_DOWN_LEFT:
        *y -= h - 1;
        break;
    case SGL_ALIGN_DOWN_RIGHT:
        *x -= w - 1;
        *y -= h - 1;
        break;
    case SGL_ALIGN_CENTER:
        *x -= w / 2;
        *y -= h / 2;
        break;
    case SGL_ALIGN_UP_CENTER:
        *x -= w / 2;
        break;
    case SGL_ALIGN_DOWN_CENTER:
        *x -= w / 2;
        *y -= h - 1;
        break;
    case SGL_ALIGN_LEFT_CENTER:
        *y -= h / 2;
        break;
    case SGL_ALIGN_RIGHT_CENTER:
        *x -= w - 1;
        *y -= h / 2;
        break;
    }
}

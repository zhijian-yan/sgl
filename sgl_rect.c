// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_rect.h"
#include "sgl_line.h"
#include "sgl_private.h"

void sgl_draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, int is_filled,
                   uint32_t color) {
    if (w < 0) {
        x += w + 1;
        w = -w;
    }
    if (h < 0) {
        y += h + 1;
        h = -h;
    }
    if (__sgl_clip_line(&x, &w, __act_scr->visible.left,
                        __act_scr->visible.right))
        return;
    if (__sgl_clip_line(&y, &h, __act_scr->visible.top,
                        __act_scr->visible.bottom))
        return;
    if (w > h) {
        if (is_filled == 0 && h > 1) {
            __sgl_draw_hpixel(x, y, w, color);
            __sgl_draw_hpixel(x, y + h - 1, w, color);
            __sgl_draw_vpixel(x, y + 1, h - 2, color);
            __sgl_draw_vpixel(x + w - 1, y + 1, h - 2, color);
        } else {
            for (h += y; y < h; ++y) {
                __sgl_draw_hpixel(x, y, w, color);
            }
        }
    } else {
        if (is_filled == 0 && w > 1) {
            __sgl_draw_vpixel(x, y, h, color);
            __sgl_draw_vpixel(x + w - 1, y, h, color);
            __sgl_draw_hpixel(x + 1, y, w - 2, color);
            __sgl_draw_hpixel(x + 1, y + h - 1, w - 2, color);
        } else {
            for (w += x; x < w; ++x) {
                __sgl_draw_vpixel(x, y, h, color);
            }
        }
    }
}

void sgl_draw_round_rect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r,
                         int is_filled, uint32_t color) {
    int32_t mw, mh;
    if (w < 0) {
        x += w + 1;
        w = -w;
    }
    if (h < 0) {
        y += h + 1;
        h = -h;
    }
    if (r < 0)
        r = -r;
    if (r > (w >> 1))
        r = (w >> 1);
    if (r > (h >> 1))
        r = (h >> 1);
    mw = w - (r << 1);
    mh = h - (r << 1);
    if (w <= 2 || h <= 2) {
        if (w > h) {
            for (h += y; y < h; ++y)
                sgl_draw_hline(x, y, w, color);
        } else {
            for (w += x; x < w; ++x)
                sgl_draw_vline(x, y, h, color);
        }
        return;
    }
    if (is_filled == 0) {
        sgl_draw_hline(x + r, y, mw, color);
        sgl_draw_hline(x + r, y + h - 1, mw, color);
        sgl_draw_vline(x, y + r, mh, color);
        sgl_draw_vline(x + w - 1, y + r, mh, color);
        __sgl_draw_circle_section(x + r, y + r, r, mw - 1, mh - 1, color);
    } else {
        sgl_draw_rect(x + r, y, mw, h, 1, color);
        sgl_draw_rect(x, y + r, r, mh, 1, color);
        sgl_draw_rect(x + w - r, y + r, r, mh, 1, color);
        __sgl_draw_filled_circle_section(x + r, y + r, r, mw - 1, mh - 1,
                                         color);
    }
}

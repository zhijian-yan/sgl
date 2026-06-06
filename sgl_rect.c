// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#include "sgl_rect.h"
#include "sgl_common.h"
#include "sgl_line.h"

void sgl_draw_rect(sgl_screen_t *scr, int32_t x, int32_t y, int32_t w,
                   int32_t h, int is_filled, uint32_t color) {
    if (sgl_clip_line(&x, &w, scr->visible.left, scr->visible.right))
        return;
    if (sgl_clip_line(&y, &h, scr->visible.top, scr->visible.bottom))
        return;
    sgl_rotate_rect_ccw(scr, &x, &y, &w, &h);
    x -= scr->offset_x;
    y -= scr->offset_y;
    if (w < 0) {
        x += w + 1;
        w = -w;
    }
    if (h < 0) {
        y += h + 1;
        h = -h;
    }
    if (w > h) {
        if (is_filled == 0 && h > 1) {
            sgl_draw_hpixel(scr, x, y, w, color);
            sgl_draw_hpixel(scr, x, y + h - 1, w, color);
            sgl_draw_vpixel(scr, x, y + 1, h - 2, color);
            sgl_draw_vpixel(scr, x + w - 1, y + 1, h - 2, color);
        } else {
            for (h += y; y < h; ++y) {
                sgl_draw_hpixel(scr, x, y, w, color);
            }
        }
    } else {
        if (is_filled == 0 && w > 1) {
            sgl_draw_vpixel(scr, x, y, h, color);
            sgl_draw_vpixel(scr, x + w - 1, y, h, color);
            sgl_draw_hpixel(scr, x + 1, y, w - 2, color);
            sgl_draw_hpixel(scr, x + 1, y + h - 1, w - 2, color);
        } else {
            for (w += x; x < w; ++x) {
                sgl_draw_vpixel(scr, x, y, h, color);
            }
        }
    }
}

void sgl_draw_round_rect(sgl_screen_t *scr, int32_t x, int32_t y, int32_t w,
                         int32_t h, int32_t r, int is_filled, uint32_t color) {
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
                sgl_draw_hline(scr, x, y, w, color);
        } else {
            for (w += x; x < w; ++x)
                sgl_draw_vline(scr, x, y, h, color);
        }
        return;
    }
    if (is_filled == 0) {
        sgl_draw_hline(scr, x + r, y, mw, color);
        sgl_draw_hline(scr, x + r, y + h - 1, mw, color);
        sgl_draw_vline(scr, x, y + r, mh, color);
        sgl_draw_vline(scr, x + w - 1, y + r, mh, color);
        sgl_draw_circle_section(scr, x + r, y + r, r, mw - 1, mh - 1, color);
    } else {
        sgl_draw_rect(scr, x + r, y, mw, h, 1, color);
        sgl_draw_rect(scr, x, y + r, r, mh, 1, color);
        sgl_draw_rect(scr, x + w - r, y + r, r, mh, 1, color);
        sgl_draw_filled_circle_section(scr, x + r, y + r, r, mw - 1, mh - 1,
                                       color);
    }
}

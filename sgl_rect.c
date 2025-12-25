#include "sgl_rect.h"
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
}

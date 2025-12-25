#include "sgl_line.h"
#include "sgl_private.h"

void sgl_draw_point(int32_t x, int32_t y, uint32_t color) {
    if (__sgl_check_rect(x, y, x, y))
        return;
    __sgl_rotate_point(&x, &y);
    __act_scr->draw_pixel(x, y, color);
}

static void __sgl_draw_hline(int32_t x, int32_t y, int32_t len,
                             uint32_t color) {
    if (y < __act_scr->visible.top || y > __act_scr->visible.bottom)
        return;
    if (__sgl_clip_line(&x, &len, __act_scr->visible.left,
                        __act_scr->visible.right))
        return;
    __sgl_draw_hpixel(x, y, len, color);
}

static void __sgl_draw_vline(int32_t x, int32_t y, int32_t len,
                             uint32_t color) {
    if (x < __act_scr->visible.left || x > __act_scr->visible.right)
        return;
    if (__sgl_clip_line(&y, &len, __act_scr->visible.top,
                        __act_scr->visible.bottom))
        return;
    __sgl_draw_vpixel(x, y, len, color);
}

void sgl_draw_hline(int32_t x, int32_t y, int32_t len, uint32_t color) {
    __sgl_rotate_point(&x, &y);
    switch (__act_scr->rotate) {
    case SGL_ROTATE_0:
        __sgl_draw_hline(x, y, len, color);
        break;
    case SGL_ROTATE_90:
        __sgl_draw_vline(x, y, len, color);
        break;
    case SGL_ROTATE_180:
        __sgl_draw_hline(x, y, -len, color);
        break;
    case SGL_ROTATE_270:
        __sgl_draw_vline(x, y, -len, color);
        break;
    }
}

void sgl_draw_vline(int32_t x, int32_t y, int32_t len, uint32_t color) {
    __sgl_rotate_point(&x, &y);
    switch (__act_scr->rotate) {
    case SGL_ROTATE_0:
        __sgl_draw_vline(x, y, len, color);
        break;
    case SGL_ROTATE_90:
        __sgl_draw_hline(x, y, -len, color);
        break;
    case SGL_ROTATE_180:
        __sgl_draw_vline(x, y, -len, color);
        break;
    case SGL_ROTATE_270:
        __sgl_draw_hline(x, y, len, color);
        break;
    }
}

void sgl_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                   uint32_t color) {
    int32_t dx, dy, sx, sy, err;
    __sgl_rotate_point(&x0, &y0);
    __sgl_rotate_point(&x1, &y1);
    dx = x1 - x0;
    dy = y1 - y0;
    sx = 1, sy = 1;
    if (dx < 0) {
        dx = -dx;
        sx = -1;
    }
    if (dy < 0) {
        dy = -dy;
        sy = -1;
    }
    if (dx > dy) {
        for (err = dx >> 1; x0 != x1; x0 += sx) {
            sgl_draw_point(x0, y0, color);
            err -= dy;
            if (err < 0) {
                y0 += sy;
                err += dx;
            }
        }
    } else {
        for (err = dy >> 1; y0 != y1; y0 += sy) {
            sgl_draw_point(x0, y0, color);
            err -= dx;
            if (err < 0) {
                x0 += sx;
                err += dy;
            }
        }
    }
    //    sgl_draw_point(x1, y1, color);
}

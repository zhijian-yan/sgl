#include "sgl_polygon.h"
#include "sgl_line.h"

void sgl_draw_trian(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2,
                    int32_t y2, uint32_t color) {
    sgl_draw_line(x0, y0, x1, y1, color);
    sgl_draw_line(x1, y1, x2, y2, color);
    sgl_draw_line(x2, y2, x0, y0, color);
}

void sgl_draw_polygon() {}

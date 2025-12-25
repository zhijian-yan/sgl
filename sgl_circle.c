#include "sgl_circle.h"
#include "sgl_line.h"
#include "sgl_private.h"

/*
+-----------> X
|   6  7
| 5      8
| 4      1
|   3  2
v
Y
*/

void sgl_draw_outlined_circle_section(int32_t xc, int32_t yc, int32_t r,
                                      sgl_circle_t dir, uint32_t color) {
    int32_t x = 0;
    int32_t y = r;
    int32_t d = 3 - 2 * r;
    while (1) {
        if (d < 0)
            d = d + 4 * x + 6;
        else {
            d = d + 4 * (x - y) + 10;
            --y;
        }
        ++x;
        if (x < y) {
            if (dir & SGL_CIRCLE_LOWER_RIGHT) {
                sgl_draw_point(xc + y, yc + x, color); // 1
                sgl_draw_point(xc + x, yc + y, color); // 2
            }
            if (dir & SGL_CIRCLE_LOWER_LEFT) {
                sgl_draw_point(xc - x, yc + y, color); // 3
                sgl_draw_point(xc - y, yc + x, color); // 4
            }
            if (dir & SGL_CIRCLE_UPPER_LEFT) {
                sgl_draw_point(xc - y, yc - x, color); // 5
                sgl_draw_point(xc - x, yc - y, color); // 6
            }
            if (dir & SGL_CIRCLE_UPPER_RIGHT) {
                sgl_draw_point(xc + x, yc - y, color); // 7
                sgl_draw_point(xc + y, yc - x, color); // 8
            }
        } else
            break;
    }
    if (x == y) {
        if (dir & SGL_CIRCLE_LOWER_RIGHT)
            sgl_draw_point(xc + y, yc + x, color); // 1, 2
        if (dir & SGL_CIRCLE_LOWER_LEFT)
            sgl_draw_point(xc - x, yc + y, color); // 3, 4
        if (dir & SGL_CIRCLE_UPPER_LEFT)
            sgl_draw_point(xc - y, yc - x, color); // 5, 6
        if (dir & SGL_CIRCLE_UPPER_RIGHT)
            sgl_draw_point(xc + x, yc - y, color); // 7, 8
    }
}

void sgl_draw_filled_circle_section(int32_t xc, int32_t yc, int32_t r,
                                    sgl_circle_t dir, uint32_t color) {
    int32_t i;
    int32_t x = 0;
    int32_t y = r;
    int32_t d = 3 - 2 * r;
    while (1) {
        if (d < 0)
            d = d + 4 * x + 6;
        else {
            d = d + 4 * (x - y) + 10;
            --y;
        }
        ++x;
        if (x < y) {
            if (dir & SGL_CIRCLE_LOWER_RIGHT) {
                sgl_draw_hline(xc + x + 1, yc + x, y - x, color); // 1
                sgl_draw_vline(xc + x, yc + x + 1, y - x, color); // 2
            }
            if (dir & SGL_CIRCLE_LOWER_LEFT) {
                sgl_draw_vline(xc - x, yc + x + 1, y - x, color); // 3
                sgl_draw_hline(xc - y, yc + x, y - x, color);     // 4
            }
            if (dir & SGL_CIRCLE_UPPER_LEFT) {
                sgl_draw_hline(xc - y, yc - x, y - x, color); // 5
                sgl_draw_vline(xc - x, yc - y, y - x, color); // 6
            }
            if (dir & SGL_CIRCLE_UPPER_RIGHT) {
                sgl_draw_vline(xc + x, yc - y, y - x, color);     // 7
                sgl_draw_hline(xc + x + 1, yc - x, y - x, color); // 8
            }
        } else
            break;
    }
    if (x == y)
        ++x;
    for (i = 1; i < x; ++i) {
        if (dir & SGL_CIRCLE_LOWER_RIGHT)
            sgl_draw_point(xc + i, yc + i, color); // 1, 2
        if (dir & SGL_CIRCLE_LOWER_LEFT)
            sgl_draw_point(xc - i, yc + i, color); // 3, 4
        if (dir & SGL_CIRCLE_UPPER_LEFT)
            sgl_draw_point(xc - i, yc - i, color); // 5, 6
        if (dir & SGL_CIRCLE_UPPER_RIGHT)
            sgl_draw_point(xc + i, yc - i, color); // 7, 8
    }
}

void sgl_draw_circle(int32_t xc, int32_t yc, int32_t r, int32_t is_filled,
                     uint32_t color) {
    if (!r)
        return;
    else if (r < 0)
        r = -r;
    if (r == 1) {
        sgl_draw_point(xc, yc, color);
        return;
    }
    --r;
    if (__sgl_check_rect(xc - r, yc - r, xc + r, yc + r))
        return;
    if (!is_filled) {
        sgl_draw_point(xc + r, yc, color); // 8, 1
        sgl_draw_point(xc, yc + r, color); // 3, 2
        sgl_draw_point(xc - r, yc, color); // 5, 4
        sgl_draw_point(xc, yc - r, color); // 6, 7
        sgl_draw_outlined_circle_section(xc, yc, r, SGL_CIRCLE_ALL, color);
    } else {
        sgl_draw_point(xc, yc, color);         // xc, yc
        sgl_draw_hline(xc + 1, yc, r, color);  // 8, 1
        sgl_draw_vline(xc, yc + 1, r, color);  // 3, 2
        sgl_draw_hline(xc - 1, yc, -r, color); // 5, 4
        sgl_draw_vline(xc, yc - 1, -r, color); // 6, 7
        sgl_draw_filled_circle_section(xc, yc, r, SGL_CIRCLE_ALL, color);
    }
}

void sgl_draw_outlined_ellipse_section(int32_t xc, int32_t yc, int32_t rx,
                                       int32_t ry, sgl_circle_t dir,
                                       uint32_t color) {
    int32_t x = 0, y = ry, d1, d2;
    int32_t rx_2 = rx * rx;
    int32_t ry_2 = ry * ry;
    d1 = 4 * ry_2 + rx_2 * (1 - 4 * ry);
    while (2 * ry_2 * (x + 1) < rx_2 * (2 * y - 1)) {
        if (d1 <= 0) {
            d1 += 4 * ry_2 * (2 * x + 3);
            ++x;
        } else {
            d1 += 4 * (ry_2 * (2 * x + 3) + rx_2 * (-2 * y + 2));
            ++x;
            --y;
        }
        if (dir & SGL_CIRCLE_LOWER_RIGHT)
            sgl_draw_point(xc + x, yc + y, color);
        if (dir & SGL_CIRCLE_UPPER_RIGHT)
            sgl_draw_point(xc + x, yc - y, color);
        if (dir & SGL_CIRCLE_LOWER_LEFT)
            sgl_draw_point(xc - x, yc + y, color);
        if (dir & SGL_CIRCLE_UPPER_LEFT)
            sgl_draw_point(xc - x, yc - y, color);
    }
    d2 = ry_2 * (2 * x + 1) * (2 * x + 1) +
         4 * (rx_2 * (y - 1) * (y - 1) - rx_2 * ry_2);
    while (y > 0) {
        if (d2 <= 0) {
            d2 += 4 * (ry_2 * (2 * x + 2) + rx_2 * (-2 * y + 3));
            ++x;
            --y;
        } else {
            d2 += 4 * (rx_2 * (-2 * y + 3));
            --y;
        }
        if (dir & SGL_CIRCLE_LOWER_RIGHT)
            sgl_draw_point(xc + x, yc + y, color);
        if (dir & SGL_CIRCLE_UPPER_RIGHT)
            sgl_draw_point(xc + x, yc - y, color);
        if (dir & SGL_CIRCLE_LOWER_LEFT)
            sgl_draw_point(xc - x, yc + y, color);
        if (dir & SGL_CIRCLE_UPPER_LEFT)
            sgl_draw_point(xc - x, yc - y, color);
    }
}

void sgl_draw_filled_ellipse_section(int32_t xc, int32_t yc, int32_t rx,
                                     int32_t ry, sgl_circle_t dir,
                                     uint32_t color) {
    int32_t x = 0, y = ry, d1, d2;
    int32_t rx_2 = rx * rx;
    int32_t ry_2 = ry * ry;
    d1 = 4 * ry_2 + rx_2 * (1 - 4 * ry);
    while (2 * ry_2 * (x + 1) < rx_2 * (2 * y - 1)) {
        if (d1 <= 0) {
            d1 += 4 * ry_2 * (2 * x + 3);
            ++x;
            if (dir & SGL_CIRCLE_LOWER_RIGHT)
                sgl_draw_point(xc + x, yc + y, color);
            if (dir & SGL_CIRCLE_UPPER_RIGHT)
                sgl_draw_point(xc + x, yc - y, color);
            if (dir & SGL_CIRCLE_LOWER_LEFT)
                sgl_draw_point(xc - x, yc + y, color);
            if (dir & SGL_CIRCLE_UPPER_LEFT)
                sgl_draw_point(xc - x, yc - y, color);
        } else {
            d1 += 4 * (ry_2 * (2 * x + 3) + rx_2 * (-2 * y + 2));
            ++x;
            --y;
            sgl_draw_hline(xc + 1, yc + y, x, color);
            sgl_draw_hline(xc + 1, yc - y, x, color);
            sgl_draw_hline(xc - x, yc + y, x, color);
            sgl_draw_hline(xc - x, yc - y, x, color);
        }
    }
    d2 = ry_2 * (2 * x + 1) * (2 * x + 1) +
         4 * (rx_2 * (y - 1) * (y - 1) - rx_2 * ry_2);
    while (y > 0) {
        if (d2 <= 0) {
            d2 += 4 * (ry_2 * (2 * x + 2) + rx_2 * (-2 * y + 3));
            ++x;
            --y;
        } else {
            d2 += 4 * (rx_2 * (-2 * y + 3));
            --y;
        }
        sgl_draw_hline(xc + 1, yc + y, x, color);
        sgl_draw_hline(xc + 1, yc - y, x, color);
        sgl_draw_hline(xc - x, yc + y, x, color);
        sgl_draw_hline(xc - x, yc - y, x, color);
    }
}

void sgl_draw_ellipse(int32_t xc, int32_t yc, int32_t rx, int32_t ry,
                      int32_t is_filled, uint32_t color) {
    if (!rx || !ry)
        return;
    if (rx < 0)
        rx = -rx;
    if (ry < 0)
        ry = -ry;
    if (rx == 1 && ry == 1) {
        sgl_draw_point(xc, yc, color);
        return;
    }
    if (rx == 1) {
        sgl_draw_vline(xc, yc - ry + 1, 2 * ry - 1, color);
        return;
    } else if (ry == 1) {
        sgl_draw_hline(xc - rx + 1, yc, 2 * rx - 1, color);
        return;
    }
    --rx;
    --ry;
    if (__sgl_check_rect(xc - rx, yc - ry, xc + rx, yc + ry))
        return;
    if (!is_filled) {
        sgl_draw_point(xc + rx, yc, color); // 1, 2
        sgl_draw_point(xc, yc + ry, color); // 3, 4
        sgl_draw_point(xc - rx, yc, color); // 5, 6
        sgl_draw_point(xc, yc - ry, color); // 7, 8
        sgl_draw_outlined_ellipse_section(xc, yc, rx, ry, SGL_CIRCLE_ALL,
                                          color);
    } else {
        sgl_draw_point(xc, yc, color);          // (xc, yc)
        sgl_draw_hline(xc + 1, yc, rx, color);  // 8, 1
        sgl_draw_vline(xc, yc + 1, ry, color);  // 2, 3
        sgl_draw_hline(xc - 1, yc, -rx, color); // 4, 5
        sgl_draw_vline(xc, yc - 1, -ry, color); // 6, 7
        sgl_draw_filled_ellipse_section(xc, yc, rx, ry, SGL_CIRCLE_ALL, color);
    }
}

#include "sgl_bitmap.h"
#include "sgl_line.h"

void sgl_show_bitmap(int32_t x, int32_t y, int32_t w, int32_t h,
                     const char *bitmap, sgl_dir_t dir, uint32_t color) {
    int32_t offset_x, offset_y, temp, index, i, j;
    uint32_t mask;
    switch (dir) {
    case SGL_DIR_UP:
        offset_x = x;
        offset_y = y;
        for (j = 0; j < h; ++j) {
            index = (j >> 3) * w;
            mask = 1 << (j & 7);
            temp = offset_y + j;
            for (i = 0; i < w; ++i)
                if ((bitmap[i + index] & mask))
                    sgl_draw_point(offset_x + i, temp, color);
        }
        break;
    case SGL_DIR_RIGHT:
        offset_x = x + h - 1;
        offset_y = y;
        for (j = 0; j < h; ++j) {
            index = (j >> 3) * w;
            mask = 1 << (j & 7);
            temp = offset_x - j;
            for (i = 0; i < w; ++i)
                if ((bitmap[i + index] & mask))
                    sgl_draw_point(temp, offset_y + i, color);
        }
        break;
    case SGL_DIR_LEFT:
        offset_x = x;
        offset_y = y + w - 1;
        for (j = 0; j < h; ++j) {
            index = (j >> 3) * w;
            mask = 1 << (j & 7);
            temp = offset_x + j;
            for (i = 0; i < w; ++i)
                if ((bitmap[i + index] & mask))
                    sgl_draw_point(temp, offset_y - i, color);
        }
        break;
    case SGL_DIR_DOWN:
        offset_x = x + w - 1;
        offset_y = y + h - 1;
        for (j = 0; j < h; ++j) {
            index = (j >> 3) * w;
            mask = 1 << (j & 7);
            temp = offset_y - j;
            for (i = 0; i < w; ++i)
                if ((bitmap[i + index] & mask))
                    sgl_draw_point(offset_x - i, temp, color);
        }
        break;
    }
}

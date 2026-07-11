// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_CORE_H
#define __SGL_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SGL_FORMAT_STRING_BUFFERSIZE (128)

typedef enum {
    SGL_MONO_BLACK = 0,
    SGL_MONO_WHITE = 1,
    SGL_MONO_INVERT = 2,
} sgl_mono_color_t;

typedef enum {
    SGL_DIR_UP = 0,
    SGL_DIR_RIGHT,
    SGL_DIR_DOWN,
    SGL_DIR_LEFT,
} sgl_dir_t;

#define SGL_DIR_DEFAULT SGL_DIR_UP

/**
 * alignment:
 *
 *     up_left           up_center           up_right
 *             +-------------+-------------+
 *             |                           |
 *             |                           |
 * left_center +        center(x,y)        + right_center
 *             |                           |
 *             |                           |
 *             +-------------+-------------+
 *   down_left          down_center          down_right
 */

typedef enum {
    SGL_ALIGN_UP_LEFT = 0,
    SGL_ALIGN_UP_RIGHT,
    SGL_ALIGN_DOWN_LEFT,
    SGL_ALIGN_DOWN_RIGHT,
    SGL_ALIGN_CENTER,
    SGL_ALIGN_UP_CENTER,
    SGL_ALIGN_DOWN_CENTER,
    SGL_ALIGN_LEFT_CENTER,
    SGL_ALIGN_RIGHT_CENTER,
} sgl_align_t;

#define SGL_ALIGN_DEFAULT SGL_ALIGN_UP_LEFT

typedef enum {
    SGL_ROTATE_0 = 0,
    SGL_ROTATE_90,
    SGL_ROTATE_180,
    SGL_ROTATE_270,
} sgl_rotate_t;

#define SGL_ROTATE_DEFAULT SGL_ROTATE_0

typedef enum {
    SGL_SLICE_STATUS_IDLE,
    SGL_SLICE_STATUS_START,
    SGL_SLICE_STATUS_RUNNIG,
} sgl_slice_status_t;

typedef enum {
    SGL_COLOR_FORMAT_MONO,
    SGL_COLOR_FORMAT_RGB332,
    SGL_COLOR_FORMAT_RGB565,
    SGL_COLOR_FORMAT_RGB565SWAP,
    SGL_COLOR_FORMAT_BGR565,
    SGL_COLOR_FORMAT_RGB888,
    SGL_COLOR_FORMAT_BGR888,
    SGL_COLOR_FORMAT_XRGB8888,
    SGL_COLOR_FORMAT_XBGR8888,
    SGL_COLOR_FORMAT_ARGB8888,
    SGL_COLOR_FORMAT_ABGR8888,
    SGL_COLOR_FORMAT_RGBA8888,
    SGL_COLOR_FORMAT_BGRA8888,
} sgl_color_format_t;

typedef struct {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} sgl_rect_t;

typedef struct sgl_screen sgl_screen_t;

struct sgl_screen {
    void *buffer;
    uint32_t buffer_size;
    uint32_t pixel_num;
    uint32_t hor_res;
    uint32_t ver_res;
    uint32_t max_x;
    uint32_t max_y;
    uint32_t offset_x;
    uint32_t offset_y;
    sgl_rect_t drawable;
    sgl_rect_t slice;
    sgl_rect_t dirty;
    uint32_t w_dirty;
    uint32_t h_dirty;
    uint32_t slice_count;
    uint32_t frame_count;
    sgl_slice_status_t slice_status;
    sgl_rotate_t rotate;
    void (*draw)(sgl_screen_t *scr);
    void (*flush)(void *buffer, sgl_rect_t *refresh);
    void (*draw_pixel)(sgl_screen_t *scr, int32_t x, int32_t y, uint32_t color);
};

int sgl_init(sgl_screen_t *scr, void *buffer, uint32_t buffer_size,
             sgl_color_format_t color_format, uint32_t hor_res,
             uint32_t ver_res);
void sgl_handler(sgl_screen_t *scr);
void sgl_set_draw(sgl_screen_t *scr, void (*draw)(sgl_screen_t *scr));
void sgl_set_flush(sgl_screen_t *scr,
                   void (*flush)(void *buffer, sgl_rect_t *refresh));
int sgl_set_draw_pixel(sgl_screen_t *scr,
                       void (*draw_pixel)(sgl_screen_t *scr, int32_t x,
                                          int32_t y, uint32_t color),
                       uint32_t buffer_size, uint32_t pixel_size);
void sgl_set_drawable(sgl_screen_t *scr, int32_t left, int32_t top,
                      int32_t right, int32_t bottom);
void sgl_reset_drawable(sgl_screen_t *scr);
void sgl_set_screen_rotation(sgl_screen_t *scr, sgl_rotate_t rotate);
uint32_t sgl_get_frame_count(sgl_screen_t *scr);
void sgl_reset_frame_count(sgl_screen_t *scr);
void sgl_clear_screen(sgl_screen_t *scr, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif

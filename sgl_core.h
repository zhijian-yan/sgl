// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_CORE_H
#define __SGL_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_config.h"

#define SGL_COLOR_DARK 0
#define SGL_COLOR_LIGHT 1
#define SGL_COLOR_INVERSE 2

typedef struct {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} sgl_rect_t;

typedef enum {
    SGL_DIR_UP = 0,
    SGL_DIR_DOWN,
    SGL_DIR_LEFT,
    SGL_DIR_RIGHT,
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
 *
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
    SGL_CIRCLE_UPPER_RIGHT = 0x01,
    SGL_CIRCLE_UPPER_LEFT = 0x02,
    SGL_CIRCLE_LOWER_RIGHT = 0x04,
    SGL_CIRCLE_LOWER_LEFT = 0x08,
    SGL_CIRCLE_ALL = 0xFF,
} sgl_circle_t;

typedef struct sgl_screen *sgl_screen_handle_t;

sgl_screen_handle_t sgl_create_screen(void *buffer, uint32_t buffer_size,
                                      uint32_t hor_res, uint32_t ver_res);
void sgl_delete_screen(sgl_screen_handle_t new_screen);
void sgl_handler(void);
void sgl_set_screen(sgl_screen_handle_t screen);
void sgl_set_buffer(void *buffer, uint32_t buffer_size);
void sgl_set_paint(void (*paint)());
void sgl_set_flush(void (*flush)(void *buffer, uint32_t buffer_size));
void sgl_set_draw_pixel(void (*draw_pixel)(int32_t x, int32_t y,
                                           uint32_t color));
void sgl_set_visible(int32_t left, int32_t top, int32_t right, int32_t bottom);
uint32_t sgl_get_fcount(void);
void sgl_reset_fcount(void);
void sgl_clear_screen(uint8_t value);

#ifdef __cplusplus
}
#endif

#endif

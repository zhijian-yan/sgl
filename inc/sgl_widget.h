// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Zhijian Yan

#ifndef __SGL_WIDGET_H
#define __SGL_WIDGET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sgl_types.h"

int sgl_widget_init(sgl_widget_t *widget,
                    void (*draw)(sgl_screen_t *scr, void *user_data),
                    void *user_data);
int sgl_widget_register_draw(sgl_widget_t *widget,
                             void (*draw)(sgl_screen_t *scr, void *user_data),
                             void *user_data);
int sgl_widget_add(sgl_widget_t *widget, sgl_widget_t *parent);
int sgl_widget_remove(sgl_widget_t *widget);

#ifdef __cplusplus
}
#endif

#endif

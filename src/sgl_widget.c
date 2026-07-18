// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Zhijian Yan

#include "../inc/sgl_widget.h"
#include "../inc/sgl_common.h"
#include <string.h>

static sgl_widget_draw_t sgl_widget_draw[SGL_WIDGET_MAX_DRAW_INDEX];
static uint32_t draw_count = 0;

int sgl_widget_init(sgl_widget_t *widget, uint16_t draw_index, int32_t x,
                    int32_t y, int32_t w, int32_t h, void *user_data) {
    if (!widget)
        return -1;
    memset(widget, 0, sizeof(sgl_widget_t));
    INIT_LIST_HEAD(&widget->sibling);
    INIT_LIST_HEAD(&widget->children);
    sgl_set_rect(&widget->rect, x, y, w, h);
    widget->draw_index = draw_index;
    widget->user_data = user_data;
    return 0;
}

void sgl_widget_set_draw(sgl_widget_t *widget, uint16_t draw_index,
                         void *user_data) {
    widget->draw_index = draw_index;
    widget->user_data = user_data;
}

void sgl_widget_set_rect(sgl_widget_t *widget, int32_t x, int32_t y, int32_t w,
                         int32_t h) {
    sgl_set_rect(&widget->rect, x, y, w, h);
}

int sgl_widget_register_draw(sgl_widget_draw_t draw) {
    int ret = draw_count;
    if (draw_count == SGL_WIDGET_MAX_DRAW_INDEX)
        return -1;
    sgl_widget_draw[draw_count] = draw;
    ++draw_count;
    return ret;
}

sgl_widget_draw_t sgl_widget_get_draw(uint32_t draw_index) {
    if (draw_index < draw_count)
        return sgl_widget_draw[draw_index];
    return NULL;
}

int sgl_widget_add(sgl_widget_t *widget, sgl_widget_t *parent) {
    if (widget == parent)
        return -1;
    if (widget->parent)
        return -1;
    widget->parent = parent;
    list_add_tail(&widget->sibling, &parent->children);
    return 0;
}

int sgl_widget_remove(sgl_widget_t *widget) {
    if (!widget->parent)
        return -1;
    list_del(&widget->sibling);
    widget->parent = NULL;
    return 0;
}

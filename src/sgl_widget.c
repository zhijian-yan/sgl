// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Zhijian Yan

#include "../inc/sgl_widget.h"
#include <string.h>

int sgl_widget_init(sgl_widget_t *widget,
                    void (*draw)(sgl_screen_t *scr, void *user_data),
                    void *user_data) {
    if (!widget)
        return -1;
    memset(widget, 0, sizeof(sgl_widget_t));
    INIT_LIST_HEAD(&widget->sibling);
    INIT_LIST_HEAD(&widget->children);
    widget->draw = draw;
    widget->user_data = user_data;
    return 0;
}

int sgl_widget_register_draw(sgl_widget_t *widget,
                             void (*draw)(sgl_screen_t *scr, void *user_data),
                             void *user_data) {
    // Assertions need to be added.
    if (!widget)
        return -1;
    widget->draw = draw;
    widget->user_data = user_data;
    return 0;
}

int sgl_widget_add(sgl_widget_t *widget, sgl_widget_t *parent) {
    // Assertions need to be added.
    if (!widget || !parent)
        return -1;
    if (widget == parent)
        return -1;
    if (widget->parent)
        return -1;
    widget->parent = parent;
    list_add_tail(&widget->sibling, &parent->children);
    return 0;
}

int sgl_widget_remove(sgl_widget_t *widget) {
    // Assertions need to be added.
    if (!widget)
        return -1;
    if (!widget->parent)
        return -1;
    list_del(&widget->sibling);
    widget->parent = NULL;
    return 0;
}

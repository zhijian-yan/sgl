// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Zhijian Yan

#ifndef __SGL_CONFIG_H
#define __SGL_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#define sgl_malloc(size) malloc(size)
#define sgl_free(ptr) free(ptr)
#define SGL_FORMAT_STRING_BUFFERSIZE (128)

#ifdef __cplusplus
}
#endif

#endif

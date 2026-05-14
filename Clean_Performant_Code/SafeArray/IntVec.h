#pragma once

/**
 * Copyright (c) 2026 PeLo
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef INT_VEC_H
#define INT_VEC_H

#include <stdlib.h>
#include <string.h>

#define INT_VEC_VERSION "0.0.1"

// -------------------------------------------------------------------------
// types

struct int_vec_
{
    int* data; 
    size_t length;
    size_t capacity;
};

typedef struct int_vec_ int_vec;

// -------------------------------------------------------------------------
// functions

// lifecycle
extern void int_vec_init(int_vec* ptr);
extern void int_vec_free(int_vec* ptr);

// capacity management
extern int int_vec_reserve(int_vec* vec, size_t new_capacity);
extern int int_vec_resize(int_vec* vec, size_t new_length);
extern int int_vec_shrink_to_fit(int_vec* vec);

// element operations
extern int int_vec_push(int_vec* ptr, int val);
// extern int int_vec_pop(int_vec* ptr);
extern int int_vec_pop(int_vec* vec, int* out_value);

// vector based operations
extern int int_vec_insert(int_vec* vec, size_t index, int value);
extern int int_vec_remove(int_vec* vec, size_t index);

// access
// extern int* int_vec_get(int_vec* vec, size_t index);
const int* int_vec_get(const int_vec* vec, size_t index);   // geht das mit dem const ?????????
extern int int_vec_set(int_vec* vec, size_t index, int value);
extern int int_vec_front(const int_vec* vec, int* out);
extern int int_vec_back(const int_vec* vec, int* out);

// iterator design
extern int* int_vec_next(const int_vec* vec, size_t* index);

// miscellaneous
extern void int_vec_clear(int_vec* ptr);
extern void int_vec_truncate(int_vec* ptr, size_t len);

// TBD:
// find
//vec_foreach_rev

// -------------------------------------------------------------------------

#endif

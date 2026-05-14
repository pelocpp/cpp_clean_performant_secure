#pragma once

/**
 * Copyright (c) 2014 rxi
 * Modified (c) 2026 PeLo
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

struct int_vec_
{
    int* data; 
    size_t length;
    size_t capacity;
};

typedef struct int_vec_ int_vec;

// -------------------------------------------------------------------------

// Lifecycle
extern void int_vec_init(int_vec* ptr);
extern void int_vec_free(int_vec* ptr);

// Capacity management
extern int int_vec_reserve(int_vec* vec, size_t new_capacity);
extern int int_vec_resize(int_vec* vec, size_t new_length);

// Element operations
extern int int_vec_push(int_vec* ptr, int val);
// extern int int_vec_pop(int_vec* ptr);
extern int int_vec_pop(int_vec* vec, int* out_value);

extern int int_vec_insert(int_vec* vec, size_t index, int value);
extern int int_vec_remove(int_vec* vec, size_t index);

// Access
int* int_vec_get(int_vec* vec, size_t index);
// const int* int_vec_get(int_vec* vec, size_t index);   // geht das mit dem const ?????????
int int_vec_set(int_vec* vec, size_t index, int value);

// Simplest Iterator Design
int* int_vec_next(int_vec* vec, size_t* index);

// Simplest Shrink API
int int_vec_shrink_to_fit(int_vec* vec);


// Obsolete
extern void int_vec_clear(int_vec* ptr);
extern void int_vec_truncate(int_vec* ptr, int len);

// -------------------------------------------------------------------------







// hmmm, die könnten alle static sein
// das mit dem trailing _ steht wohl für lokal ?!?!=?!
//               why not void** data,
int  int_vec_expand_(char** data, size_t * length, size_t* capacity, size_t memsz);

int  int_vec_expand_(char** data, size_t* length, size_t* capacity, size_t memsz);
int  int_vec_reserve_(char** data, size_t* length, size_t* capacity, size_t memsz, int n);
int  int_vec_reserve_po2_(char** data, size_t* length, size_t* capacity, size_t memsz, int n);
int  int_vec_compact_(char** data, size_t* length, size_t* capacity, size_t memsz);
int  int_vec_insert_(char** data, size_t* length, size_t* capacity, size_t memsz, size_t idx);
void int_vec_splice_(char** data, size_t* length, size_t* capacity, size_t memsz, int start, int count);
void int_vec_swapsplice_(char** data, size_t* length, size_t* capacity, size_t memsz, int start, int count);
void int_vec_swap_(char** data, size_t* length, size_t* capacity, size_t memsz, size_t idx1, size_t idx2);

#endif


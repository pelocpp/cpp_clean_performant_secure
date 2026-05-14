// ===========================================================================
// IntVec.c // Small Safe Array Library
// ===========================================================================

/**
 * Copyright (c) 2026 PeLo
 */

#ifndef INT_VEC_H
#define INT_VEC_H

#include <stdlib.h>
#include <string.h>

// -------------------------------------------------------------------------
// defines

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
extern void int_vec_init          (int_vec* ptr);
extern void int_vec_free          (int_vec* ptr);

// capacity management
extern int  int_vec_reserve       (int_vec* vec, size_t new_capacity);
extern int  int_vec_resize        (int_vec* vec, size_t new_length);
extern int  int_vec_shrink_to_fit (int_vec* vec);
extern void int_vec_clear         (int_vec* ptr);
extern void int_vec_truncate      (int_vec* ptr, size_t len);

// vector based operations
extern int int_vec_push           (int_vec* ptr, int val);
extern int int_vec_pop            (int_vec* vec, int* out_value); 
extern int int_vec_insert         (int_vec* vec, size_t index, int value);
extern int int_vec_remove         (int_vec* vec, size_t index);

// element access / queries
extern const int* int_vec_get     (const int_vec* vec, size_t index);
extern       int  int_vec_set     (      int_vec* vec, size_t index, int value);
extern       int  int_vec_front   (const int_vec* vec, int* out);
extern       int  int_vec_back    (const int_vec* vec, int* out);
extern       int  int_vec_find    (const int_vec* vec, int value, size_t* index_ptr);

// iterator design
extern const int* int_vec_next    (const int_vec* vec, size_t* index);
extern const int* int_vec_prev    (const int_vec* vec, size_t* index_ptr);

#endif

// ===========================================================================
// End-of-File
// ===========================================================================

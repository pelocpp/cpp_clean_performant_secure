#pragma once

/**
 * Copyright (c) 2014 rxi
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
//               why not void** data,
int int_vec_expand_(char** data, int* length, int* capacity, int memsz);

// Warum steht das oben ???
//#define vec_unpack_(v)\
//  (char**)&(v)->data, &(v)->length, &(v)->capacity, sizeof(*(v)->data)

// Hmmmmmmmmm, das muss entweder ein Makro bleiben ... oder man lässt es weg
// Es werden 4 Werte ausgepackt ...
//void vec_unpack_(int_vec* ptr) {
//    (char**)&(ptr)->data, ptr->length, ptr->capacity, sizeof(*(ptr)->data);
//}

// -------------------------------------------------------------------------
//#define int_vec (T)\
//  struct { int *data; int length, capacity; }

typedef struct { int* data; int length; int capacity; } int_vec;

// -------------------------------------------------------------------------
//#define vec_init(v)\
//  memset((v), 0, sizeof(*(v)))

//void int_vec_init(int_vec* ptr)
//{
//    memset(ptr, 0, sizeof(*(ptr)));   // sizeof testen ...
//}

extern void int_vec_init(int_vec* ptr);

// -------------------------------------------------------------------------
//#define vec_deinit(v)\
//  ( free((v)->data),\
//    vec_init(v) ) 

//void int_vec_deinit(int_vec* ptr)
//{
//    free(ptr->data);
//    int_vec_init(ptr);
//}

extern void int_vec_deinit(int_vec* ptr);

// -------------------------------------------------------------------------
//#define vec_push(v, val)\
//  ( vec_expand_(vec_unpack_(v)) ? -1 :\
//    ((v)->data[(v)->length++] = (val), 0), 0 )

//int int_vec_push(int_vec* ptr, int val)
//{
//    // int int_vec_expand_(char** data, int* length, int* capacity, int memsz) {
//
//
//   // (int_vec_expand_(vec_unpack_(ptr)) ? -1 : ((ptr)->data[(ptr)->length++] = (val), 0), 0);
//
//    return (int_vec_expand_((char**)&(ptr)->data, &ptr->length, &ptr->capacity, sizeof(*(ptr)->data)) ? -1 : ((ptr)->data[(ptr)->length++] = (val), 0), 0);
//}

extern int int_vec_push(int_vec* ptr, int val);

// -------------------------------------------------------------------------
//#define vec_pop(v)\
//  (v)->data[--(v)->length]

//int vec_pop(int_vec* ptr)
//{
//    return ptr->data[--(ptr->length)];   // Operatoren vorang !!!
//}

int vec_pop(int_vec* ptr);

// -------------------------------------------------------------------------
 // added: PeLo
#define vec_at(v, i) \
  ((i) >= 0 && (i) < (v).length ? (v).data[i] : (abort(), (v).data[0]))


// added: PeLo
// Note: To create a safe "setter" in C, typically a do - while (0) macro pattern is used.
// This is a standard C idiom that allows the macro to behave like a single statement,
// making it safe to use inside if / else blocks without causing syntax errors.
// This ensures the macro always requires a semicolon and acts as a single block.
// Without it, an if (condition) vec_set_safe(...); else ... might break
// because the else would attach to the internal if of the macro.
//
#define vec_set_safe(v, i, val) do {               \
    if ((i) >= 0 && (i) < (v).length) {            \
        (v).data[i] = (val);                       \
    } else {                                       \
        fprintf(stderr, "Index %d out of bounds for length %d\n", (int)(i), (v).length); \
        abort();                                   \
    }                                              \
} while (0)


// added: PeLo
// Alternative: A "Silent" Setter
// Usage: if (!vec_try_set(v, 10, 5)) { /* handle error */ }
//
#define vec_try_set(v, i, val) \
    ((i) >= 0 && (i) < (v).length ? ((v).data[i] = (val), 1) : 0)


#define vec_splice(v, start, count)\
  ( vec_splice_(vec_unpack_(v), start, count),\
    (v)->length -= (count) )


#define vec_swapsplice(v, start, count)\
  ( vec_swapsplice_(vec_unpack_(v), start, count),\
    (v)->length -= (count) )


#define vec_insert(v, idx, val)\
  ( vec_insert_(vec_unpack_(v), idx) ? -1 :\
    ((v)->data[idx] = (val), 0), (v)->length++, 0 )


#define vec_sort(v, fn)\
  qsort((v)->data, (v)->length, sizeof(*(v)->data), fn)


#define vec_swap(v, idx1, idx2)\
  vec_swap_(vec_unpack_(v), idx1, idx2)


#define vec_truncate(v, len)\
  ((v)->length = (len) < (v)->length ? (len) : (v)->length)

// -------------------------------------------------------------------------
//#define vec_clear(v)\
//  ((v)->length = 0)

//void vec_clear(int_vec* ptr)
//{
//    ptr->length = 0;  // hmm, und was ist mit free ???????????????
//}

extern void vec_clear(int_vec* ptr);

// -------------------------------------------------------------------------
//#define vec_first(v)\
//  (v)->data[0]

//int first(int_vec* ptr)
//{
//    return ptr->data[0];
//}

extern int first(int_vec* ptr);

#define vec_last(v)\
  (v)->data[(v)->length - 1]


#define vec_reserve(v, n)\
  vec_reserve_(vec_unpack_(v), n)


#define vec_compact(v)\
  vec_compact_(vec_unpack_(v))


#define vec_pusharr(v, arr, count)\
  do {\
    int i__, n__ = (count);\
    if (vec_reserve_po2_(vec_unpack_(v), (v)->length + n__) != 0) break;\
    for (i__ = 0; i__ < n__; i__++) {\
      (v)->data[(v)->length++] = (arr)[i__];\
    }\
  } while (0)


#define vec_extend(v, v2)\
  vec_pusharr((v), (v2)->data, (v2)->length)


#define vec_find(v, val, idx)\
  do {\
    for ((idx) = 0; (idx) < (v)->length; (idx)++) {\
      if ((v)->data[(idx)] == (val)) break;\
    }\
    if ((idx) == (v)->length) (idx) = -1;\
  } while (0)


#define vec_remove(v, val)\
  do {\
    int idx__;\
    vec_find(v, val, idx__);\
    if (idx__ != -1) vec_splice(v, idx__, 1);\
  } while (0)


#define vec_reverse(v)\
  do {\
    int i__ = (v)->length / 2;\
    while (i__--) {\
      vec_swap((v), i__, (v)->length - (i__ + 1));\
    }\
  } while (0)


#define vec_foreach(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = 0;\
        (iter) < (v)->length && (((var) = (v)->data[(iter)]), 1);\
        ++(iter))


#define vec_foreach_rev(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = (v)->length - 1;\
        (iter) >= 0 && (((var) = (v)->data[(iter)]), 1);\
        --(iter))


#define vec_foreach_ptr(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = 0;\
        (iter) < (v)->length && (((var) = &(v)->data[(iter)]), 1);\
        ++(iter))


#define vec_foreach_ptr_rev(v, var, iter)\
  if  ( (v)->length > 0 )\
  for ( (iter) = (v)->length - 1;\
        (iter) >= 0 && (((var) = &(v)->data[(iter)]), 1);\
        --(iter))


// hmmm, die könnten alle static sein
// das mit dem trailing _ steht wohl für lokal ?!?!=?!
int  int_vec_expand_(char** data, int* length, int* capacity, int memsz);
int  int_vec_reserve_(char** data, int* length, int* capacity, int memsz, int n);
int  int_vec_reserve_po2_(char** data, int* length, int* capacity, int memsz, int n);
int  int_vec_compact_(char** data, int* length, int* capacity, int memsz);
int  int_vec_insert_(char** data, int* length, int* capacity, int memsz, int idx);
void int_vec_splice_(char** data, int* length, int* capacity, int memsz, int start, int count);
void int_vec_swapsplice_(char** data, int* length, int* capacity, int memsz, int start, int count);
void int_vec_swap_(char** data, int* length, int* capacity, int memsz, int idx1, int idx2);


//typedef vec_t(void*) vec_void_t;
//typedef vec_t(char*) vec_str_t;
//typedef vec_t(int) vec_int_t;
//typedef vec_t(char) vec_char_t;
//typedef vec_t(float) vec_float_t;
//typedef vec_t(double) vec_double_t;

#endif

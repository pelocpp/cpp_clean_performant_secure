#include "IntVec.h"

// ----------------------------------------------------------------------
// helper functions

int int_vec_expand_(char** data, size_t* length, size_t* capacity, size_t memsz) {
    if (*length + 1 > *capacity) {
        void* ptr;
        size_t n = (*capacity == 0) ? 1 : *capacity << 1;
        // printf(">>>>>>>>>>>>> Old: %d - New: %d\n", *capacity, n );
        ptr = realloc(*data, n * memsz);
        if (ptr == NULL) return -1;
        *data = ptr;
        *capacity = n;
    }
    return 0;
}

int int_vec_insert_(char** data, size_t* length, size_t* capacity, size_t memsz, size_t idx) {
    int err = int_vec_expand_(data, length, capacity, memsz);
    if (err) return err;
    memmove(*data + (idx + 1) * memsz,
        *data + idx * memsz,
        (*length - idx) * memsz);
    return 0;
}

// ----------------------------------------------------------------------
// int_vec API

void int_vec_init(int_vec* vec)
{
    // memset(vec, 0, sizeof(int_vec));  // evtl, schneller

    vec->data = NULL;
    vec->length = 0;
    vec->capacity = 0;
}

void int_vec_free(int_vec* vec)
{
    free(vec->data);

    int_vec_init(vec);
}

// ----------------------------------------------------------------------

/*
** Ensure capacity is at least new_capacity
*/
int int_vec_reserve(int_vec* vec, size_t new_capacity)
{
   // int* new_data;

    if (new_capacity <= vec->capacity) {
        return 0;
    }

    int* new_data = realloc(vec->data, new_capacity * sizeof(int));
    if (new_data == NULL) {
        return -1;
    }

    vec->data = new_data;
    vec->capacity = new_capacity;

    return 0;
}


/*
** Resize logical length
*/
int int_vec_resize(int_vec* vec, size_t new_length)
{
    size_t new_capacity;

    if (new_length > vec->capacity) {

        /*
        ** Growth strategy:
        ** double until large enough
        */
        new_capacity = vec->capacity ? vec->capacity : 1;

        while (new_capacity < new_length) {
            new_capacity *= 2;
        }

        if (int_vec_reserve(vec, new_capacity) != 0) {
            return -1;
        }
    }


    /*
    ** Zero initialize newly created elements
    */
    if (new_length > vec->length) {

        memset(
            vec->data + vec->length,
            0,
            (new_length - vec->length) * sizeof(int)
        );
    }

    vec->length = new_length;

    return 0;
}

// ----------------------------------------------------------------------

int int_vec_push_old(int_vec* ptr, int val)
{
    int ret = int_vec_expand_((char**)&(ptr)->data, &ptr->length, &ptr->capacity, sizeof(*(ptr)->data));

    if (ret != 0) {
        return -1;
    }
    else {
        ptr->data[ptr->length] = val;   // zerlegen .............
        ptr->length++;
        return 0;
    }
}

//int int_vec_pop(int_vec* ptr)
//{
//   // return ptr->data[--(ptr->length)];   // Operatoren vorang !!!
//
//    --(ptr->length);
//    return ptr->data[ptr->length];
//}


//  Push value to end
int int_vec_push(int_vec* vec, int value)
{
    if (vec->length == vec->capacity) {

        size_t new_capacity =
            vec->capacity ? vec->capacity * 2 : 1;

        if (int_vec_reserve(vec, new_capacity) != 0) {
            return -1;
        }
    }

    vec->data[vec->length] = value;
    vec->length++;

    return 0;
}

//  Remove last element
int int_vec_pop(int_vec* vec, int* out_value)
{
    if (vec->length == 0) {
        return -1;
    }

    vec->length--;

    if (out_value != NULL) {
        *out_value = vec->data[vec->length];
    }

    return 0;
}

// ----------------------------------------------------------------------


// Insert value at index
int int_vec_insert(int_vec* ptr, size_t idx, int val)
{

    // int vec_insert_(char** data, size_t* length, size_t* capacity, int memsz, int idx) {

    int ret = int_vec_insert_((char**)&(ptr)->data, &ptr->length, &ptr->capacity, sizeof(*(ptr)->data), idx);

    if (ret != 0) {
        return -1;
    }
    else {
        ptr->data[idx] = val;
        ptr->length++;
        return 0;
    }
}

// Chat GPT
int int_vec_insertZWEI(int_vec* vec, size_t index, int value)
{
    if (index > vec->length) {
        return -1;
    }

    if (int_vec_push(vec, 0) != 0) {
        return -1;
    }

    memmove(
        &vec->data[index + 1],
        &vec->data[index],
        (vec->length - index - 1) * sizeof(int)
    );

    vec->data[index] = value;

    return 0;
}

// Remove value at index
// Chat GPT
// Die Version von rxi ist ein remove Value, not at index....
int int_vec_remove(int_vec* vec, size_t index)
{
    if (index >= vec->length) {
        return -1;
    }

    memmove(
        &vec->data[index],
        &vec->data[index + 1],
        (vec->length - index - 1) * sizeof(int)
    );

    vec->length--;

    return 0;
}

// Remove all elements
void int_vec_clear(int_vec * vec)
{
    vec->length = 0;
}

/*
** Safe element access
*/
int* int_vec_get(int_vec* vec, size_t index)
{
    if (index >= vec->length) {
        return NULL;
    }

    return &vec->data[index];
}

/*
** Safe element assignment
*/
int int_vec_set(int_vec* vec, size_t index, int value)
{
    if (index >= vec->length) {
        return -1;
    }

    vec->data[index] = value;
    return 0;
}


// ----------------------------------------------------------------------

/*
** Iterator helper
**
** Example:
**
** size_t i = 0;
** int *value;
**
** while ((value = int_vec_next(&vec, &i))) {
**     printf("%d\n", *value);
** }
*/
int* int_vec_next(int_vec* vec, size_t* index)
{
    int* value;

    if (*index >= vec->length) {
        return NULL;
    }

    value = &vec->data[*index];

    (*index)++;

    return value;
}

// ----------------------------------------------------------------------

// Reduce capacity to exactly fit length
int int_vec_shrink_to_fit(int_vec* vec)
{
    int* new_data;

    /*
    ** Special case:
    ** empty vector
    */
    if (vec->length == 0) {

        free(vec->data);

        vec->data = NULL;
        vec->capacity = 0;

        return 0;
    }

    new_data = realloc(
        vec->data,
        vec->length * sizeof(int)
    );

    /*
    ** realloc failure:
    ** original allocation remains valid
    */
    if (new_data == NULL) {
        return -1;
    }

    vec->data = new_data;
    vec->capacity = vec->length;

    return 0;
}


// ----------------------------------------------------------------------


void int_vec_truncate(int_vec* ptr, int len)
{
    ptr->length = (len < ptr->length) ? len : ptr->length;
}

int first(int_vec* ptr)
{
    return ptr->data[0];
}



//
//void int_vec_foreach(int_vec* ptr, int* varPtr)
//{
//    if (ptr->length > 0) {
//
//        for (int i = 0; i < ptr->length; ++i) {
//            *varPtr = ptr->data[i];
//        }
//    }
//}
//
////#define vec_foreach(v, var, iter)\
////  if  ( (v)->length > 0 )\
////  for ( (iter) = 0;\
////        (iter) < (v)->length && (((var) = (v)->data[(iter)]), 1);\
////        ++(iter))

#include "IntVec.h"

#include <limits.h>

// ----------------------------------------------------------------------
// lifecycle

// constructor
void int_vec_init(int_vec* vec)
{
    if (vec == NULL) { 
        return;
    }

    vec->data = NULL;
    vec->length = 0;
    vec->capacity = 0;
}

// destructor
void int_vec_free(int_vec* vec)
{
    if (vec == NULL) {
        return;
    }

    free(vec->data);

    int_vec_init(vec);
}

// reduce capacity to exactly fit length
int int_vec_shrink_to_fit(int_vec* vec)
{
    if (vec == NULL) { 
        return -1;
    }

    // nothing to do
    if (vec->capacity == vec->length) {
        return 0;
    }

    // special case: empty vector
    if (vec->length == 0) {

        free(vec->data);

        vec->data = NULL;
        vec->capacity = 0;

        return 0;
    }

    // guard against overflow when computing bytes
    if (vec->length > SIZE_MAX / sizeof(int)) {
        return -1;
    }

    size_t new_bytes = vec->length * sizeof(int);
    int* new_data = realloc(vec->data, new_bytes);

    if (new_data == NULL) {
        // realloc failure: original allocation is still valid
        return -1;
    }

    vec->data = new_data;
    vec->capacity = vec->length;

    return 0;
}

// ----------------------------------------------------------------------

// ensure capacity is at least new_capacity
int int_vec_reserve(int_vec* vec, size_t new_capacity)
{
    if (vec == NULL) {
        return -1;
    }

    if (new_capacity > SIZE_MAX / sizeof(int)) {
        return -1;
    }

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


// resize logical length
int int_vec_resize(int_vec* vec, size_t new_length)
{
    if (vec == NULL) {
        return -1;
    }

    size_t new_capacity;

    if (new_length > vec->capacity) {

        // growth strategy: double until large enough, but avoid overflow
        new_capacity = vec->capacity ? vec->capacity : 1;

        while (new_capacity < new_length) {

            if (new_capacity > SIZE_MAX / 2) {

                /* can't safely double - use exact required size */
                new_capacity = new_length;
                break;
            }

            new_capacity *= 2;
        }

        if (new_capacity > SIZE_MAX / sizeof(int)) {
            return -1;
        }
        
        if (int_vec_reserve(vec, new_capacity) != 0) {
            return -1;
        }
    }

    // guard against overflow when computing bytes
    size_t diff = new_length - vec->length;
    
    if (diff > 0) {

        if (diff > SIZE_MAX / sizeof(int)) {
            return -1;
        }

        // zero initialize newly created elements
        memset(
            vec->data + vec->length,
            0,
            diff * sizeof(int)
        );
    }

    vec->length = new_length;

    return 0;
}

// ----------------------------------------------------------------------

//  push value to end
int int_vec_push(int_vec* vec, int value)
{
    if (vec == NULL) {
        return -1;
    }

    if (vec->length == vec->capacity) {

        if (vec->capacity == SIZE_MAX) { // cannot grow
            return -1;
        }

        size_t new_capacity = vec->capacity ? vec->capacity * 2 : 1;

        // avoid overflow and ensure at least length + 1
        if (new_capacity < vec->length + 1) {
            new_capacity = vec->length + 1;
        }

        if (int_vec_reserve(vec, new_capacity) != 0) {
            return -1;
        }
    }

    vec->data[vec->length] = value;
    vec->length++;

    return 0;
}

//  remove last element
int int_vec_pop(int_vec* vec, int* out_value)
{
    if (vec == NULL) {
        return -1;
    }

    if (vec->length == 0) {
        return -1;
    }

    vec->length--;

    if (out_value != NULL) {
        *out_value = vec->data[vec->length];
    }

    // optional: clear freed slot for debuggability / security
    vec->data[vec->length] = 0;

    return 0;
}

// ----------------------------------------------------------------------


// insert value at index
int int_vec_insert(int_vec* vec, size_t index, int value)
{
    if (vec == NULL) {
        return -1;
    }

    // index may be equal to length (append) but not greater
    if (index > vec->length) {
        return -1;
    }

    // can't grow beyond SIZE_MAX elements
    if (vec->length == SIZE_MAX) {
        return -1;
    }

    size_t required = vec->length + 1;

    if (required > vec->capacity) {

        size_t new_capacity = vec->capacity ? vec->capacity : 1;

        // double until large enough, but avoid overflow
        while (new_capacity < required) {

            if (new_capacity > SIZE_MAX / 2) {

                // can't safely double - use exact required size
                new_capacity = required;
                break;
            }

            new_capacity *= 2;
        }

        if (int_vec_reserve(vec, new_capacity) != 0) {
            return -1;
        }
    }

    // if inserting not at the end, shift tail elements right by one
    if (index < vec->length) {
        memmove(
            vec->data + index + 1,               /* dest  */
            vec->data + index,                   /* src   */
            (vec->length - index) * sizeof(int)  /* bytes */
        );
    }

    vec->data[index] = value;
    vec->length++;

    return 0;
}

// remove value at index
// Chat GPT
// Die Version von rxi ist ein remove Value, not at index....
int int_vec_remove(int_vec* vec, size_t index)
{
    if (vec == NULL) {
        return -1;
    }

    if (index >= vec->length) {
        return -1;
    }

    size_t tail = vec->length - index - 1;

    memmove(
        vec->data + index,       /* dest  */
        vec->data + index + 1,   /* src   */
        tail * sizeof(int)       /* bytes */
    );

    vec->length--;

    //optional: clear the freed slot for debuggability / security
    vec->data[vec->length] = 0;

    return 0;
}

// remove all elements
void int_vec_clear(int_vec * vec)
{
    if (vec == NULL) {
        return;
    }

    // preferring no old values
    if (vec->data != NULL && vec->length != 0) {
        memset(vec->data, 0, vec->length * sizeof(int));
    }

    vec->length = 0;
}

// safe element access
const int* int_vec_get(const int_vec* vec, size_t index)
{
    if (vec == NULL) {
        return NULL;
    }

    if (index >= vec->length) {
        return NULL;
    }

    return &vec->data[index];
}

// safe element assignment
int int_vec_set(int_vec* vec, size_t index, int value)
{
    if (vec == NULL) {
        return -1;
    }

    if (index >= vec->length) {
        return -1;
    }

    vec->data[index] = value;

    return 0;
}


// ----------------------------------------------------------------------

// iterator support
int* int_vec_next(const int_vec* vec, size_t* index_ptr)
{
    if (vec == NULL) {
        return NULL;
    }

    if (index_ptr == NULL) {
        return NULL;
    }

    if (*index_ptr >= vec->length) {
        return NULL;
    }

    int* value = &vec->data[*index_ptr];

    (*index_ptr)++;

    return value;
}

// ----------------------------------------------------------------------

void int_vec_truncate(int_vec* vec, size_t len)
{
    if (vec == NULL) {
        return;
    }

    vec->length = (len < vec->length) ? len : vec->length;
}

// first element
int int_vec_front(const int_vec* vec, int* out)
{
    if (vec == NULL) {
        return -1;
    }

    if (out == NULL) {
        return -1;
    }

    if (vec->length == 0) {
        return -1;
    }

    *out = vec->data[0];
    return 0;
}

// last element
int int_vec_back(const int_vec* vec, int* out)
{
    if (vec == NULL) {
        return -1;
    }

    if (out == NULL) {
        return -1;
    }

    if (vec->length == 0) {
        return -1;
    }

    *out = vec->data[vec->length - 1];
    return 0;
}

// ----------------------------------------------------------------------
// helper functions

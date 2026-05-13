#include "IntVec.h"

// ----------------------------------------------------------------------
// helper functions

int int_vec_expand_(char** data, int* length, int* capacity, int memsz) {
    if (*length + 1 > *capacity) {
        void* ptr;
        int n = (*capacity == 0) ? 1 : *capacity << 1;
        ptr = realloc(*data, n * memsz);
        if (ptr == NULL) return -1;
        *data = ptr;
        *capacity = n;
    }
    return 0;
}

// ----------------------------------------------------------------------
// int_vec API

void int_vec_init(int_vec* ptr)
{
    memset(ptr, 0, sizeof(*(ptr)));   // sizeof testen ...
}

void int_vec_deinit(int_vec* ptr)
{
    free(ptr->data);
    int_vec_init(ptr);
}

int int_vec_push(int_vec* ptr, int val)
{
    // int int_vec_expand_(char** data, int* length, int* capacity, int memsz) {


   // (int_vec_expand_(vec_unpack_(ptr)) ? -1 : ((ptr)->data[(ptr)->length++] = (val), 0), 0);

    return (int_vec_expand_((char**)&(ptr)->data, &ptr->length, &ptr->capacity, sizeof(*(ptr)->data)) ? -1 : ((ptr)->data[(ptr)->length++] = (val), 0), 0);
}

int vec_pop(int_vec* ptr)
{
    return ptr->data[--(ptr->length)];   // Operatoren vorang !!!
}

void vec_clear(int_vec* ptr)
{
    ptr->length = 0;  // hmm, und was ist mit free ???????????????
}

int first(int_vec* ptr)
{
    return ptr->data[0];
}

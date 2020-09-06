#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

NON_NULL void __vec_reserve(void *vec,
                            size_t n,
                            size_t elemsize,
                            size_t capacity)
{
    union {
        STRUCT_BODY(void);
        struct {
            size_t filler;
            char buf[];
        };
    } *v = vec;

    if (n > capacity) {
        if (v->on_heap) {
            v->ptr = realloc(v->ptr,
                             elemsize * (size_t) 1 << (v->capacity = ilog2(n)));
        } else {
            void *tmp =
                malloc(elemsize * (size_t) 1 << (v->capacity = ilog2(n)));
            memcpy(tmp, v->buf, elemsize * v->size);
            v->ptr = tmp;
            v->on_heap = 1;
        }
    }
}

NON_NULL void __vec_push_back(void *restrict vec,
                              void *restrict e,
                              size_t elemsize,
                              size_t capacity)
{
    union {
        STRUCT_BODY(char);
        struct {
            size_t filler;
            char buf[];
        };
    } *v = vec;

    if (v->on_heap) {
        if (v->size == capacity)
            v->ptr = realloc(v->ptr, elemsize * (size_t) 1 << ++v->capacity);
        memcpy(&v->ptr[v->size++ * elemsize], e, elemsize);
    } else {
        if (v->size == capacity) {
            void *tmp =
                malloc(elemsize * (size_t) 1 << (v->capacity = capacity + 1));
            memcpy(tmp, v->buf, elemsize * v->size);
            v->ptr = tmp;
            v->on_heap = 1;
            memcpy(&v->ptr[v->size++ * elemsize], e, elemsize);
        } else
            memcpy(&v->buf[v->size++ * elemsize], e, elemsize);
    }
}

NON_NULL void *__vec_pos(void *vec, size_t n, size_t type_size)
{
    union {
        STRUCT_BODY(char);
        struct {
            size_t filler;
            char buf[];
        };
    } *v = vec;

    assert(v->size > n);

    return (void *) ((v->on_heap ? v->ptr : v->buf) + n * type_size);
}

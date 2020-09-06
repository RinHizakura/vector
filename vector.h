#ifndef _VECTOR_H
#define _VECTOR_H

#include <assert.h>
#include <stdlib.h>

#define STRUCT_BODY(type)                                                  \
    struct {                                                               \
        size_t size : 54, on_heap : 1, capacity : 6, flag1 : 1, flag2 : 1, \
            flag3 : 1;                                                     \
        type *ptr;                                                         \
    }

#define NEXT_POWER_OF_2(s) \
    (s & (s - 1)) == 0     \
        ? s                \
        : (size_t) 1 << (64 - __builtin_clzl(s)) /* next power of 2 */

#define v(t, s, name, ...)                                              \
    _Static_assert(__builtin_strncmp(#t, "void", 4), "error type");     \
    union {                                                             \
        STRUCT_BODY(t);                                                 \
        struct {                                                        \
            size_t filler;                                              \
            t buf[NEXT_POWER_OF_2(s)];                                  \
        };                                                              \
    } name __attribute__((cleanup(vec_free))) = {.buf = {__VA_ARGS__}}; \
    name.size = sizeof((__typeof__(name.buf[0])[]){0, __VA_ARGS__}) /   \
                    sizeof(name.buf[0]) -                               \
                1;                                                      \
    name.capacity = sizeof(name.buf) / sizeof(name.buf[0])

#define vec_size(v) v.size
#define vec_capacity(v) \
    (v.on_heap ? (size_t) 1 << v.capacity : sizeof(v.buf) / sizeof(v.buf[0]))

#define vec_data(v) (v.on_heap ? v.ptr : v.buf) /* always contiguous buffer */

#define vec_elemsize(v) sizeof(v.buf[0])

#define vec_pos(v, n) \
    *(__typeof__(v.buf[0]) *) __vec_pos(&v, n, vec_elemsize(v))


#define vec_reserve(v, n) __vec_reserve(&v, n, vec_elemsize(v), vec_capacity(v))
#define vec_push_back(v, e)                                            \
    __vec_push_back(&v, &(__typeof__(v.buf[0])[]){e}, vec_elemsize(v), \
                    vec_capacity(v))

#define vec_pop_back(v) \
    assert(v.size > 0); \
    v.size -= 1

#define display(v)                               \
    do {                                         \
        for (size_t i = 0; i < vec_size(v); i++) \
            printf("%d ", vec_pos(v, i));        \
        puts(v.on_heap ? "heap" : "stack");      \
    } while (0)


#define vec_front(v) vec_pos(v, 0)
#define vec_back(v) vec_pos(v, v.size - 1)

#define vec_insert(v, p, e)                         \
    do {                                            \
        assert(p >= 0 && p < v.size);               \
        __typeof__(v.buf[0]) __tmp;                 \
        for (int i = 0; i < (signed) v.size; i++) { \
            if (i == p) {                           \
                __tmp = vec_pos(v, i);              \
                vec_pos(v, i) = e;                  \
            } else if (i > p) {                     \
                __typeof__(v.buf[0]) __t;           \
                __t = __tmp;                        \
                __tmp = vec_pos(v, i);              \
                vec_pos(v, i) = __t;                \
            }                                       \
        }                                           \
        vec_push_back(v, __tmp);                    \
    } while (0)

#define vec_insert_n(v, p, n, e)                                 \
    do {                                                         \
        assert(p >= 0 && p < v.size);                            \
        assert(n > 0);                                           \
        __typeof__(v.buf[0]) *__tmp =                            \
            malloc(sizeof(__typeof__(v.buf[0])) * (v.size - p)); \
        int cnt = 0;                                             \
        for (int i = 0; i < (signed) v.size; i++) {              \
            if (i >= p) {                                        \
                __tmp[cnt++] = vec_pos(v, i);                    \
            }                                                    \
        }                                                        \
        for (int i = 0; i < (signed) v.size - p; i++)            \
            vec_pop_back(v);                                     \
        vec_reserve(v, v.size + n + cnt);                        \
        for (int i = 0; i < n; i++)                              \
            vec_push_back(v, e);                                 \
        for (int i = 0; i < cnt; i++)                            \
            vec_push_back(v, __tmp[i]);                          \
        free(__tmp);                                             \
    } while (0)



#define NON_NULL __attribute__((nonnull))

static NON_NULL void __attribute__((unused)) vec_free(void *p)
{
    STRUCT_BODY(void) *s = p;
    if (s->on_heap)
        free(s->ptr);
}

static inline int ilog2(size_t n)
{
    return 64 - __builtin_clzl(n) - ((n & (n - 1)) == 0);
}

NON_NULL void __vec_reserve(void *vec,
                            size_t n,
                            size_t elemsize,
                            size_t capacity);

NON_NULL void __vec_push_back(void *restrict vec,
                              void *restrict e,
                              size_t elemsize,
                              size_t capacity);

NON_NULL void *__vec_pos(void *vec, size_t n, size_t elemsize);


#endif

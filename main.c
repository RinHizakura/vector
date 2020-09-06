#include <stdio.h>
#include "vector.h"

#define display(v)                               \
    do {                                         \
        for (size_t i = 0; i < vec_size(v); i++) \
            printf("%d ", vec_pos(v, i));        \
        puts(v.on_heap ? "heap" : "stack");      \
    } while (0)


int main()
{
    v(int, 2, vec, 13, 42);

    printf("pos(vec2,0)=%d, pos(vec2,1)=%d\n", vec_pos(vec, 0),
           vec_pos(vec, 3));

    vec_push_back(vec, 88);

    printf("capacity(vec)=%zu\n", vec_capacity(vec));

    display(vec);
    vec_pop_back(vec);
    display(vec);
    vec_pop_back(vec);
    display(vec);
    vec_pop_back(vec);
    display(vec);
    vec_pop_back(vec);
    display(vec);
    vec_pop_back(vec);

    return 0;
}

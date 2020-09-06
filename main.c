#include <stdio.h>
#include "vector.h"



int main()
{
    v(int, 0, vec1);

    vec_push_back(vec1, 78);
    vec_push_back(vec1, 16);

    display(vec1);
    vec_insert_n(vec1, 1, 3, 10);
    display(vec1);

    printf("capacity(vec)=%zu\n", vec_capacity(vec1));
    printf("front: %d back %d\n", vec_front(vec1), vec_back(vec1));

    display(vec1);
    vec_pop_back(vec1);
    display(vec1);
    vec_pop_back(vec1);

    return 0;
}

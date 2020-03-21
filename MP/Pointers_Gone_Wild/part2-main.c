#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    //1
    first_step(81);
    
    //2
    int value = 132;
    second_step(&value);
    
    //3
    value = 8942;
    int *a = &value;
    double_step(&a);
    
    //4
    char *c = malloc(5 + sizeof(int));
    *(int *)(c + 5) = 15;
    strange_step(c);
    free(c); c = NULL;
    
    //5
    char *d = malloc(4 * sizeof(char));
    d[3] = 0;
    empty_step(d);
    
    //6
    d[3] = 'u'; //reuse char pointer d
    two_step((void *)d, d);
    free(d); d = NULL;

    //7
    char *seven = malloc(5 * sizeof(char));
    char * first = seven, *second = first + 2, *third = second + 2;
    three_step(first, second, third);
    free(seven); seven = NULL;

    //8
    char *eight = malloc(4 * sizeof(char));
    eight[1] = 0; eight[2] = 8; eight[3] = 16;
    step_step_step(eight, eight, eight);
    free(eight); eight = NULL;

    //9
    value = 'a' - '\0';
    char ch = 'a';
    char *nine = &ch;
    it_may_be_odd(nine, value);

    //10
    char str[] = "UIUC,CS241";
    tok_step(str);

    //11
    int *p = malloc(sizeof(int));
    *p = 0x201;
    the_end((void *)p, (void *)p);
    free(p);

    return 0;
}

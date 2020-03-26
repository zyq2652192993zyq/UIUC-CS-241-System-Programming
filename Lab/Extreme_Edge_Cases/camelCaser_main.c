#include <stdio.h>
#include <stdlib.h>
#include "camelCaser.h"
#include "camelCaser_tests.h"

int main()
{
    if (camlCaserTest())
        printf("Success!\n");
    else
        printf("Failure!\n");

    return 0;
}

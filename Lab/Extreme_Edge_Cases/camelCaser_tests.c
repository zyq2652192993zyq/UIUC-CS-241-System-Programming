#include "camelCaser_tests.h"

bool camelCaserTest()
{
    char * inputs[] = {
        "Hello World.",
        "The Heisenbug is an incredible creature. Facenovel servers get their power from its indeterminism. Code smell can be ignored with INCREDIBLE use of air freshener. God objects are the new religion.",
        NULL
    };

    char ** input = inputs;
    while(*input){
        char **output = camel_caser(*input);
        bool flag = true;
        if (!check(output)) {
            flag = false;
            break;
        } 
        makeEmpty(output);
        input++;
    }

    return flag;
}

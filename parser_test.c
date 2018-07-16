#define __STDC_FORMAT_MACROS

#include "mydumper.h"

int main(void)
{
    parseSID("s999999");
    printf("ID is %" PRId64 "\n", id);

    parseSID("saaaaaa");
    printf("ID is %" PRId64 "\n", id);

    parseSID("saaaac1");
    printf("ID is %" PRId64 "\n", id);

    parseSID("saaaaab");
    printf("ID is %" PRId64 "\n", id);
    return 0;
}

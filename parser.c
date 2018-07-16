#define __STDC_FORMAT_MACROS

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <glib.h>

// #include <glib.h>

const int shortSidLen = 6;
const int64_t shortMaxSid = 2176782336; // 36^6
const char shortSidHash[] = "abcdefghijklmnopqrstuvwxyz0123456789";
const int shortSidHashLen = 36;

int64_t _index = 2176782336;
bool parseSID(const char sid[], GString *result)
{

    if (strlen(sid) != 7)
    {
        g_string_printf(result, "%" PRId64, _index);
        _index++;
        return false;
    }
    if (sid[0] != 's')
    {
        g_string_printf(result, "%" PRId64, _index);
        _index++;
        return false;
    }

    int64_t num = 0;
    int64_t base = 1;
    int64_t n;

    int inShortSidHash = 0;

    for (n = 5; n >= 0; n--)
    {
        int64_t i;
        for (i = 0; i < shortSidHashLen; i++)
        {

            if (shortSidHash[i] == sid[n + 1])
            {
                num = num + (i * base);
                base = base * (int64_t)36;
                inShortSidHash = 1;
            }
        }
        if (inShortSidHash == 0)
        {
            g_string_printf(result, "%" PRId64, _index);
            _index++;
            printf("SId %s failed to parse. \n", sid);
            return false;
        }
    };
    g_string_printf(result, "%" PRId64, num);
    return true;
}

#include "str_utils.h"

// #define read_line2(buf) read_str(buf, -1, UB2)
// #define read_str_2(buf, len) read_str(buf, -1, UB2)

int copy_str(char *dst, char *src, int start, char frontier)
{
    char *s = src + start;
    while (*s != 0 && *s++ != frontier)
        ;
    while (*s != frontier && *s != 0)
        *dst++ = *s++;
    *dst = 0;
    return s - src;
}

int find_ch(char *src, int start, char ch)
{
    char *s = src + start;
    while (*s != 0 && *s != ch)
    {
        s++;
    }

    return s - src;
}

int copy_strex(char *dst, char *src, int start, int length)
{
    char *s = src + start;
    while (length--)
    {
        *dst++ = *s++;
    }
    *dst = 0;
    return s - src;
}

int copy_str_until(char *dst, char *src, int start, char ch)
{
    char *s = src + start;
    while (*s != ch && *s != 0)
    {
        *dst++ = *s++;
    }
    *dst = 0;
    return s - src;
}
int copy_str_untilex(char *dst, char *src, int start, char ch, int length)
{
    char *s = src + start;
    while (length-- && *s != ch && *s != 0)
    {
        *dst++ = *s++;
    }
    *dst = 0;
    return s - src;
}
int strlen(char *c)
{
    int i = 0;
    while (*c++)
    {
        i++;
    }
    return i;
}
int strcmp(char *str1, char *str2)
{
    while (*str1 != 0 && *str1++ == *str2++)
        continue;
    return *str1 - *str2;
}
int copy_num(int *dst, char *src, int start)
{
    char *s = src + start;
    int sign = 1;
    while (*s != '-' && (*s < 48 || *s > 57))
    {
        if (*s == 0)
        {
            return -1;
        }
        s++;
    }
    if (*s == '-')
    {
        sign = -1;
        s++;
    }
    *dst = 0;
    while (*s >= 48 && *s <= 57)
    {
        *dst *= 10;
        *dst += *s++ - 48;
    }
    *dst *= sign;
    return s - src;
}

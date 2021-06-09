
#ifndef _STR_UTILS_INC
#define _STR_UTILS_INC
int copy_str(char *dst, char *src, int start, char frontier);
int find_ch(char *src, int start, char ch);
int copy_strex(char *dst, char *src, int start, int length);
int copy_str_until(char *dst, char *src, int start, char ch);
int copy_str_untilex(char *dst, char *src, int start, char ch, int length);
int copy_num(int *dst, char *src, int start);
#endif

#ifndef _INC_STRING
int strlen(char *c);
int strcmp(char *c, char *str);
#endif

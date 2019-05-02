#ifndef STRINGS_H
#define STRINGS_H
#include "types.h"

void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[],char n);
int strcmp(char s1[],char s2[]);
void* memset(void *dst,int c,uint n);
void* memmove(void*dst,const void* src,uint n);

#endif

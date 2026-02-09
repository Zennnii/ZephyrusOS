#ifndef STRING_H
#define STRING_H

#include "stdint.h"

int strcmp(const char *a, const char *b);
int strncmp(const char *s1, const char *s2, unsigned int n);
unsigned int strlen(const char *s);
char *strncpy(char *dest, const char *src, size_t n);

#endif
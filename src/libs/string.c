#include "string.h"

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char *)a - *(unsigned char *)b;
}

int strncmp(const char *s1, const char *s2, unsigned int n) {
    unsigned int i = 0;
    while (i < n && s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
        i++;
    }
    if (i == n) return 0; // matched n characters
    return (unsigned char)s1[i] - (unsigned char)s2[i];
}

unsigned int strlen(const char *s) {
    unsigned int len = 0;
    while (*s++) len++;
    return len;
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}
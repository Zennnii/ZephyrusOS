#ifndef STDINT_H
#define STDINT_H

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed long int32_t;
typedef unsigned long uint32_t;

typedef signed long long int64_t;
typedef unsigned long long uint64_t;

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

typedef uint32_t size_t;

typedef uint8_t bool;
#define true 1
#define false 0

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif
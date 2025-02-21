/******************************************************************************
 * Copyright (C) 2014-2020 Zhifeng Gong <gozfree@163.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#ifndef GEAR_MISC_H
#define GEAR_MISC_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * BASIC MACRO DEFINES
 ******************************************************************************/

#ifdef __GNUC__
#define LIKELY(x)           (__builtin_expect(!!(x), 1))
#define UNLIKELY(x)         (__builtin_expect(!!(x), 0))
#else
#define LIKELY(x)           (x)
#define UNLIKELY(x)         (x)
#endif


#define MAX_ERRNO           (4095)
#define IS_ERR_VALUE(x)     UNLIKELY((unsigned long)(intptr_t)(void *)(intptr_t)(x) >= (unsigned long)-MAX_ERRNO)

static inline void *ERR_PTR(long error)
{
    return (void *) (intptr_t)error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long) (intptr_t)ptr;
}

static inline bool IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)(intptr_t)ptr);
}

static inline bool IS_ERR_OR_NULL(const void *ptr)
{
    return UNLIKELY(!ptr) || IS_ERR_VALUE((unsigned long)(intptr_t)ptr);
}

/**
 * ERR_CAST - Explicitly cast an error-valued pointer to another pointer type
 * @ptr: The pointer to cast.
 *
 * Explicitly cast an error-valued pointer to another pointer type in such a
 * way as to make it clear that's what's going on.
 */
static inline void * ERR_CAST(const void *ptr)
{
    /* cast away the const */
    return (void *) ptr;
}

static inline int PTR_ERR_OR_ZERO(const void *ptr)
{
    if (IS_ERR(ptr))
        return PTR_ERR(ptr);
    else
        return 0;
}

/******************************************************************************
 * MACRO DEFINES ARE UPPERCASE
 ******************************************************************************/
typedef struct rational {
    int num;
    int den;
} rational_t;

/**
 * Variable-argument unused annotation
 */
#define UNUSED(e, ...)      (void) ((void) (e), ##__VA_ARGS__)

#define SWAP(a, b)          \
    do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)

#define MIN2(a, b)          ((a) > (b) ? (b) : (a))
#define MAX2(a, b)          ((a) > (b) ? (a) : (b))
#define ABS(x)              ((x) >= 0 ? (x) : -(x))

#define CALLOC(size, type)  (type *) calloc(size, sizeof(type))
#define ARRAY_SIZE(a)       (sizeof(a) / sizeof(a[0]))

#define VERBOSE()                                                   \
    do {                                                            \
        printf("%s:%s:%d xxxxxx\n", __FILE__, __func__, __LINE__);  \
    } while (0)

#define DUMP_BUFFER(buf, len)                                            \
    do {                                                                 \
        int _i, _j=0;                                                    \
        char _tmp[128] = {0};                                             \
        if (buf == NULL || len <= 0) {                                   \
            break;                                                       \
        }                                                                \
        for (_i = 0; _i < len; _i++) {                                   \
            if (!(_i%16)) {                                              \
                if (_i != 0) {                                           \
                    printf("%s", _tmp);                                  \
                }                                                        \
                memset(_tmp, 0, sizeof(_tmp));                           \
                _j = 0;                                                  \
                _j += snprintf(_tmp+_j, 64, "\n%p: ", buf+_i);           \
            }                                                            \
            _j += snprintf(_tmp+_j, 4, "%02hhx ", *((char *)buf + _i));  \
        }                                                                \
        printf("%s\n", _tmp);                                            \
    } while (0)

#define ALIGN2(x, a)    (((x) + (a) - 1) & ~((a) - 1))

#define is_alpha(c) (((c) >=  'a' && (c) <= 'z') || ((c) >=  'A' && (c) <= 'Z'))

/**
 * Compile-time strlen(3)
 * XXX: Should only used for `char[]'  NOT `char *'
 * Assume string ends with null byte('\0')
 */
#define STRLEN(s)          (sizeof(s) - 1)

/**
 * Compile-time assurance  see: linux/arch/x86/boot/boot.h
 * Will fail build if condition yield true
 */
#ifndef BUILD_BUG_ON
#if defined (OS_WINDOWS)
/*
 * MSVC compiler allows negative array size(treat as unsigned value)
 *  yet them don't allow zero-size array
 */
#define BUILD_BUG_ON(cond)      ((void) sizeof(char[!(cond)]))
#else
#define BUILD_BUG_ON(cond)      ((void) sizeof(char[1 - 2 * !!(cond)]))
#endif
#endif

GEAR_API void *memdup(const void *src, size_t len);
GEAR_API struct iovec *iovec_create(size_t len);
GEAR_API void iovec_destroy(struct iovec *);

GEAR_API bool is_little_endian(void);

GEAR_API int get_proc_name(char *name, size_t len);


#ifdef __cplusplus
}
#endif
#endif

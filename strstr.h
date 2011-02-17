#pragma once

/*
 * Str is a contiguous chunk of memory:
 * +-----+-----+-------------+
 * | CAP | LEN | STRING DATA |
 * +-----+-----+-------------+
 *
 * It can be used as a safe string implementation. The string itself is
 * null-terminated, so.. it's good for interop with standard C strings.
 *
 * I use 'unsigned int' as a type for length and capacity simply because 4
 * billion is enough for practically anything. And 'unsigned int' is 32 bits on
 * both x86 and x86_64, so I think it's a reasonable choice.
 *
 * The amount of memory required for a str with capacity == 5 is:
 *	(sizeof(struct str) + 5 + 1)
 *
 * Additional byte is used for zero termination and it's not a part of the
 * capacity.
 *
 * String data is always a correct C string.
 */

#include <stddef.h> /* for size_t */

/* TODO(nsf): Use this overridable allocator interface and add tests for
 * malloc/free correctness.
 *
 * A good idea: allocator interface should be stored in a thread-local
 * variable.
 */
struct str_alloc_interface {
	void *(*malloc)(size_t);
	void (*free)(void*);
};

/* should be > 0, and remember, that real memory size is +1 (trailing \0 byte) */
#ifndef STR_DEFAULT_CAPACITY
#define STR_DEFAULT_CAPACITY 7
#endif

struct str {
	unsigned int cap;
	unsigned int len;
	char data[];
};

/* different ways to create a str */
struct str *str_new(unsigned int cap);
struct str *str_from_cstr(const char *cstr);
struct str *str_from_cstr_len(const char *cstr, unsigned int len);
struct str *str_printf(const char *fmt, ...);
struct str *str_dup(const struct str *str);
struct str *str_from_file(const char *filename);

void str_free(struct str *str);
void str_clear(struct str *str);

/* make sure there is enough capacity for 'n' additional bytes */
void str_ensure_cap(struct str **str, unsigned int n);

/* appending to a str */
void str_add_str(struct str **str, const struct str *str2);
void str_add_cstr(struct str **str, const char *cstr);
//void str_add_cstr_len(struct str **str, const char *cstr, unsigned int len);
void str_add_printf(struct str **str, const char *fmt, ...);
void str_add_file(struct str **str, const char *filename);

/* trim, removes 'isspace' characters from sides: both, left, right */
void str_trim(struct str *str);
void str_ltrim(struct str *str);
void str_rtrim(struct str *str);

/* Splits 'path' immediately following the final path separator, separating it
 * into a directory and file name component. Returns a directory component if
 * any (if none, returns zero). If 'half2' isn't zero, writes file name
 * component to it (allocating a str, you're responsible to free it).
 */
struct str *str_path_split(const struct str *path, struct str **half2);

/*
 * FStr is a fixed string.
 * It doesn't manage its own memory, that's why it's called fixed.
 *
 * Length and capacity meanings are the same as in Str. Therefore usually the
 * capacity of the FStr is a buffer length minus one (zero termination).
 */

struct fstr {
	unsigned int cap;
	unsigned int len;
	char *data;
};

#define FSTR_INIT_FOR_BUF(fstr, buf)\
	fstr_init(fstr, buf, 0, sizeof(buf)/sizeof(buf[0])-1)
void fstr_init(struct fstr *fstr, char *data, unsigned int len, unsigned int cap);

/* appending to a fstr */
void fstr_add_str(struct fstr *fstr, const struct str *str);
void fstr_add_cstr(struct fstr *fstr, const char *cstr);
void fstr_add_printf(struct fstr *fstr, const char *fmt, ...);

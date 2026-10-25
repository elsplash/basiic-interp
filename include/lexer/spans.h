/* span.h
COPYRIGHT (C) 2026 ELSPLASH (electrsplash@gmail.com)
----------------------------------------------------

This header includes the line span, and string span.
Which also includes ways to manipulate and fetch them.

This file also provides a primitive string datatype,
which is supposed to abstract the dynamic string
operations, though I don't recommned using this.

Table Of Contents:

1. MACROS

1.1 SPANS_XX_LIB

These are all the macros similar to the SPANS_XX_LIB,
these macros are `glob_extern` by default. Though if
you want to replace it with some platform-specific
attribute, you may replace it via:

`#define SPANS_XX_LIB my_special_attr'

NOTE: Replace the XX part of the library with the
      sublibrary's name.

1.5 SPANS_XX_STATIC

Similar to 1.1..1, though it is mapped to `glob_static'.
It makes functions only this library needs, that
doesn't fit the purpose of this library.

Also similar to 1.1..1. You may replace it to your
platform specific attribute via:

`#define SPANS_XX_STATIC my_special_attr'

1.6 SPANS_ASSERT

This macro defines the assert function to use for
checking edge cases in your code. You may disable
it via:

`#define SPANS_ASSERT'

This is here to make sure you don't make mistakes.
If you wish to replace it instead it must
follow the parameters `(check, message)'. Either
the former, or you may modify the code as you
intend:

`#define SPANS_ASSERT(check, message) my_special_assert(check, message)'

1.7 SPANS_PANIC

This macro defines where the program should
end prematurely anywhere, with a verbose message.
This is for debugging of course, this has no
difference to `SPANS_ASSERT', except we just need
it to clear out some edge cases that a
`SPANS_ASSERT' just cannot clear on it's own.

You may replace it via:

`#define SPANS_PANIC(message) my_special_panic(message)'

I DO NOT RECOMMEND REMOVING IT ENTIRELY, as if
it continues as usual, will lead to undefined behavior,
since it doesn't recover after that.

2. STRUCTS

2.1.. spans_str_t

2.1..1 GENERAL

This struct is the main focus around these
other spans structs. It is simply a dynamically heap
allocated string, with functions.

2.2..2 CREATION

For creating strings without initializing them,
you may use `spans_str_creat()'. like this

spans_str_t string = spans_str_creat();

Though if you need it to be initialized beforehand,
you may use `spans_str_init(chr)' via:

cosnt char *primitive = "Hello World!";
spans_str_t string = spans_str_init(primitive);

2.2..3 MANIPULATION

For manipulation you may use the push, pop, and
insert functions. Those have their own variants,
so let's first cover the pop variants.

The pop variants are simply left and right. So
for the left one it moves the string to the left
so it removes the left part of the string. The
right however simply decreases the length of the
string. You may use the pop functions as so:

const char *primitive = "Hello world!";
spans_str_t string = spans_str_init(primitive);

spans_str_popl(&string, 6); // Removes "Hello "
spans_str_pop(&string, 1); // Removes "!"

For push, it adds yet another layer, the char
datatype, and the char pointer datatype, as so
you may use them like this:

// ... Continuing the above code
spans_str_pushl_chrp(&string, "What is the "); // Prepends "What is the "
spans_str_pushl_chr(&string, '?'); // Appends the '?' character.

It may also be done with other spans_str_t
datatypes.

// ... Again continuing
const spans_str_t temp = spans_str_init(" It is earth.");
spans_str_pushr_str(&string, &temp); // Appends " It is earth."

Inserting is the same as the push functions,
but it also accepts an index of where to push
the char, char pointer, and a string.

// ... You get the idea
const spans_str_t temp2 = spans_str_init(" A big beautiful sphere, As ");
const char *prefix = ", I'm really curious";

// Inserts prefix at idx 17.
spans_str_insert_chrp(&string, prefix, 17);

// Since it updated we need to insert at idx 39.
spans_str_insert_str(&string, &temp2, 39)

// Again updating, we need to insert at
spans_str_insert_chr(&string, '?', );

So now we can display our new string variable,
via the `spans_str_display()' function. Do note
that it is heap allocated so we must free it
after use.

// .

After all this, we may free the string as we
are not going to use it anymore, using the
`spans_str_free()' function. Also along with
all the other temporary string variables we
initialized, we can run:

// ...

2.2..4 READING

2.2..4 MISCELLANEOUS

-------------------------------------------------
Licensed under the GNU General Public License V3.
*/

#ifndef SPANS_DEF
#define SPANS_DEF

#include "../macros.h"
#include <stdlib.h>

#ifndef SPANS_STR_LIB
#define SPANS_STR_LIB glob_extern
#endif

#ifndef SPANS_LS_LIB
#define SPANS_LS_LIB glob_extern
#endif

#ifndef SPANS_LS_STATIC
#define SPANS_LS_STATIC glob_static
#endif

#ifndef SPANS_SS_LIB
#define SPANS_SS_LIB glob_extern
#endif

#ifndef SPANS_SS_STATIC
#define SPANS_SS_STATIC glob_static
#endif

#ifndef SPANS_ASSERT
#include <assert.h>
#define SPANS_ASSERT(check, message) assert(check && message)
#endif

#ifndef SPANS_PANIC
#include <stdio.h>
#define SPANS_PANIC(message) \
do {                         \
  puts("SPANS PANIC!");      \
  puts(message);             \
  exit(1);                   \
} while (0)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
A primitive string datatype, that's supposed to
abstract the dynamic allocation of char* pointers.
*/
typedef struct
{
  char          * __s;
  size_t          len;
  size_t          cap;
}
spans_str_t;

/*
This struct will be used for extracting the lines from
a file, and be used for lexing, which is what the
strspan_t is for.
*/
typedef struct
{
  spans_str_t   str;
  const char  * filename;
  size_t        num;
}
spans_linespan_t;

/*
This struct is used for extracting line spans from a file.
Which will be used later in lexing.
*/
typedef struct
{
  spans_linespan_t * ptr;
  size_t             len;
  size_t             cap;
}
spans_ls_table_t;

/*
This struct will be used for viewing a part of the
line span. Which is used for the lexer.
*/
typedef struct
{
  /* NOTE
  The str element will not be exactly the one in the
  linespan, since string formatting in the blexer
  is existent, we need this.
  */
  spans_str_t   str;

  /*
  Pretty much needed since the str.len is not a
  reliable source of the original length.
   */
  size_t        len;
  size_t        clmn;

  /* NOTE
  This line_idx indicates the index of the line it's
  viewing. Which the linespan_t table is created by
  a function `ls_fetch_file(file)`.
  */
  size_t line_idx;
}
spans_strspan_t;

/*
This struct is from strspan_t, which strips the string
from the strspan_t struct.

Though this still has the line_idx of course, with the
same purpose.
*/
typedef struct
{
  size_t
    len,
    clmn,
    l_idx,
    line_num;
}
spans_span_t;

/* Returns a default str_t */
SPANS_STR_LIB spans_str_t spans_str_creat(void);

/* This initializes the string with s */
SPANS_STR_LIB spans_str_t spans_str_init(const char *restr s);

/*
This is highly similar to strcmp(), though it actually
returns the way a comparison operator returns it.

This returns error codes that are aways falsey. These
error codes are:
-1 - s is NULL
-2 - c is NULL
*/
SPANS_STR_LIB int spans_str_equal_chrp(const spans_str_t *restr s, const char *restr c);

/*
Similar to spans_str_equal_chr(), but accepts a spans_str_t,
instead of a char pointer. Also similar to spans_str_equal_chr(),
is the error codes:
-1 - a is NULL
-2 - b is NULL
*/
SPANS_STR_LIB int spans_str_equal_str(const spans_str_t *restr a, const spans_str_t *restr b);

/*
This checks if the string has enough capacity.

If it has enough capacity, it will return 0.
If it expanded the string, it will return 1.
If s is NULL, or realloc fails it will return -1.
*/
SPANS_STR_LIB int spans_str_reserve(spans_str_t *restr s, const size_t sz);

/*
This will just decrease the .len element of the string.

Though it will still check if the string is, or under quarter
of the string capacity, then it will try to realloc to half
of it's original, ensuring that there is enough space for
more, after deflation.
*/
SPANS_STR_LIB int spans_str_popr(spans_str_t *restr s, const size_t sz);

/*
Similar to spans_str_popr, but it pops the left side.
This still tries to deflate the string, much like the
right version.
*/
SPANS_STR_LIB int spans_str_popl(spans_str_t *restr s, size_t sz);

/* This function removes the character at the index given. */
SPANS_STR_LIB int spans_str_pop_at(spans_str_t *restr s,
                                   const size_t       idx,
                                   const size_t       sz);

/*
This function pushes a character to the right, while
also doing some bounds checking and expanding if
necessary.
*/
SPANS_STR_LIB int spans_str_pushr_chr(spans_str_t *restr s, const char c);

/*
This function pushes a character to the left, much like
the right version
*/
SPANS_STR_LIB int spans_str_pushl_chr(spans_str_t *restr s, const char c);

/*
This pushes a char pointer from the right of the string,
and does bounds checking to do so.
*/
SPANS_STR_LIB int spans_str_pushr_chrp(spans_str_t *restr s, const char *restr p);

/*
This pushes a char pointer from the left side, similar to
the right version of this function.
*/
SPANS_STR_LIB int spans_str_pushl_chrp(spans_str_t *restr s, const char *restr p);

/* This pushes a string from the left side. */
SPANS_STR_LIB int spans_str_pushr_str(spans_str_t *restr a, const spans_str_t *restr b);

/* The right version of spans_str_pushr_str(). */
SPANS_STR_LIB int spans_str_pushl_str(spans_str_t *restr a, const spans_str_t *restr b);

/*
This inserts a character in an index to a string. This
function also does bounds checks to allocate
memory if needed.
*/
SPANS_STR_LIB int spans_str_insert_chr(spans_str_t *restr s,
                                       const char         c,
                                       const size_t       idx);

/*
This inserts a char pointer data into a string, similar
to spans_str_insert_chr.
*/
SPANS_STR_LIB int spans_str_insert_chrp(spans_str_t *restr s,
                                        const char  *restr p,
                                        const size_t       idx);

/*
This returns a heap allocated char pointer, so remember to
free it.
*/
SPANS_STR_LIB char* spans_str_display(const spans_str_t *restr s);

/* Frees the haep allocated string within s */
SPANS_STR_LIB void spans_str_free(spans_str_t *restr s);

/*
Returns a default linespan_t.

The defaults are:
str = spans_str_creat(),
num = 1,
*/
SPANS_LS_LIB spans_linespan_t spans_ls_creat(const char *restr file_path);

/*
Checks if both spans_linespan_t are the same. This also
returns which is not similar, from top to bottom are
checked, so if the top is true, and the bottom is also
true, the top will take precedence.

The error codes are:
-1 - a is NULL
-2 - b is NULL
-3 - Line numbers don't match
-4 - a->str is NULL
-5 - b->str is NULL
*/
SPANS_LS_LIB int spans_ls_equal(const spans_linespan_t *restr a, const spans_linespan_t *restr b);

/*
This function returns a table of linespan_t, for later
lexing. Which is supposed to be indexed from `strspan_t`s

The returned value is Nullable, which indicates an error.
*/
SPANS_LS_LIB spans_ls_table_t spans_ls_fetch_file(const char *restr filepath);

/*
This function frees the `spans_ls_table_t` datatype, if the
ltv is heap allocated you may free it yourself.
*/
SPANS_LS_LIB void spans_ls_table_free(spans_ls_table_t *restr ltv);

/*
This fetches the string in l, from start, and with length
len.

This is used to make `strspan_t`s.
*/
SPANS_LS_LIB char* spans_ls_fetch_self(const spans_linespan_t    *restr l,
                                      const size_t                      start_clmn,
                                      const size_t                      len);

/*
This is my version of power, so I don't have to take from
the math.h library. This will only accept `size_t`s.
*/
SPANS_LS_STATIC size_t spans_power(const size_t a, const size_t b);

/*
This returns the string length of a number if it were to
become a number.
*/
SPANS_LS_STATIC size_t spans_numlen(const size_t i);

/* This returns a heap allocated string, so remember to free it! */
SPANS_LS_LIB char* spans_ls_display(const spans_linespan_t *restr l);

/* Frees the string inside the linespan */
SPANS_LS_LIB void spans_ls_free(spans_linespan_t *restr l);

/* Fetches the line span from the table. */
SPANS_LS_LIB spans_linespan_t* spans_ls_table_fetch(const spans_ls_table_t *restr lt,
                                                    const size_t                  l_idx);

/*
Returns a strspan_t.

This fetches from the line span table, and fetches the
string from `ls_fetch_self()`.
*/
SPANS_SS_LIB spans_strspan_t spans_ss_init(const spans_linespan_t *restr l,
                                           const size_t                  l_idx,
                                           const size_t                  start_clmn,
                                           const size_t                  len);

/*
This checks if strspan is equal to a char pointer. Of course this
returns similar error codes to `spans_ls_equal()`.

It's error codes are:
-
*/
SPANS_SS_LIB int spans_ss_equal_chrp(const spans_strspan_t *restr ss, const char *restr c);

SPANS_SS_LIB int spans_ss_equal_ss(const spans_strspan_t *restr a , const spans_strspan_t *restr b);

/* This function concatenates s1's string, with s2's string. */
SPANS_SS_LIB int spans_ss_concat(      spans_strspan_t *restr s1,
                                 const spans_strspan_t *restr s2);

/* Repeats a character n times, for you lazy people. The pointer returned is heap allocated.*/
SPANS_SS_STATIC char* spans_chr_repeat(const char c, const size_t n);

/* This returns a heap allocated string, so remember to free it! */
SPANS_SS_LIB char* spans_ss_display(const spans_ls_table_t *restr lt,
                                    const spans_strspan_t  *restr s,
                                    const char                    swiggly_chr);

/* This converts the strspan_t to a span_t. */
SPANS_SS_LIB spans_span_t spans_ss_to(const spans_ls_table_t *restr lt,
                                      const spans_strspan_t *restr s);

/* Frees the string strspan_t */
SPANS_SS_LIB void spans_ss_free(spans_strspan_t *restr s);

#define str_t      spans_str_t
#define linespan_t spans_linespan_t
#define ls_table_t spans_ls_table_t
#define strspan_t  spans_strspan_t
#define span_t     spans_span_t

#define str_creat()          spans_str_creat()
#define str_init(s)          spans_str_init(s)
#define str_equal_chrp(s, c)  spans_str_equal_chrp(s, c)
#define str_equal_str(a, b)  spans_str_equal_str(a, b)
#define str_reserve(s, sz)   spans_str_reserve(s, sz)
#define str_popr(s, sz)      spans_str_popr(s, sz)
#define str_popl(s, sz)      spans_str_popl(s, sz)
#define str_pushr_chr(s, c)  spans_str_pushr_chr(s, c)
#define str_pushl_chr(s, c)  spans_str_pushl_chr(s, c)
#define str_pushr_chrp(s, p) spans_str_pushr_chrp(s, p)
#define str_pushl_chrp(s, p) spans_str_pushl_chrp(s, p)
#define str_pushr_str(a, b)  spans_str_pushr_str(a, b)
#define str_pushl_str(a, b)  spans_str_pushl_str(a. b)
#define str_display(s)       spans_str_display(s)
#define str_free(s)          spans_str_free(s)

#define ls_creat(file_path)               spans_ls_creat(file_path)
#define ls_equal(a, b)                    spans_ls_equal(a, b)
#define ls_fetch_file(file_path)          spans_ls_fetch_file(file_path)
#define ls_table_free(ltv)                spans_ls_table_free(ltv)
#define ls_fetch_self(l, start_clmn, len) spans_ls_fetch_self(l, start_clmn, len)
#define power(a, b)                       spans_power(a, b)
#define numlen(n)                         spans_numlen(n)
#define ls_display(l)                     spans_ls_display(l)
#define ls_free(l)                        spans_ls_free(l)
#define ls_table_fetch(lt, l_idx)         spans_ls_table_fetch(lt, l_idx)

#define ss_init(lt, l_idx, start_clmn, len) spans_ss_init(lt, l_idx, start_clmn, len)
#define ss_equal_chrp(s, c)                 spans_ss_equal_chrp(s, c)
#define ss_equal_ss(a, b)                   spans_ss_equal_ss(a, b)
#define ss_concat(s1, s2)                   spans_ss_concat(s1, s2)
#define ss_to(s)                            spans_ss_to(s)
#define chr_repeat(c, n)                    spans_chr_repeat(c, n)
#define ss_display(lt, s, swiggle_chr)      spans_ss_display(lt, s, swiggle_chr)
#define ss_free(s)                          spans_ss_free(s)

/* --- IMPLEMENTATION --- */

#define SPANS_IMPL

#ifdef SPANS_IMPL

#include <stdio.h>
#include <string.h>

#ifndef STR_T_INIT_SZ
#define STR_T_INIT_SZ 64
#endif

SPANS_STR_LIB spans_str_t spans_str_creat(void)
{
  char *p = calloc(1, STR_T_INIT_SZ);
  SPANS_ASSERT(p, "Failed to allocate p in str_creat()");

  return (spans_str_t) {
    .__s = p,
    .len = 0,
    .cap = STR_T_INIT_SZ,
  };
}

SPANS_STR_LIB spans_str_t spans_str_init(const char *restr s)
{
  if (!s)
    return spans_str_creat();

  const int slen  = strlen(s);
  size_t i = STR_T_INIT_SZ;

  while (i < slen) i *= 2;

  char * str = calloc(1, i);

  SPANS_ASSERT(str, "Failed to allocate str in str_creat()");

  strcpy(str, s);

  return (spans_str_t) {
    .__s = str,
    .len = slen,
    .cap = i,
  };
}

SPANS_STR_LIB int spans_str_equal_chrp(const spans_str_t *restr s, const char *restr c)
{
  if (!s) return -1;
  if (!c) return -2;
  if (strncmp(s->__s, c, s->len) == 0) return 1;
  return 0;
}

SPANS_STR_LIB int spans_str_equal_str(const spans_str_t *restr a, const spans_str_t *restr b)
{
  if (!a) return -1;
  if (!b) return -2;
  if (strncmp(a->__s, b->__s, a->len) == 0) return 1;
  return 0;
}

SPANS_STR_LIB int spans_str_reserve(spans_str_t *restr s, const size_t sz)
{
  if (!s) return -1;
  if (s->cap - s->len < sz)
  {
    while (s->cap - s->len < sz) s->cap *= 2;

    s->__s = realloc(s->__s, s->cap);

    if (!s->__s) return -2;
    return 1;
  }
  return 0;
}

SPANS_STR_LIB int spans_str_popr(spans_str_t *restr s, const size_t sz)
{
  if (!s) return -1;

  s->len -= sz;

  if (s->len < s->cap / 4)
  {
    s->cap /= 2;
    s->__s  = realloc(s->__s, s->cap);

    if (!s->__s) return -2;
    return 1;
  }

  return 0;
}

SPANS_STR_LIB int spans_str_popl(spans_str_t *restr s, size_t sz)
{
  if (!s) return -1;
  if (s->len < sz) sz = s->len;

  s->len -= sz;

  for (size_t i = 0; i < s->len; i++)
    s->__s[i] = s->__s[i + sz];

  if (s->len < s->cap / 4)
  {
    s->cap /= 2;
    s->__s  = realloc(s->__s, s->cap);

    if (!s->__s) return -2;
    return 1;
  }

  return 0;
}

SPANS_STR_LIB int spans_str_pop_at(spans_str_t *restr s,
                                   const size_t       idx,
                                   const size_t       sz)
{
  if (!s) return -1;
  if (idx + sz >= s->len) return -2;

  for (size_t i = idx; i < s->len - 1; i++)
    s->__s[i] = s->__s[i + sz];

  s->len -= sz;

  if (s->len < s->cap / 4)
  {
    s->cap /= 2;
    s->__s  = realloc(s->__s, s->cap);

    if (!s->__s) return -2;
    return 1;
  }

  return 0;
}

SPANS_STR_LIB int spans_str_pushr_chr(spans_str_t *restr s, const char c)
{
  if (!s) return -1;

  switch (spans_str_reserve(s, 1))
  {
  case -2:
    return -2;
  case -1:
    return -1;
  case 1:
    /* Fallthrough */
  case 0:
    break;
  }

  s->__s[s->len++] = c;

  return 0;
}

SPANS_STR_LIB int spans_str_pushl_chr(spans_str_t *restr s, const char c)
{
  if (!s) return -1;

  switch (spans_str_reserve(s, 1))
  {
  case -2:
    return -2;
  case -1:
    return -1;
  case 1:
    /* Fallthrough */
  case 0:
    break;
  }

  for (size_t i = s->len; i-- > 0;)
    s->__s[i + 1] = s->__s[i];

  s->__s[0] = c;

  return 0;
}

SPANS_STR_LIB int spans_str_pushr_chrp(spans_str_t *restr s, const char *restr p)
{
  if (!s || !p) return -1;
  const int plen = strlen(p);

  switch (spans_str_reserve(s, plen))
  {
  case -2:
    return -2;
  case -1:
    return -1;
  case 1:
    /* Fallthrough */
  case 0:
    break;
  }

  memcpy(s->__s + s->len, p, plen);
  s->len += plen;

  return 0;
}

SPANS_STR_LIB int spans_str_pushl_chrp(spans_str_t *restr s, const char *restr p)
{
  if (!s || !p) return -1;
  const int plen = strlen(p);

  switch (spans_str_reserve(s, plen))
  {
  case -2:
    return -2;
  case -1:
    return -1;
  case 1:
    /* Fallthrough */
  case 0:
    break;
  }

  for (size_t i = plen; i-- > 0;)
    s->__s[i + plen] = s->__s[i];

  memcpy(s->__s, p, plen);

  s->len += plen;

  return 0;
}

SPANS_STR_LIB int spans_str_pushr_str(      spans_str_t *restr a,
                                      const spans_str_t *restr b)
{
  if (!a) return -1;
  if (!b) return -2;

  int rval = spans_str_pushr_chrp(a, b->__s);
  if (rval != 0) return rval - 2;

  return 0;
}

SPANS_STR_LIB int spans_str_pushl_str(      spans_str_t *restr a,
                                      const spans_str_t *restr b)
{
  if (!a) return -1;
  if (!b) return -2;

  int rval = spans_str_pushl_chrp(a, b->__s);
  if (rval != 0) return rval - 2;

  return 0;
}

SPANS_STR_LIB int spans_str_insert_chr(spans_str_t *restr s,
                                       const char         c,
                                       const size_t       idx)
{
  if (!s)                return -1;
  if (idx >= s->len)     return -2;
  if (idx == 0)          return spans_str_pushl_chr(s, c);
  if (idx == s->len - 1) return spans_str_pushr_chr(s, c);

  switch (spans_str_reserve(s, 1))
  {
  case -2:
    return -2;
  case -1:
    return -1;
  case 1:
    /* Fallthrough */
  case 0:
    break;
  }

  for (size_t i = s->len; i-- > idx;)
    s->__s[i + 1] =  s->__s[i];
  s->__s[idx] = c;

  return 0;
}

SPANS_STR_LIB int spans_str_insert_chrp(spans_str_t  *restr s,
                                        const char   *restr p,
                                        const size_t        idx)
{
  if (!s)                return -1;
  if (idx >= s->len)     return -2;
  if (idx == 0)          return spans_str_pushl_chrp(s, p);
  if (idx == s->len - 1) return spans_str_pushr_chrp(s, p);

  const int plen = strlen(p);

  switch (spans_str_reserve(s, plen))
  {
  case -2:
    return -2;
  case -1:
    return -1;
  case 1:
    /* Fallthrough */
  case 0:
    break;
  }

  for (size_t i = s->len; i-- > idx;)
    s->__s[i + plen] =  s->__s[i];
  memcpy(s->__s + idx, p, plen);

  return 0;
}

SPANS_STR_LIB int spans_str_insert_str(      spans_str_t *restr a,
                                       const spans_str_t *restr b,
                                             size_t             idx)
{
  if (!a) return -1;
  if (!b) return -2;

  int rval = spans_str_insert_chrp(a, b->__s, idx);
  if (rval != 0) return rval - 2;

  return 0;
}

SPANS_STR_LIB char* spans_str_display(const spans_str_t *restr s)
{
  char *buf = calloc(1, s->len + 1);
  snprintf(buf, s->len + 1, "%.*s", (int) s->len, s->__s);
  return buf;
}

SPANS_STR_LIB void spans_str_free(spans_str_t *restrict s)
{
  free(s->__s);
  s->__s = NULL;
}

SPANS_LS_LIB spans_linespan_t spans_ls_creat(const char *restr filepath)
{
  return (spans_linespan_t) {
    .str      = spans_str_creat(),
    .filename = filepath,
    .num      = 1,
  };
}

SPANS_LS_LIB spans_linespan_t spans_ls_init(const char *restr span,
                                            const char *restr filepath,
                                            const size_t      line_num)
{
  return (spans_linespan_t) {
    .str      = spans_str_init(span),
    .filename = filepath,
    .num      = line_num,
  };
}

SPANS_LS_LIB int spans_ls_equal(const linespan_t *restr a,
                                const linespan_t *restr b)
{
  if (!a) return -1;
  if (!b) return -2;
  if (a->num != b->num)
    return -3;
  const int rval = spans_str_equal_str(&a->str, &b->str);
  if (rval < 0)
    return rval - 3;
  return rval;
}

#ifndef SPANS_LS_TABLE_INITSZ
#define SPANS_LS_TABLE_INITSZ 4
#endif

SPANS_LS_LIB spans_ls_table_t spans_ls_table_creat(void)
{
  spans_linespan_t *ptr = malloc(sizeof(*ptr) * SPANS_LS_TABLE_INITSZ);
  SPANS_ASSERT(ptr, "Failed to allocate in spans_ls_table_creat().");
  return (spans_ls_table_t) {
    .ptr = ptr, .len = 0,
    .cap = sizeof(*ptr) * SPANS_LS_TABLE_INITSZ,
  };
}

SPANS_LS_LIB int spans_ls_table_push(spans_ls_table_t *restr lt,
                                     const spans_linespan_t  l)
{
  if (!lt) return -1;

  if ((lt->len + 1) * sizeof(*lt->ptr) > lt->cap)
  {
    lt->cap *= 2;
    lt->ptr = realloc(lt->ptr, lt->cap);
    if (!lt->ptr) return -2;
  }

  lt->ptr[lt->len] = l;
  lt->len++;
  return 0;
}

SPANS_LS_LIB spans_ls_table_t spans_ls_fetch_file(const char *restr filepath)
{
  spans_ls_table_t lt = spans_ls_table_creat();
  
  FILE* f = fopen(filepath, "rb");
  if (!f)
  {
    spans_ls_table_free(&lt);
    SPANS_PANIC("Failed to open file.");
  }

  size_t            lnum    = 1;
  spans_linespan_t  ls_push = spans_ls_creat(filepath);

  int cur = 0;

  while (cur != EOF)
  {
    ls_push.str = spans_str_creat();
    while ((cur - fgetc(f)) != EOF || cur != '\n')
    {
      if (spans_str_pushr_chr(&ls_push.str, (char) cur) < 0)
        SPANS_PANIC("Failed to push character in spans_ls_fetch_file().");
    }
    ls_push.num = lnum;
    lnum++;

    spans_ls_table_push(&lt, ls_push);
  }

  return lt;
}

SPANS_LS_LIB char* spans_ls_fetch_self(const spans_linespan_t *restr l,
                                       const size_t                  start,
                                       const size_t                  len)
{
  if (l->str.len < start || l->str.len < start + len) return NULL;

  char *o = calloc(1, len + 1);

  if (!o) return NULL;

  memcpy(o, l->str.__s + start, len);

  return o;
}

SPANS_LS_LIB spans_linespan_t* spans_ls_table_fetch(const spans_ls_table_t    *restr lt,
                                                          const size_t               l_idx)
{
  if (lt->len <= l_idx) return NULL;

  return lt->ptr + l_idx;
}

SPANS_LS_STATIC size_t spans_power(const size_t a, const size_t b)
{
  if (b == 0) return 1L;
  size_t o = a;
  for (size_t i = 0; i < b; i++)
    o *= a;
  return o;
}

SPANS_LS_STATIC size_t spans_numlen(const size_t i)
{
  size_t pow = 10;
  size_t j   = 0;

  while (i / pow != 0)
    pow = spans_power(pow, ++j);

  if (j == 0) return 1;
  return j;
}

SPANS_LS_LIB char* spans_ls_display(const spans_linespan_t *restr l)
{
  const unsigned int buff_sz = spans_numlen(l->num) + l->str.len + 5;
  char *buff                 = calloc(1, buff_sz);
  if (!buff) return NULL;

  char *str_display = spans_str_display(&l->str);
  snprintf(buff, buff_sz,
           "%lu | %s",
           l->num, str_display);

  free(str_display);
  return buff;
}

SPANS_LS_LIB void spans_ls_free(spans_linespan_t *restr l)
{
  spans_str_free(&l->str);
}

SPANS_LS_LIB void spans_ls_table_free(spans_ls_table_t *restr ltv)
{
  for (size_t i = 0; i < ltv->len; i++)
  {
    linespan_t *l = &ltv->ptr[i];
    ls_free(l);
  }
  free(ltv->ptr);
}

SPANS_SS_LIB spans_strspan_t spans_ss_init(const spans_linespan_t    *restr l,
                                           const size_t                     l_idx,
                                           const size_t                     start,
                                           const size_t                     len)
{
  SPANS_ASSERT(l, "l is NULL in ss_init()");
  SPANS_ASSERT(l->str.len > start, "Starting index is out of l->str's bounds in ss_init()");
  SPANS_ASSERT(l->str.len >= start + len, "Tried fetching more than line contents in ss_init()");

  char *p = spans_ls_fetch_self(l, start, len);
  SPANS_ASSERT(p, "Failed to fetch string from linespan in ss_init()");

  spans_str_t s = str_init(p);
  free(p);

  return (spans_strspan_t) {
    .str      = s,
    .len      = s.len,
    .clmn     = start,
    .line_idx = l_idx,
  };
}

SPANS_SS_LIB int spans_ss_equal_chrp(const spans_strspan_t *restr s,
                                     const char            *restr c)
{
  if (!s) return -1;
  const int rval = spans_str_equal_chrp(&s->str, c);
  if (rval < 0)
    return rval - 1;
  return rval;
}

SPANS_SS_LIB int spans_ss_equal_ss(const spans_strspan_t *restr a,
                                   const spans_strspan_t *restr b)
{
  if (!a) return -1;
  if (!b) return -2;
  const int rval = spans_str_equal_str(&a->str, &b->str);
  if (rval < 0)
    return rval - 2;
  return rval;
}

SPANS_SS_LIB int spans_ss_concat(      spans_strspan_t *restr s1,
                                 const spans_strspan_t *restr s2)
{
  if (!s1 || !s2)                   return -1;
  if (s1->line_idx != s2->line_idx) return -2;
  if (s1->clmn > s2->clmn)          return -3;

  spans_str_pushr_chrp(&s1->str, s2->str.__s);

  return 0;
}

SPANS_SS_STATIC char* spans_chr_repeat(const char c, const size_t n)
{
  char *o = calloc(1, n + 1);
  if (!o) return NULL;
  memset(o, (int) c, n);
  return o;
}

SPANS_SS_LIB char* spans_ss_display(const spans_ls_table_t *restr lt,
                                    const spans_strspan_t  *restr s,
                                    const char                    swiggly_chr)
{
  const spans_linespan_t *l = spans_ls_table_fetch(lt, s->line_idx);
  if (!l) return NULL;

  const size_t buf_sz = ((spans_numlen(l->num) + 5) * 2) + l->str.len + s->clmn + s->len + 1;
  char * buf          = calloc(1, buf_sz);
  if (!buf) return NULL;

  char * spaces1  = spans_chr_repeat(' ', spans_numlen(l->num));
  char * spaces2  = spans_chr_repeat(' ', s->clmn);
  char * swiggly = spans_chr_repeat(swiggly_chr, s->len);
  if (!spaces1 &&  !spaces2 && !swiggly) return NULL;

  char *ls_display = spans_ls_display(l);
  snprintf(buf, buf_sz, "%s\n%s | %s%s",
           ls_display, spaces1, spaces2, swiggly);

  free(ls_display);
  free(swiggly);
  free(spaces1);
  free(spaces2);
  return buf;
}

SPANS_SS_LIB spans_span_t spans_ss_to(const spans_ls_table_t *restr lt,
                                      const spans_strspan_t  *restr s)
{
  const spans_linespan_t *l = spans_ls_table_fetch(lt, s->line_idx);
  SPANS_ASSERT(l, "l_idx of s is invalid.");

  return (spans_span_t) {
    .len      = s->len,
    .clmn     = s->clmn,
    .l_idx    = s->line_idx,
    .line_num = l->num,
  };
}

SPANS_SS_LIB void ss_free(spans_strspan_t *restr s)
{
  spans_str_free(&s->str);
}

/* TODO: Some spans_t library functions */

#endif /* SPANS_IMPL*/

#ifdef __cplusplus
}
#endif

#endif /* SPAN_DEF */

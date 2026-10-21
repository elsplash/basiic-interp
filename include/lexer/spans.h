/* span.h
COPYRIGHT (C) 2026 ELSPLASH (electrsplash@gmail.com)
----------------------------------------------------

This header includes the line span, and string span.
Which also includes ways to manipulate and fetch them.

This file also provides a primitive string datatype,
which is supposed to abstract the dynamic string
operations, though I don't recommned using this.

There are also some important macros in here aswell.

-------------------------------------------------
Licensed under the GNU General Public License V3.
*/

#ifndef SPANS_DEF
#define SPANS_DEF

#include "macros.h"
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
SPANS_STR_LIB int spans_str_equal_chr(const spans_str_t *restr s, const char *restr c);

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
SPANS_STR_LIB int spans_str_popl(spans_str_t *restr s, const size_t sz);

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
SPANS_LS_LIB spans_ls_table_t* spans_ls_fetch_file(const char *restr file_path);

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
SPANS_SS_LIB int spans_ss_equal_chr(const spans_strspan_t *restr ss, const char *restr c);

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
#define str_equal_chr(s, c)  spans_str_equal_chr(s, c)
#define str_equal_str(a, b)  spans_str_equal_str(a, b)
#define str_reserve(s, sz)   spans_str_reserve(s, sz)
#define str_popr(s, sz)      spans_str_popr(s, sz)
#define str_popl(s, sz)      spans_str_popl(s, sz)
#define str_pushr_chr(s, c)  spans_str_pushr_chr(s, c)
#define str_pushl_chr(s, c)  spans_str_pushl_chr(s, c)
#define str_pushr_chrp(s, p) spans_str_pushr_chrp(s, p)
#define str_pushl_chrp(s, p) spans_str_pushl_chrp(s, p)
#define str_display(s)       spans_str_display(s)
#define str_free(s)          spans_str_free(s)

#define ls_creat(file_path)               spans_ls_creat(file_path)
#define ls_equal(a, b)					  spans_ls_equal(a, b)
#define ls_fetch_file(file_path)          spans_ls_fetch_file(file_path)
#define ls_table_free(ltv)				  spans_ls_table_free(ltv)
#define ls_fetch_self(l, start_clmn, len) spans_ls_fetch_self(l, start_clmn, len)
#define power(a, b)                       spans_power(a, b)
#define numlen(n)                         spans_numlen(n)
#define ls_display(l)                     spans_ls_display(l)
#define ls_free(l)                        spans_ls_free(l)
#define ls_table_fetch(lt, l_idx)         spans_ls_table_fetch(lt, l_idx)

#define ss_init(lt, l_idx, start_clmn, len) spans_ss_init(lt, l_idx, start_clmn, len)
#define ss_equal_chr(s, c)                  spans_ss_equal_chr(s, c)
#define ss_equal_ss(a, b)                   spans_ss_equal_ss(a, b)
#define ss_concat(s1, s2)                   spans_ss_concat(s1, s2)
#define ss_to(s)                            spans_ss_to(s)
#define chr_repeat(c, n)                    spans_chr_repeat(c, n)
#define ss_display(lt, s, swiggle_chr)      spans_ss_display(lt, s, swiggle_chr)
#define ss_free(s)                          spans_ss_free(s)

/* --- IMPLEMENTATION --- */

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

SPANS_STR_LIB int spans_str_equal_chr(const spans_str_t *restr s, const char *restr c)
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

SPANS_STR_LIB int spans_str_popl(spans_str_t *restr s, const size_t sz)
{
  if (!s) return -1;

  if (s->len < sz) s->len = 0;
  else             s->len -= sz;
  for (size_t i = 0; i < s->len - sz; i++)
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

  for (size_t i = 0; i-- > 0;)
    s->__s[i + plen] = s->__s[i];

  memcpy(s->__s, p, plen);

  s->len += plen;

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

SPANS_LS_LIB spans_linespan_t spans_ls_creat(const char *restr file_path)
{
  return (spans_linespan_t) {
    .str      = spans_str_creat(),
    .filename = file_path,
    .num      = 1,
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

#ifndef LS_TABLE_INITSZ
#define LS_TABLE_INITSZ 4
#endif

SPANS_LS_LIB spans_ls_table_t* spans_ls_fetch_file(const char *restr file_path)
{
  spans_ls_table_t *lt = calloc(1, sizeof(*lt));

  lt->ptr = calloc(LS_TABLE_INITSZ, sizeof(spans_linespan_t));

  if (!lt->ptr) return NULL;

  lt->len = 0;
  lt->cap = sizeof(spans_linespan_t) * LS_TABLE_INITSZ;

  spans_linespan_t  ls_push = spans_ls_creat(file_path);
  size_t            lnum    = 1;
  spans_str_t       line;

  FILE* f = fopen(file_path, "rb");
  if (!f) return NULL;

  int cur = 0;

  while (cur != EOF)
  {
    ls_push.str = spans_str_creat();
    /*
    NOTE: You may fix this in any way you like, since this works,
          I will not touch it as long as my program runs fine.
          - ESplash
    */
    while (1)
    {
      cur = fgetc(f);
      if (cur == EOF || cur == '\n') break;
      if (spans_str_pushr_chr(&ls_push.str, (char) cur) < 0)
      {
        str_free(&line);
        return NULL;
      }
    }
    ls_push.num = lnum;
    lnum++;

    if ((lt->len + 1) * sizeof(*lt->ptr) >= lt->cap)
    {
      while ((lt->len + 1) * sizeof(*lt->ptr) >= lt->cap) lt->cap *= 2;
      lt->ptr  = realloc(lt->ptr, lt->cap);
      if (!lt->ptr)
      {
        str_free(&line);
        return NULL;
      }
    }

    memcpy(lt->ptr + lt->len, (void*) &ls_push, sizeof(ls_push));
    lt->len++;
  }

  return lt;
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

SPANS_LS_LIB spans_linespan_t* spans_ls_table_fetch(const spans_ls_table_t    *restr lt,
                                                          const size_t               l_idx)
{
  if (lt->len <= l_idx) return NULL;

  return lt->ptr + l_idx;
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

SPANS_SS_LIB int spans_ss_equal_chr(const spans_strspan_t *restr s,
                                    const char            *restr c)
{
  if (!s) return -1;
  const int rval = spans_str_equal_chr(&s->str, c);
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

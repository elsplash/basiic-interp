/* span.h
This header includes the line span, and string span.
Which also includes ways to manipulate and fetch them.

This file also provides a primitive string datatype,
which is supposed to abstract the dynamic string
operations, though I don't recommned using this.
*/

#ifndef SPANS_DEF
#define SPANS_DEF

#ifdef __cplusplus
extern "C" {
#endif

#ifndef restr

#if defined(__STDC_VERSION__ ) && __STDC_VERSION__ >= 199901L
	#define restr restrict
#elif defined(_MSC_VER) && _MSC_VER >= 1400
	#define restr __restrict
#elif defined(__GNUC__) | defined(__clang__)
	#define restr __restrict__
#else
	#define restr
#endif

#endif /* restr */

/* glob_extern
This macro expands to make it so it outputs the function correctly.
*/
#ifndef glob_extern

#ifdef _WIN32
#define glob_extern __declspec(dllexport)

#elif defined(__GNUC__) && __GNUC__ > 4
#define  glob_extern __attribute__((visibility("default")))
  
#else
#define glob_extern extern

#endif
#endif

/* glob_static
This macro expands so it makes static functions properly.
*/
#ifndef glob_static

#ifdef _WIN32
#define glob_static __decl_spec(dllimport)

#elif defined(__GNUC__) && __GNUC__ > 4
#define glob_static __attribute__((visibility("hidden")))

#else
#define glob_static static
  
#endif
#endif

#ifndef SPANS_STR_LIB
#define SPANS_STR_LIB glob_extern
#endif

#ifndef SPAS_LS_LIB
#define SPAS_LS_LIB glob_extern
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

/* str_t
A primitive string datatype, that's supposed to
abstract the dynamic allocation of char* pointers.
*/
typedef struct
{
  char          * __s;
  unsigned long   len;
  unsigned long   cap;
} str_t;

/* linespan_t
This struct will be used for extracting the lines from
a file, and be used for lexing, which is what the
strspan_t is for.
*/
typedef struct {
  str_t           str;
  const char    * filename;
  unsigned long   num;
} linespan_t;

/* ls_table_t
This struct is used for extracting line spans from a file.
Which will be used later in lexing.
*/
typedef struct
{
  linespan_t * ptr;
  unsigned long len;
  unsigned long cap;
} ls_table_t;

/* strspan_t
This struct will be used for viewing a part of the
line span. Which is used for the lexer.
*/
typedef struct
{
  str_t         str;
  unsigned long clmn;

  /* NOTE
  This line_idx indicates the index of the line it's
  viewing. Which the linespan_t table is created by
  a function `fetch_linespan_f(file)`.
  */
  unsigned long line_idx;
} strspan_t;

/* span_t
This struct is from strspan_t, which strips the string
from the strspan_t struct.

Though this still has the line_idx of course, with the
same purpose.
*/
typedef struct
{
  unsigned long
    len,
    clmn,
    l_idx,
    line_num;
} span_t;

/* Returns a default str_t */
SPANS_STR_LIB inline str_t spans_str_creat(void);

/* This initializes the string with s */
SPANS_STR_LIB str_t spans_str_init(const char *restr s);

/*
This checks if the string has enough capacity.

If it has enough capacity, it will return 0.
If it expanded the string, it will return 1.
If s is NULL, or realloc fails it will return -1.
*/
SPANS_STR_LIB int spans_str_reserve(str_t *restr s, const unsigned long sz);

/*
This will just decrease the .len element of the string.

Though it will still check if the string is, or under quarter
of the string capacity, then it will try to realloc to half
of it's original, ensuring that there is enough space for
more, after deflation.
*/
SPANS_STR_LIB int spans_str_pop(str_t *restr s, const unsigned long sz);

/*
This function pushes a character, while also doing some
bounds checking and expanding if necessary.
*/
SPANS_STR_LIB int spans_str_push_chr(str_t *restr s, const char c);

/*
This pushes a string to the string, and enlarges the string
capacity if needed.
*/
SPANS_STR_LIB int spans_str_push_str(str_t *restr s, const char *restr p);

/*
This returns a heap allocated char pointer, so remember to
free it.
*/
SPANS_STR_LIB char* spans_str_display(const str_t *restr s);

/* Frees the haep allocated string within s */
SPANS_STR_LIB inline void spans_str_free(str_t *restr s);

/*
Returns a default linespan_t.

The defaults are:
str = str_crea(),
num = 1,
*/
SPAS_LS_LIB inline linespan_t spans_ls_creat(const char *restr file_path);

/*
This function returns a table of linespan_t, for later
lexing. Which is supposed to be indexed from `strspan_t`s

The returned value is Nullable, which indicates an error.
*/
SPAS_LS_LIB ls_table_t* spans_ls_fetch_file(const char *restr file_path);

/*
This fetches the string in l, from start, and with length
len.

This is used to make `strspan_t`s.
*/
SPAS_LS_LIB char* spans_ls_fetch_self(const linespan_t    *restr l,
                                const unsigned long        start_clmn,
                                const unsigned long        len);

SPANS_LS_STATIC unsigned long spans_power(const unsigned long a, const unsigned long b);

SPANS_LS_STATIC unsigned long spans_numlen(const unsigned long i);
  
/* This returns a heap allocated string, so remember to free it! */
SPAS_LS_LIB char* spans_ls_display(const linespan_t *restr l);

/* Frees the string inside the linespan */
SPAS_LS_LIB inline void spans_ls_free(linespan_t *restr l);

/* Fetches the line span from the table. */
SPAS_LS_LIB inline linespan_t* spans_ls_table_fetch(const ls_table_t    *restr lt,
                                                    const unsigned long        l_idx);

/*
Returns a strspan_t.

This fetches from the line span table,a nd fetches the string from
`ls_fetch_self()`.
*/
SPANS_SS_LIB strspan_t spans_ss_init(const linespan_t    * l,
                                    const unsigned long   l_idx,
                                    const unsigned long   start_clmn,
                                    const unsigned long   len);

/* This function concatenates s1's string, with s2's string. */
SPANS_SS_LIB int spans_ss_concat(      strspan_t *restr s1,
                          const strspan_t *restr s2);

/* Repeats a character n times, for you lazy people. The pointer returned is heap allocated.*/
SPANS_SS_STATIC inline char* spans_chr_repeat(const char c, const unsigned long n);

/* This returns a heap allocated string, so remember to free it! */
SPANS_SS_LIB char* spans_ss_display(const ls_table_t *restr lt, const strspan_t *restr s, const char swiggly_chr);

/* This converts the strspan_t to a span_t. */
SPANS_SS_LIB span_t spans_ss_to(const ls_table_t *restr lt, const strspan_t *restr s);

/* Frees the string strspan_t */
SPANS_SS_LIB void spans_ss_free(strspan_t *restr s);

#define str_creat()        spans_str_creat()
#define str_init(s)        spans_str_init(s)
#define str_reserve(s, sz) spans_str_reserve(s, sz)
#define str_pop(s, sz)     spans_str_pop(s, sz)
#define str_push_chr(s, c) spans_str_push_chr(s, c)
#define str_push_str(s, p) spans_str_push_str(s, p)
#define str_display(s)     spans_str_display(s)
#define str_free(s)        spans_str_free(s)

#define ls_creat(file_path)               spans_ls_creat(file_path)
#define ls_fetch_file(file_path)          spans_ls_fetch_file(file_path)
#define ls_fetch_self(l, start_clmn, len) spans_ls_fetch_self(l, start_clmn, len)
#define power(a, b) 					  spans_power(a, b)
#define numlen(n)                         spans_numlen(n)
#define ls_display(l)                     spans_ls_display(l)
#define ls_free(l)                        spans_ls_free(l)
#define ls_table_fetch(lt, l_idx)         spans_ls_table_fetch(lt, l_idx)

#define ss_init(lt, l_idx, start_clmn, len) spans_ss_init(lt, l_idx, start_clmn, len)
#define ss_concat(s1, s2)                   spans_ss_concat(s1, s2)
#define ss_to(s)                            spans_ss_to(s)
#define chr_repeat(c, n)                    spans_chr_repeat(c, n)
#define ss_display(lt, s, swiggle_chr)      spans_ss_display(lt, s, swiggle_chr)
#define ss_free(s)                          spans_ss_free(s)

/*-- IMPLEMENTATION --*/

#ifdef SPAN_IMPL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef STR_T_INIT_SZ
#define STR_T_INIT_SZ 64
#endif

SPANS_STR_LIB inline str_t spans_str_creat(void) {
  char *p = calloc(1, STR_T_INIT_SZ);
  assert(p && "Failed to allocate p in str_creat()");
  return (str_t) {
    .__s = p,
    .len = 0,
    .cap = STR_T_INIT_SZ,
  };
}

SPANS_STR_LIB str_t spans_str_init(const char *restr s)
{
  if (!s)
    return str_creat();

  const int slen = strlen(s);
  unsigned long i = STR_T_INIT_SZ;

  while (i < slen)
    i *= 2;

  char * str = calloc(1, i);

  assert(str && "Failed to allocate str in str_creat()");

  strcpy(str, s);

  return (str_t) {
    .__s = str,
    .len = slen,
    .cap = i,
  };
}

SPANS_STR_LIB int spans_str_reserve(str_t *restr s, const unsigned long sz)
{
  if (!s) return -1;
  if (s->cap - s->len < sz)
  {
    while (s->cap - s->len < sz)
      s->cap *= 2;

    s->__s = realloc(s->__s, s->cap);

    if (!s->__s)
      return -2;
    return 1;
  }
  return 0;
}

SPANS_STR_LIB int str_pop(str_t *restr s, const unsigned long sz)
{
  if (!s)
    return -1;

  s->len -= sz;

  if (s->len < s->cap / 4) {
    s->cap /= 2;
    s->__s = realloc(s->__s, s->cap);

    if (!s->__s)
      return -2;
    return 1;
  }
  return 0;
}

SPANS_STR_LIB int spans_str_push_chr(str_t *restr s, const char c)
{
  if (!s) return -1;

  switch (str_reserve(s, 1))
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

SPANS_STR_LIB int spans_str_push_str(str_t *restr s, const char *restr p)
{
  if (!s || !p)
    return -1;
  const int plen = strlen(p);

  switch (str_reserve(s, plen))
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

  strcat(s->__s, p);
  s->len += plen;

  return 0;
}

SPANS_STR_LIB char* spans_str_display(const str_t *restr s)
{
  char *buf = malloc(s->len + 1);
  snprintf(buf, s->len + 1, "%.*s", s->len, s->__s);
  return buf;
}

SPANS_STR_LIB inline void spans_str_free(str_t *restrict s)
{
  free(s->__s);
  s->__s = NULL;
}

SPAS_LS_LIB inline linespan_t spans_ls_creat(const char *restr file_path)
{
  return (linespan_t) {
    .str      = str_creat(),
    .filename = file_path,
    .num      = 1,
  };
}

#define LS_TABLE_INITSZ 8

SPAS_LS_LIB ls_table_t* spans_ls_fetch_file(const char *restr file_path)
{
  ls_table_t *lt = calloc(1, sizeof(*lt));
  
  lt->ptr = calloc(LS_TABLE_INITSZ, sizeof(linespan_t));

  if (!lt->ptr) return NULL;

  lt->cap = sizeof(linespan_t) * LS_TABLE_INITSZ;

  linespan_t 	ls_push = ls_creat(file_path);
  str_t 		line = str_creat();
  unsigned long lnum = 1;

  FILE* f = fopen(file_path, "rb");

  if (!f) return NULL;

  int cur = 0;

  while (cur == EOF) {
  	while ((cur = fgetc(f)) == '\n' || cur == EOF)
  	{
  	  if (!str_push_chr(&line, cur)) return NULL;
  	}
  	ls_push.str = line;
  	ls_push.num = lnum++;

    if (lt->len == lt->cap)
    {
      lt->cap *= 2;
      lt->ptr = realloc(lt->ptr, lt->cap);
      if (!lt->ptr) return NULL;
    }
  	memcpy(lt->ptr + lt->len, (void*) &ls_push, sizeof(ls_push));
  	lt->len++;
  }

  return lt;
}

SPAS_LS_LIB char* spans_ls_fetch_self(const linespan_t    *restr l,
                                const unsigned long        start,
                   	            const unsigned long        len)
{
  if (l->str.len < start || l->str.len < start + len) return NULL;
  char *o = calloc(1, len + 1);
  if (!o) return NULL;

  memcpy(o, l->str.__s + start, len);

  return o;
}

SPANS_LS_STATIC unsigned long spans_power(const unsigned long a, const unsigned long b)
{
  unsigned long o = a;
  for (unsigned int i = 0; i < b; i++)
    o *= a;
  return o;
}

SPANS_LS_STATIC unsigned long spans_numlen(const unsigned long i)
{
  unsigned long pow = 1;
  unsigned long j   = 1;

  while (i / pow != 0)
    pow = power(pow, ++j);

  return j;
}

SPAS_LS_LIB char* spans_ls_display(const linespan_t *restr l)
{
  const unsigned int buff_sz = numlen(l->num) + l->str.len + 3;
  char *buff = calloc(1, buff_sz);
  snprintf(buff, buff_sz, "%d | %s", l->num, str_display(&l->str));
  return buff;
}

SPAS_LS_LIB inline void spans_ls_free(linespan_t *restr l)
{
  str_free(&l->str);
}

SPAS_LS_LIB inline linespan_t* spans_ls_table_fetch(const ls_table_t    *restr lt,
                                                    const unsigned long        l_idx)
{
  if (lt->len <= l_idx)
    return NULL;

  return lt->ptr + l_idx;
}

SPANS_SS_LIB strspan_t spans_ss_init(const linespan_t    *restr l,
                                    const unsigned long        l_idx,
                                    const unsigned long        start,
                                    const unsigned long        len)
{
  assert(l && "l is NULL in ss_init()");
  assert(l->str.len > start && "Starting index is out of l->str's bounds in ss_init()");
  assert(l->str.len >= start + len && "Tried fetching more than line contents in ss_init()");

  char *p = ls_fetch_self(l, start, len);
  assert(p && "Failed to fetch string from linespan in ss_init()");

  return (strspan_t) {
    .str = p,
    .clmn = start,
    .line_idx = l_idx,
  };
}

SPANS_SS_LIB int spans_ss_concat(      strspan_t *restr s1,
                          const strspan_t *restr s2)
{
  if (!s1 || !s2) return -1;
  if (s1->line_idx != s2->line_idx) return -2;
  if (s1->clmn > s2->clmn) return -3;

  str_push_str(&s1->str, s2->str.__s);

  return 0;
}

SPANS_SS_STATIC inline char* spans_chr_repeat(const char c, const unsigned long n)
{
  char *o = calloc(1, n + 1);
  if (!o) return NULL;
  memset(o, (int) c, (size_t) n);
  return 0;
}

SPANS_SS_LIB char* spans_ss_display(const ls_table_t *restr lt, const strspan_t *restr s, const char swiggly_chr)
{
  const linespan_t *l = ls_table_fetch(lt, s->line_idx);

  const unsigned long buf_sz = ((numlen(l->num) + 3) * 2) + l->str.len + s->clmn + s->str.len + 1;
  char * buf = calloc(1, buf_sz);

  char * spaces = chr_repeat(' ', s->clmn);
  char * swiggly = chr_repeat(swiggly_chr, s->str.len);
  if (!spaces || !swiggly) return NULL;

  snprintf(buf, buf_sz, "%s\n%d | %s%s",
           ls_display(l), l->num, spaces, swiggly);

  free(spaces);
  free(swiggly);
  return buf;
}

SPANS_SS_LIB span_t spans_ss_to(const ls_table_t *restr lt, const strspan_t *restr s)
{
  const linespan_t *l = ls_table_fetch(lt, s->line_idx);
  return (span_t) {
    .len = s->str.len,
    .clmn = s->clmn,
    .l_idx = s->line_idx,
    .line_num = l->num,
  };
}

SPANS_SS_LIB void ss_free(strspan_t *restr s)
{
  str_free(&s->str);
}

#endif /* SPAN_IMPL*/

#ifdef __cplusplus
}
#endif

#endif /* SPAN_DEF */

/* lexer.h
COPYRIGHT (C) 2026 ELSPLASH (electrsplash@gmail.com)
----------------------------------------------------

This header provides the functions for lexing a file,
or line. The lexing process is aligned with BASIIC.
it's own.

-------------------------------------------------
Licensed under the GNU General Public License V3.
*/

#ifndef BLEXER_DEF
#define BLEXER_DEF

/* NOTE ABOUT PREFIXES:
I prefer the prefixes be used anyway, just for the sake of
consistency.

This is so we know which struct/enum/function is from where.
*/
#if defined(BLEXER_IMPL) && !defined(SPANS_IMPL)
#define SPANS_IMPL
#endif

#include "macros.h"
#include "spans.h"

#ifndef BLEXER_LIB
#define BLEXER_LIB extern
#endif

#ifndef BLEXER_STATIC
#define BLEXER_STATIC static
#endif

#ifndef BLEXER_ASSERT

#include <assert.h>
#define BLEXER_ASSERT(check, message) assert(check && message)

#endif

#ifndef BLEXER_PANIC
#define BLEXER_PANIC(message) \
do                            \
{                             \
  puts("[BLEXER PANIC!]");    \
  puts(message);              \
  exit(1);                    \
}                             \
while (0)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
This enum is based off the qBasic documentation of every valid
character by their interpreter (I believe).

Though with some tokens added in for other things not included
there, like the TT_ID for identifiers, TT_NUM for numbers, and
TT_ALNUM for alphanumeric labels.
*/
typedef enum {
  /* -- SYMBOLIC TOKENS -- */
  TT_SPACE,

  TT_EQUAL,
  TT_EQEQ,

  TT_PLUS,
  TT_ADDEQ,

  TT_MINUS,
  TT_SUBEQ,

  TT_STAR,
  TT_MULEQ,

  TT_SLASH,
  TT_DIVEQ,

  TT_CARET,

  TT_L_PAREN,
  TT_R_PAREN,

  TT_PERCENT,
  TT_POUND,
  TT_DOLLAR,
  TT_EXCL,

  TT_L_BRACK,
  TT_LESSEQ,

  TT_R_BRACK,
  TT_GREATEQ,

  TT_NOT,

  TT_COMMA,
  TT_PERIOD,
  TT_SING_QUOT,
  TT_SEMICOLON,
  TT_COLON,

  TT_AMPERSAND,
  TT_ANDAND,

  TT_BAR,
  TT_OROR,

  TT_QUEST,

  TT_L_ANG_BRACK,
  TT_R_ANG_BRACK,

  TT_BACKSLASH,
  TT_NEWLINE,
  TT_AT,
  TT_UNDER,

  /* -- NON-SYMBOLIC TOKENS -- */
  TT_NS_ID,
  TT_NS_NUM,
  TT_NS_ALNUM,

  TT_NS_STRING,

  /* -- KEYWORD TOKENS -- */
  /* - PROGRAM FLOW - */
  TT_KW_RUN,
  TT_KW_CALL,
  TT_KW_CHAIN,
  TT_KW_RETURN,

  TT_KW_COMMON,

  TT_KW_DECLARE,
  TT_KW_DEF,

  TT_KW_DO,
  TT_KW_TO,
  TT_KW_UNTIL,

  TT_KW_IF,
  TT_KW_ELSE,
  TT_KW_ELSEIF,
  TT_KW_THEN,

  TT_KW_CASE,

  TT_KW_END,
  TT_KW_ENDDEF,

  TT_KW_EXIT,

  TT_KW_FOR,
  TT_KW_WHILE,
  TT_KW_LOOP,

  TT_KW_NEXT,
  TT_KW_STOP,

  TT_KW_GOSUB,
  TT_KW_GOTO,

  TT_KW_REM,

  TT_KW_SELECT,

  TT_KW_SUB,
  TT_KW_WEND,

  /* - I/O - */
  TT_KW_BEEP,
  TT_KW_PLAY,
  TT_KW_SOUND,

  TT_KW_CLEAR,
  TT_KW_CLS,

  TT_KW_CLOSE,
  TT_KW_OPEN,

  TT_KW_COLOR,
  TT_KW_PAINT,
  TT_KW_PALLETE,
  TT_KW_PMAP,
  TT_KW_PRESET,
  TT_KW_PSET,

  TT_KW_DRAW,
  TT_KW_LINE,

  TT_KW_SCREEN,
  TT_KW_VIEW,
  TT_KW_WINDOW,
  TT_KW_WIDTH,

  TT_KW_COM,
  TT_KW_DATA,
  TT_KW_ERROR,

  TT_KW_FIELD,
  TT_KW_LOCATE,

  TT_KW_GET,
  TT_KW_INPUT,
  TT_KW_KEY,

  TT_KW_LPRINT,

  TT_KW_OUT,

  TT_KW_PRINT,
  TT_KW_PUT,

  TT_KW_RANDOMIZE,

  TT_KW_READ,
  TT_KW_WRITE,

  TT_KW_RESTORE,

  TT_KW_SLEEP,

  /* - VARIABLES  - */
  TT_KW_AS,
  TT_KW_CONST,
  TT_KW_STATIC,
  TT_KW_SHARED,

  TT_KW_ERASE,

  TT_KW_DIM,
  TT_KW_REDIM,

  TT_KW_TYPE,

  TT_KW_DEFDBL,
  TT_KW_DEFINT,
  TT_KW_DEFLNG,
  TT_KW_DEFSNG,
  TT_KW_DEFSTR,

  /* - MATH - */
  TT_KW_CDBL,
  TT_KW_CINT,
  TT_KW_CLNG,
  TT_KW_COS,
  TT_KW_CSNG,
  TT_KW_CSRLIN,

  TT_KW_ABS,
  TT_KW_ATN,
  TT_KW_EXP,
  TT_KW_FIX,
  TT_KW_INT,
  TT_KW_LOG,
  TT_KW_SGN,
  TT_KW_SIN,
  TT_KW_SQR,
  TT_KW_TAN,

  /* - STRING -*/
  TT_KW_ASC,
  TT_KW_HEXS,
  TT_KW_OCTS,
  TT_KW_CHRS,

  TT_KW_INKEYS,
  TT_KW_INSTR,

  TT_KW_LCASES,
  TT_KW_UCASES,

  TT_KW_LEFTS,
  TT_KW_MIDS,
  TT_KW_RIGHTS,

  TT_KW_LTRIMS,
  TT_KW_RTRIMS,

  TT_KW_LEN,

  TT_KW_SPACES,

  TT_KW_STRS,
  TT_KW_STRINGS,

  TT_KW_VAL,

  /* - MISC - */
  TT_KW_EOF,
  TT_KW_ERL,
  TT_KW_ERR,
  TT_KW_FRE,
  TT_KW_INP,
  TT_KW_LOF,
  TT_KW_POS,

  TT_KW_POINT,
  TT_KW_PEEK,
  TT_KW_PEEKS, /* String version of the other function I believe? - ESplash
                  (10.16.2026) */

  TT_KW_VARPTR,
  TT_KW_VARSEG,

  TT_KW_TIME,
  TT_KW_DATE,

  /* - OPERATORS - */
  TT_KW_AND,
  TT_KW_EQV,
  TT_KW_IMP,
  TT_KW_MOD,
  TT_KW_NOT,
  TT_KW_OR,
  TT_KW_XOR,

  /* - GRAPHICS - NOTE: Some are in the I/O */
  TT_KW_CIRCLE,

  /* - FILE SYSTEM - */
  TT_KW_CHDIR,
  TT_KW_MKDIR,
  TT_KW_RMDIR,

  TT_KW_FILES,
  TT_KW_NAME,

  TT_KW_KILL,

  TT_KW_SHELL,
}
blexer_tokentype_t;

/*
The main datatype we're going to work with.

The type declares it's type of token, see "blexer_tokentype_t" for
more information.

The span contains the section of the linespan that contains said
token.
*/
typedef struct
{
  blexer_tokentype_t type;
  spans_strspan_t   span;
}
blexer_lextoken_t;

/*
This vector is pretty simple. Just a vector implementation for
our lexer tokens to be stored in.
*/
typedef struct
{
  blexer_lextoken_t * ptr;
  size_t              len;
  size_t              cap;
}
blexer_lextok_vec_t;

/* The lexer state, for the tokenize line function. */
typedef enum
{
  LEXST_ID,
  LEXST_NUM,
  LEXST_ALNUM,
  LEXST_SPACE,
  LEXST_ETC,
}
blexer_lexer_state_t;

/*
This function validates the lexer token vector if it's somehow
not valid. Though if these functions somehow fail, or you
manually manipulated them, you can never be too sure.

This is made so that you can check via `if (blexer_ltvec_validate())'
or `if (!blexer_ltvec_validate())' so you don't have to add `== 0'.

This returns the values of:

-2 - ltvec->len is more than ltvec->cap
-1 - ltvec->ptr is NULL.
 0 - ltvec is NULL>
 1 - No issues.
*/
BLEXER_STATIC int blexer_ltvec_validate(const blexer_lextok_vec_t *restr ltvec);

/*
This function creates an blexer_lextok_vec_t datatype, just for you
to not do it manually. The ptr will be heap allocated by default.
*/
BLEXER_STATIC blexer_lextok_vec_t blexer_ltvec_creat(void);

/*
This function fetches a lexer token for you.

This couplees some checks, like validating the lexer token vector,
and doing bounds checking for the idx.

NOTE: If this function should return a pointer instead, I believe we
      should simply move that to another function instead.
*/
BLEXER_STATIC blexer_lextoken_t blexer_ltvec_fetch(const blexer_lextok_vec_t *restr ltvec,
                                                   const size_t                     idx);

/*
This function is used to push to the lexer token vector
(blexer_lextoken_vec_t).

The reason why it's static, and not part of the library is
that it's not supposed to give function mutators to the vector,
and only view it.
*/
BLEXER_STATIC int blexer_ltvec_push(blexer_lextok_vec_t *restr ltv, const blexer_lextoken_t p);

/*
Thi fetches the token type from blexer_lexer_state_t, and also
fetches spans_strspan_t from the parameters given. This is repeated
twice so it is coupled.

This is supposed to be a helper function for blexer_tokenize_line,
so I don't think you'd want this in the library anyway.
*/
BLEXER_STATIC void blexer_fetch_n_push(const spans_linespan_t     *restr l,
                                             blexer_lextok_vec_t  *restr out,
                                       const blexer_lexer_state_t        stat,
                                       const size_t                      l_idx,
                                       const size_t                      start,
                                       const size_t                      len);

/*
This is a function that frees the lexer token's strspan, for the
sake of verbosity.
*/
BLEXER_LIB int blexer_ltok_free(blexer_lextoken_t *restr ltok);

/*
This frees the datatype `blexer_lextok_vec_t`, which may contain several
`spans_strspans_t`, so this function will free the string spans for you,
though it doesn't free the `ltv` parameter given, so if it is heap
allocated you may free it on the callee, not here.
*/
BLEXER_LIB int blexer_ltvec_free(blexer_lextok_vec_t *restr ltvec);

/*
This function tokenizes the special logic for strings in qBasic, though
it has to know where it should start first.
*/
BLEXER_LIB blexer_lextoken_t blexer_tokenize_string(const spans_linespan_t *restr l,
                                                 const size_t                  l_idx,
                                                 const size_t                  start,
                                                       size_t           *restr len);

/*
This function is supposed to be the main tokenizer, so it should
be the only one in this header.

Why not make a blexer_tokenize_file(), you may ask? Well, it's
because BASIC is line based, and the main loop is simple enough
for you to inline yourself.

Remember to free the returned value, as it is heap allocated.
*/
BLEXER_LIB blexer_lextok_vec_t blexer_tokenize_line(const spans_linespan_t *restr l,
                                                 const size_t                  l_idx);

#define tokentype_t   blexer_tokentype_t
#define lextoken_t    blexer_lextoken_t
#define lextok_vec_t  blexer_lextok_vec_t
#define lexer_state_t blexer_lexer_state_t

/* -- BLEXER STATIC -- */
/* - Creation - */
#define ltvec_creat()                                 blexer_ltvec_creat()

/* - Manipulation - */
#define ltvec_push(ltv, p)                            blexer_ltvec_push(ltv, p)
#define fetch_n_push(l, out, stat, l_idx, start, len) blexer_fetch_n_push(l, out, stat, l_idx, start, len)

/* -- BLEXER LIBRARY -- */
/* - Interface - */
#define tokenize_line(l, l_idx) blexer_tokenize_line(l, l_idx)

/* - Misc. - */
#define ltvec_free(ltvec)       blexer_ltvec_free(ltvec)
#define ltok_free(ltok)         blexer_ltok_free(ltok)

/* --- IMPLEMENTATION --- */

#ifdef BLEXER_IMPL

#include <stdlib.h>
#include <ctype.h>

#ifndef BLEXER_LTVEC_INITSZ
#define BLEXER_LTVEC_INITSZ 8
#endif

BLEXER_STATIC int blexer_ltvec_validate(const blexer_lextok_vec_t *restr ltvec)
{
  if (!ltvec)
    return 0;

  if (!ltvec->ptr)
    return -1;

  if (ltvec->len > ltvec->cap)
    return -2;

  return 1;
}

BLEXER_STATIC blexer_lextok_vec_t blexer_ltvec_creat(void)
{
  blexer_lextoken_t *ptr = calloc(sizeof(*ptr), BLEXER_LTVEC_INITSZ);
  BLEXER_ASSERT(ptr, "Failed to allocate in blexer_ltvec_creat()");

  return (blexer_lextok_vec_t) {
    .ptr = ptr,
    .len      = 0,
    .cap      = BLEXER_LTVEC_INITSZ
  };
}

BLEXER_STATIC blexer_lextoken_t blexer_ltvec_fetch(const blexer_lextok_vec_t *restr ltvec,
                                                   const size_t                     idx)
{
  BLEXER_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is not valid in blexer_ltvec_fetch()");
  BLEXER_ASSERT(idx >= ltvec->len, "idx is out of bound sin blexer_ltvec_fetch");

  return ltvec->ptr[idx];
}

BLEXER_STATIC int blexer_ltvec_push(blexer_lextok_vec_t *restr ltvec, const blexer_lextoken_t p)
{
  int rval = blexer_ltvec_validate(ltvec) - 1;
  if (rval != 0) return rval;

  if (ltvec->len + 1 >= ltvec->cap)
  {
    while (ltvec->len + 1 >= ltvec->cap)
      ltvec->cap *= 2;
    ltvec->ptr = realloc(ltvec->ptr, ltvec->cap * sizeof(*ltvec->ptr));
    rval++;
  }

  ltvec->ptr[ltvec->len++] = p;
  return rval;
}

BLEXER_STATIC int blexer_ss_to_lower(spans_strspan_t *restr ss)
{
  if (!ss) return -1;
  for (size_t i = 0; i < ss->str.len; i++)
  {
    ss->str.__s[i] = tolower(ss->str.__s[i]);
  }
  return 0;
}

BLEXER_STATIC void blexer_fetch_n_push(const spans_linespan_t    *restr l,
                                           blexer_lextok_vec_t  *restr out,
                                     const blexer_lexer_state_t        stat,
                                     const size_t                     l_idx,
                                     const size_t                     start,
                                     const size_t                     len)
{
  BLEXER_ASSERT(stat != LEXST_ETC, "State is an impossible state (LEXST_ETC in blexer_tokenize_line())");
  strspan_t ss = spans_ss_init(l, l_idx, start, len);
  blexer_ss_to_lower(&ss);
  blexer_tokentype_t token_type;

  switch (stat)
  {
  case LEXST_ID:
    token_type = TT_NS_ID;
    if (spans_ss_equal_chrp(&ss, "run"))
      token_type = TT_KW_RUN;
    else if (spans_ss_equal_chrp(&ss, "call"))
      token_type = TT_KW_CALL;
    else if (spans_ss_equal_chrp(&ss, "chain"))
      token_type = TT_KW_CHAIN;
    else if (spans_ss_equal_chrp(&ss, "return"))
      token_type = TT_KW_RETURN;

    else if (spans_ss_equal_chrp(&ss, "common"))
      token_type = TT_KW_COMMON;

    else if (spans_ss_equal_chrp(&ss, "declare"))
      token_type = TT_KW_DECLARE;
    else if (spans_ss_equal_chrp(&ss, "def"))
      token_type = TT_KW_DEF;

    else if (spans_ss_equal_chrp(&ss, "do"))
      token_type = TT_KW_DO;
    else if (spans_ss_equal_chrp(&ss, "to"))
      token_type = TT_KW_TO;
    else if (spans_ss_equal_chrp(&ss, "until"))
      token_type = TT_KW_UNTIL;

    else if (spans_ss_equal_chrp(&ss, "if"))
      token_type = TT_KW_IF;
    else if (spans_ss_equal_chrp(&ss, "else"))
      token_type = TT_KW_ELSE;
    else if (spans_ss_equal_chrp(&ss, "elseif"))
      token_type = TT_KW_ELSEIF;
    else if (spans_ss_equal_chrp(&ss, "then"))
      token_type = TT_KW_THEN;

    else if (spans_ss_equal_chrp(&ss, "case"))
      token_type = TT_KW_CASE;

    else if (spans_ss_equal_chrp(&ss, "end"))
      token_type = TT_KW_END;
    else if (spans_ss_equal_chrp(&ss, "enddef"))
      token_type = TT_KW_ENDDEF;

    else if (spans_ss_equal_chrp(&ss, "exit"))
      token_type = TT_KW_EXIT;

    else if (spans_ss_equal_chrp(&ss, "for"))
      token_type = TT_KW_FOR;
    else if (spans_ss_equal_chrp(&ss, "while"))
      token_type = TT_KW_WHILE;
    else if (spans_ss_equal_chrp(&ss, "loop"))
      token_type = TT_KW_LOOP;

    else if (spans_ss_equal_chrp(&ss, "next"))
      token_type = TT_KW_NEXT;
    else if (spans_ss_equal_chrp(&ss, "stop"))
      token_type = TT_KW_STOP;

    else if (spans_ss_equal_chrp(&ss, "gosub"))
      token_type = TT_KW_GOSUB;
    else if (spans_ss_equal_chrp(&ss, "goto"))
      token_type = TT_KW_GOTO;

    else if (spans_ss_equal_chrp(&ss, "rem"))
      token_type = TT_KW_REM;

    else if (spans_ss_equal_chrp(&ss, "select"))
      token_type = TT_KW_SELECT;

    else if (spans_ss_equal_chrp(&ss, "sub"))
      token_type = TT_KW_SUB;
    else if (spans_ss_equal_chrp(&ss, "wend"))
      token_type = TT_KW_WEND;

    else if (spans_ss_equal_chrp(&ss, "beep"))
      token_type = TT_KW_BEEP;
    else if (spans_ss_equal_chrp(&ss, "play"))
      token_type = TT_KW_PLAY;
    else if (spans_ss_equal_chrp(&ss, "sound"))
      token_type = TT_KW_SOUND;

    else if (spans_ss_equal_chrp(&ss, "clear"))
      token_type = TT_KW_CLEAR;
    else if (spans_ss_equal_chrp(&ss, "cls"))
      token_type = TT_KW_CLS;

    else if (spans_ss_equal_chrp(&ss, "close"))
      token_type = TT_KW_CLOSE;
    else if (spans_ss_equal_chrp(&ss, "open"))
      token_type = TT_KW_OPEN;

    else if (spans_ss_equal_chrp(&ss, "color"))
      token_type = TT_KW_COLOR;
    else if (spans_ss_equal_chrp(&ss, "paint"))
      token_type = TT_KW_PAINT;
    else if (spans_ss_equal_chrp(&ss, "pallete"))
      token_type = TT_KW_PALLETE;
    else if (spans_ss_equal_chrp(&ss, "pmap"))
      token_type = TT_KW_PMAP;
    else if (spans_ss_equal_chrp(&ss, "preset"))
      token_type = TT_KW_PRESET;
    else if (spans_ss_equal_chrp(&ss, "pset"))
      token_type = TT_KW_PSET;

    else if (spans_ss_equal_chrp(&ss, "draw"))
      token_type = TT_KW_DRAW;
    else if (spans_ss_equal_chrp(&ss, "line"))
      token_type = TT_KW_LINE;

    else if (spans_ss_equal_chrp(&ss, "screen"))
      token_type = TT_KW_SCREEN;
    else if (spans_ss_equal_chrp(&ss, "view"))
      token_type = TT_KW_VIEW;
    else if (spans_ss_equal_chrp(&ss, "window"))
      token_type = TT_KW_WINDOW;
    else if (spans_ss_equal_chrp(&ss, "width"))
      token_type = TT_KW_WIDTH;

    else if (spans_ss_equal_chrp(&ss, "com"))
      token_type = TT_KW_COM;
    else if (spans_ss_equal_chrp(&ss, "data"))
      token_type = TT_KW_DATA;
    else if (spans_ss_equal_chrp(&ss, "error"))
      token_type = TT_KW_ERROR;

    else if (spans_ss_equal_chrp(&ss, "field"))
      token_type = TT_KW_FIELD;
    else if (spans_ss_equal_chrp(&ss, "locate"))
      token_type = TT_KW_LOCATE;

    else if (spans_ss_equal_chrp(&ss, "get"))
      token_type = TT_KW_GET;
    else if (spans_ss_equal_chrp(&ss, "input"))
      token_type = TT_KW_INPUT;
    else if (spans_ss_equal_chrp(&ss, "key"))
      token_type = TT_KW_KEY;

    else if (spans_ss_equal_chrp(&ss, "lprint"))
      token_type = TT_KW_LPRINT;

    else if (spans_ss_equal_chrp(&ss, "out"))
      token_type = TT_KW_OUT;

    else if (spans_ss_equal_chrp(&ss, "print"))
      token_type = TT_KW_PRINT;
    else if (spans_ss_equal_chrp(&ss, "put"))
      token_type = TT_KW_PUT;

    else if (spans_ss_equal_chrp(&ss, "randomize"))
      token_type = TT_KW_RANDOMIZE;

    else if (spans_ss_equal_chrp(&ss, "read"))
      token_type = TT_KW_READ;
    else if (spans_ss_equal_chrp(&ss, "write"))
      token_type = TT_KW_WRITE;

    else if (spans_ss_equal_chrp(&ss, "restore"))
      token_type = TT_KW_RESTORE;

    else if (spans_ss_equal_chrp(&ss, "sleep"))
      token_type = TT_KW_SLEEP;

    else if (spans_ss_equal_chrp(&ss, "as"))
      token_type = TT_KW_AS;
    else if (spans_ss_equal_chrp(&ss, "const"))
      token_type = TT_KW_CONST;
    else if (spans_ss_equal_chrp(&ss, "static"))
      token_type = TT_KW_STATIC;
    else if (spans_ss_equal_chrp(&ss, "shared"))
      token_type = TT_KW_SHARED;

    else if (spans_ss_equal_chrp(&ss, "erase"))
      token_type = TT_KW_ERASE;

    else if (spans_ss_equal_chrp(&ss, "dim"))
      token_type = TT_KW_DIM;
    else if (spans_ss_equal_chrp(&ss, "redim"))
      token_type = TT_KW_REDIM;

    else if (spans_ss_equal_chrp(&ss, "type"))
      token_type = TT_KW_TYPE;

    else if (spans_ss_equal_chrp(&ss, "defdbl"))
      token_type = TT_KW_DEFDBL;
    else if (spans_ss_equal_chrp(&ss, "defint"))
      token_type = TT_KW_DEFINT;
    else if (spans_ss_equal_chrp(&ss, "deflng"))
      token_type = TT_KW_DEFLNG;
    else if (spans_ss_equal_chrp(&ss, "defsng"))
      token_type = TT_KW_DEFSNG;
    else if (spans_ss_equal_chrp(&ss, "defstr"))
      token_type = TT_KW_DEFSTR;

    else if (spans_ss_equal_chrp(&ss, "cdbl"))
      token_type = TT_KW_CDBL;
    else if (spans_ss_equal_chrp(&ss, "cint"))
      token_type = TT_KW_CINT;
    else if (spans_ss_equal_chrp(&ss, "clng"))
      token_type = TT_KW_CLNG;
    else if (spans_ss_equal_chrp(&ss, "cos"))
      token_type = TT_KW_COS;
    else if (spans_ss_equal_chrp(&ss, "csng"))
      token_type = TT_KW_CSNG;
    else if (spans_ss_equal_chrp(&ss, "csrlin"))
      token_type = TT_KW_CSRLIN;

    else if (spans_ss_equal_chrp(&ss, "abs"))
      token_type = TT_KW_ABS;
    else if (spans_ss_equal_chrp(&ss, "atn"))
      token_type = TT_KW_ATN;
    else if (spans_ss_equal_chrp(&ss, "exp"))
      token_type = TT_KW_EXP;
    else if (spans_ss_equal_chrp(&ss, "fix"))
      token_type = TT_KW_FIX;
    else if (spans_ss_equal_chrp(&ss, "int"))
      token_type = TT_KW_INT;
    else if (spans_ss_equal_chrp(&ss, "log"))
      token_type = TT_KW_LOG;
    else if (spans_ss_equal_chrp(&ss, "sgn"))
      token_type = TT_KW_SGN;
    else if (spans_ss_equal_chrp(&ss, "sin"))
      token_type = TT_KW_SIN;
    else if (spans_ss_equal_chrp(&ss, "sqr"))
      token_type = TT_KW_SQR;
    else if (spans_ss_equal_chrp(&ss, "tan"))
      token_type = TT_KW_TAN;

    else if (spans_ss_equal_chrp(&ss, "asc"))
      token_type = TT_KW_ASC;
    else if (spans_ss_equal_chrp(&ss, "hex$"))
      token_type = TT_KW_HEXS;
    else if (spans_ss_equal_chrp(&ss, "oct$"))
      token_type = TT_KW_OCTS;

    else if (spans_ss_equal_chrp(&ss, "inkey$"))
      token_type = TT_KW_INKEYS;
    else if (spans_ss_equal_chrp(&ss, "instr"))
      token_type = TT_KW_INSTR;

    else if (spans_ss_equal_chrp(&ss, "lcase$"))
      token_type = TT_KW_LCASES;
    else if (spans_ss_equal_chrp(&ss, "ucase$"))
      token_type = TT_KW_UCASES;

    else if (spans_ss_equal_chrp(&ss, "left$"))
      token_type = TT_KW_LEFTS;
    else if (spans_ss_equal_chrp(&ss, "mid$"))
      token_type = TT_KW_MIDS;
    else if (spans_ss_equal_chrp(&ss, "right$"))
      token_type = TT_KW_RIGHTS;

    else if (spans_ss_equal_chrp(&ss, "ltrim$"))
      token_type = TT_KW_LTRIMS;
    else if (spans_ss_equal_chrp(&ss, "rtrim$"))
      token_type = TT_KW_RTRIMS;

    else if (spans_ss_equal_chrp(&ss, "len"))
      token_type = TT_KW_LEN;

    else if (spans_ss_equal_chrp(&ss, "space$"))
      token_type = TT_KW_SPACES;

    else if (spans_ss_equal_chrp(&ss, "str$"))
      token_type = TT_KW_STRS;
    else if (spans_ss_equal_chrp(&ss, "string$"))
      token_type = TT_KW_STRINGS;

    else if (spans_ss_equal_chrp(&ss, "val"))
      token_type = TT_KW_VAL;

    else if (spans_ss_equal_chrp(&ss, "eof"))
      token_type = TT_KW_EOF;
    else if (spans_ss_equal_chrp(&ss, "erl"))
      token_type = TT_KW_ERL;
    else if (spans_ss_equal_chrp(&ss, "err"))
      token_type = TT_KW_ERR;
    else if (spans_ss_equal_chrp(&ss, "fre"))
      token_type = TT_KW_FRE;
    else if (spans_ss_equal_chrp(&ss, "inp"))
      token_type = TT_KW_INP;
    else if (spans_ss_equal_chrp(&ss, "lof"))
      token_type = TT_KW_LOF;
    else if (spans_ss_equal_chrp(&ss, "pos"))
      token_type = TT_KW_POS;

    else if (spans_ss_equal_chrp(&ss, "point"))
      token_type = TT_KW_POINT;
    else if (spans_ss_equal_chrp(&ss, "peek"))
      token_type = TT_KW_PEEK;
    else if (spans_ss_equal_chrp(&ss, "peek$"))
      token_type = TT_KW_PEEKS;

    else if (spans_ss_equal_chrp(&ss, "varptr"))
      token_type = TT_KW_VARPTR;
    else if (spans_ss_equal_chrp(&ss, "varseg"))
      token_type = TT_KW_VARSEG;

    else if (spans_ss_equal_chrp(&ss, "time"))
      token_type = TT_KW_TIME;
    else if (spans_ss_equal_chrp(&ss, "date"))
      token_type = TT_KW_DATE;

    else if (spans_ss_equal_chrp(&ss, "and"))
      token_type = TT_KW_AND;
    else if (spans_ss_equal_chrp(&ss, "eqv"))
      token_type = TT_KW_EQV;
    else if (spans_ss_equal_chrp(&ss, "imp"))
      token_type = TT_KW_IMP;
    else if (spans_ss_equal_chrp(&ss, "mod"))
      token_type = TT_KW_MOD;
    else if (spans_ss_equal_chrp(&ss, "not"))
      token_type = TT_KW_NOT;
    else if (spans_ss_equal_chrp(&ss, "or"))
      token_type = TT_KW_OR;
    else if (spans_ss_equal_chrp(&ss, "xor"))
      token_type = TT_KW_XOR;

    else if (spans_ss_equal_chrp(&ss, "circle"))
      token_type = TT_KW_CIRCLE;

    else if (spans_ss_equal_chrp(&ss, "chdir"))
      token_type = TT_KW_CHDIR;
    else if (spans_ss_equal_chrp(&ss, "mkdir"))
      token_type = TT_KW_MKDIR;
    else if (spans_ss_equal_chrp(&ss, "rmdir"))
      token_type = TT_KW_RMDIR;

    else if (spans_ss_equal_chrp(&ss, "files"))
      token_type = TT_KW_FILES;
    else if (spans_ss_equal_chrp(&ss, "name"))
      token_type = TT_KW_NAME;

    else if (spans_ss_equal_chrp(&ss, "kill"))
      token_type = TT_KW_KILL;

    else if (spans_ss_equal_chrp(&ss, "shell"))
      token_type = TT_KW_SHELL;

    break;

  case LEXST_NUM:
    token_type = TT_NS_NUM;
    break;

  case LEXST_ALNUM:
    token_type = TT_NS_ALNUM;
    break;

  case LEXST_SPACE:
    token_type = TT_SPACE;
    break;

  default:
    break;
  }

  const blexer_lextoken_t ltok = (blexer_lextoken_t) {
    .type = token_type,
    .span = ss,
  };
  blexer_ltvec_push(out, ltok);
}

BLEXER_LIB int blexer_ltok_free(blexer_lextoken_t *restr ltok)
{
  if (!ltok)
    return -1;

  spans_ss_free(&ltok->span);

  return 0;
}

BLEXER_LIB int blexer_ltvec_free(blexer_lextok_vec_t *restr ltvec)
{
  int rval = blexer_ltvec_validate(ltvec) - 1;
  if (rval != 0) return rval; 

  for (size_t i = 0; i < ltvec->len; i++)
  {
    if (blexer_ltok_free(&ltvec->ptr[i]) != 0) return i;
  }

  free(ltvec->ptr);
  return 0;
}

BLEXER_LIB blexer_lextoken_t blexer_tokenize_string(const spans_linespan_t *restr l,
                                                 const size_t                  l_idx,
                                                 const size_t                  start,
                                                       size_t           *restr len) /* This is to keep track of where the string ended. */
{
  BLEXER_ASSERT(l, "Passed l parameter is NULL");
  BLEXER_ASSERT(len, "Passed len parameter is NULL");
  BLEXER_ASSERT(*len == 0, "Passed len parameter is invalid");

  /*
  Manual iteration is required.
  We assume the linespan starts at a quotation mark,
  like `"Hello World!"`, so we can skip it gladly.

  If no quotation mark is found we need to end at the
  end of the line, hence lenght needs to be subtracted
  by one.
  */
  while ((*len)++ < l->str.len - 1)
  {
    if (l->str.__s[*len + start] == '"')
    {
      if (*len != l->str.len - 1 && l->str.__s[++(*len) + start] == '"')
      {
        continue;
      }
      break;
    }
  }

  blexer_lextoken_t out = (blexer_lextoken_t) {
    .type = TT_NS_STRING,
    .span = spans_ss_init(l, l_idx, start, *len),
  };

  /*- Format out.span for future parsing -*/

  /* Trim out the double quotation marks */

  if (out.span.str.__s[out.span.str.len - 1] == '"')
    spans_str_popr(&out.span.str, 1); /* Since it's possible that the right quote can be missing. */
  spans_str_popl(&out.span.str, 1);

  /* Format the quotation mark escape sequences */
  size_t i = 0;
  while (i < out.span.str.len)
  {
    if (out.span.str.__s[i] == '"')
      spans_str_pop_at(&out.span.str, i, 1);
    i++;
  }

  return out;
}

BLEXER_LIB blexer_lextok_vec_t blexer_tokenize_line(const spans_linespan_t *restr l,
                                                 const size_t                  l_idx)
{
  BLEXER_ASSERT(l, "Passed l parameter is NULL");

  blexer_lextok_vec_t   out   = blexer_ltvec_creat();
  blexer_lexer_state_t  stat  = LEXST_ETC;
  long                 start = -1;
  size_t               len   = 0;

  size_t i = 0;
  while (i < l->str.len)
  {
    const char c = l->str.__s[i];

    if (start == -1) start = i;
    len++;

    if (isalpha(c) && stat != LEXST_NUM)
    {
      switch (stat)
      {
      case LEXST_ALNUM:
        /* Fallthrough */
      case LEXST_ID:
        break;

      case LEXST_SPACE:
        blexer_fetch_n_push(l, &out, stat, l_idx, start, len - 1);
        len   = 1;
        start = i;
        /* Fallthrough*/
      case LEXST_ETC:
        stat  = LEXST_ID;
        break;

      default:
        BLEXER_PANIC("Invalid state in blexer_tokenize_line() (1)");
      }
      i++;
      continue;
    } else if (isdigit(c))
    {
      switch (stat)
      {
      case LEXST_ID:
        stat = LEXST_ALNUM;
        /* Fallthrough */
      case LEXST_ALNUM:
        /* Fallthrough */
      case LEXST_NUM:
        break;

      case LEXST_SPACE:
        blexer_fetch_n_push(l, &out, stat, l_idx, start, len - 1);
        len   = 1;
        start = i;
        /* Fallthrough */
      case LEXST_ETC:
        stat = LEXST_NUM;
        break;

      default:
        BLEXER_PANIC("Invalid state in blexer_tokenize_line() (2)");
      }
      i++;
      continue;
    } else if (c == ' ' && stat == LEXST_SPACE) {
      i++;
      continue;
    }

    if (len > 1)
    {
      if (stat == LEXST_ID || LEXST_ALNUM)
      {
        switch (l->str.__s[i + 1])
        {
        case '!':
          /* Fallthrough */
        case '&':
          /* Fallthrough */
        case '%':
          /* Fallthrough */
        case '#':
          /* Fallthrough */
        case '$':
          i++;
          len++;
          break;

        default:
          break;
        }
      }
      blexer_fetch_n_push(l, &out, stat, l_idx, start, len - 1);
      len   = 1;
      start = i;
    }
    else if (c == '\n')
    {
      if (len - 1 != 0) blexer_fetch_n_push(l, &out, stat, l_idx, start, len - 1);
      len   = 1;
      start = i;
      const spans_strspan_t ss = spans_ss_init(l, l_idx, start, len);
      blexer_ltvec_push(&out, (blexer_lextoken_t) {
        .span = ss,
        .type = TT_NEWLINE,
      });
      break;
    }

    blexer_tokentype_t token_type = TT_NS_ID;
    switch (c)
    {
    case ' ':
      stat = LEXST_SPACE;
      i++;
      continue;

    case '=':
      token_type = TT_EQUAL;
      if (l->str.__s[i + 1] == '=')
      {
        len++;
        i++;
        token_type = TT_EQEQ;
      }
      break;

    case '+':
      token_type = TT_PLUS;
      if (l->str.__s[i + 1] == '=')
      {
        len++;
        i++;
        token_type = TT_ADDEQ;
      }
      break;

    case '-':
      token_type = TT_MINUS;
      if (l->str.__s[i + 1] == '=')
      {
        len++;
        i++;
        token_type = TT_SUBEQ;
      }
      break;

    case '*':
      token_type = TT_STAR;
      if (l->str.__s[i + 1] == '=')
      {
        len++;
        i++;
        token_type = TT_MULEQ;
      }
      break;

    case '/':
      token_type = TT_SLASH;
      if (l->str.__s[i + 1] == '=')
      {
        len++;
        i++;
        token_type = TT_DIVEQ;
      }
      break;

    case '^':
      token_type = TT_CARET;
      break;

    case '(':
      token_type = TT_L_PAREN;
      break;

    case ')':
      token_type = TT_R_PAREN;
      break;

    case '%':
      token_type = TT_PERCENT;
      break;

    case '#':
      token_type = TT_POUND;
      break;

    case '$':
      token_type = TT_DOLLAR;
      break;

    case '!':
      token_type = TT_EXCL;
      break;

    case '[':
      token_type = TT_L_BRACK;
      break;

    case ']':
      token_type = TT_R_BRACK;
      break;

    case ',':
      token_type = TT_COMMA;
      break;

    case '.':
      token_type = TT_PERIOD;
      break;

    case '\'':
      token_type = TT_SING_QUOT;
      break;

    case '"':
      {
        size_t string_len = 0;
        blexer_lextoken_t ltok = blexer_tokenize_string(l, l_idx, i, &string_len);
        blexer_ltvec_push(&out, ltok);
        i += string_len;
      }
      start = -1;
      len   = 0;
      continue;

    case ';':
      token_type = TT_SEMICOLON;
      break;

    case ':':
      token_type = TT_COLON;
      break;

    case '&':
      token_type = TT_AMPERSAND;
      if (l->str.__s[i + 1] == '&')
      {
        len++;
        i++;
        token_type = TT_ANDAND;
      }
      break;

    case '|':
      token_type = TT_BAR;
      if (l->str.__s[i + 1] == '|')
      {
        len++;
        i++;
        token_type = TT_OROR;
      }
      break;

    case '?':
      token_type = TT_QUEST;
      break;

    case '<':
      token_type = TT_L_ANG_BRACK;
      if (l->str.__s[i + 1] == '=')
      {
        len++;
        i++;
        token_type = TT_LESSEQ;
      }
      else if (l->str.__s[i + 1] == '>')
      {
        len++;
        i++;
        token_type = TT_NOT;
      }
      break;

    case '>':
      token_type = TT_R_ANG_BRACK;
      if (l->str.__s[i + 1] == '=')
      {
        len++;
        i++;
        token_type = TT_GREATEQ;
      }
      break;

    case '\\':
      token_type = TT_BACKSLASH;
      break;

    case '@':
      token_type = TT_AT;
      break;

    case '_':
      token_type = TT_UNDER;
      break;

    default:
      /* NOTE: We are implicitly making this an identifier. */
      break;
    }
    const strspan_t ss = spans_ss_init(l, l_idx, start, len);
    const blexer_lextoken_t ltok = (blexer_lextoken_t) {
      .span = ss,
      .type = token_type,
    };

    blexer_ltvec_push(&out, ltok);

    start = -1;
    len   =  0;
    i++;
  }

  if (len > 0) blexer_fetch_n_push(l, &out, stat, l_idx, start, len);

  return out;
}

#endif /* BLEXER_IMPL */

#ifdef __cplusplus
}
#endif

#endif /* BLEXER_DEF */

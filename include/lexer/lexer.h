/* lexer.h
This header provides the functions for lexing a file, or line.
The lexing process is aligned with BASIIC.
it's own.
*/

#ifndef LEXER_DEF
#define LEXER_DEF

/* NOTE ABOUT PREFIXES:
I prefer the prefixes be used anyway, just for the sake of
consistency.

This is so we know which struct/enum/function is from where.
*/
#if defined(LEXER_IMPL) && !defined(SPANS_IMPL)
#define SPANS_IMPL
#endif

#ifndef LEXER_LIB
#define LEXER_LIB extern
#endif

#ifndef LEXER_STATIC
#define LEXER_STATIC static
#endif

#ifndef LEXER_ASSERT
#define LEXER_ASSERT(x) assert(x)
#endif

#include "spans.h"

#ifdef __cplusplus
extern "C" {
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
  TT_PLUS,
  TT_MINUS,
  TT_STAR,
  TT_SLASH,

  TT_CARET,

  TT_L_PAREN,
  TT_R_PAREN,

  TT_PERCENT,
  TT_POUND,
  TT_DOLLAR,
  TT_EXCL,

  TT_L_BRACK,
  TT_R_BRACK,

  TT_COMMA,
  TT_PERIOD,
  TT_SING_QUOT,
  TT_DOUB_QUOT,
  TT_SEMICOLON,
  TT_COLON,

  TT_AMPERSAND,
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
  TT_KW_HEX,
  TT_KW_OCT,

  TT_KW_INKEY,
  TT_KW_INSTR,

  TT_KW_LCASE,
  TT_KW_UCASE,

  TT_KW_LEFT,
  TT_KW_MID,
  TT_KW_RIGHT,

  TT_KW_LTRIM,
  TT_KW_RTRIM,

  TT_KW_SPACE,

  TT_KW_STR,
  TT_KW_STRING,

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
} lexer_tokentype_t;

/*
The main datatype we're going to work with.

The type declares it's type of token, see "lexer_tokentype_t" for
more information.

The span contains the section of the linespan that contains said
token.
*/
typedef struct {
  lexer_tokentype_t type;
  spans_strspan_t   span;
} lexer_lextoken_t;

/*
This vector is pretty simple. Just a vector implementation for
our lexer tokens to be stored in.
*/
typedef struct {
  lexer_lextoken_t * ltok_ptr;
  unsigned int       len;
  unsigned int       cap;
} lexer_lextok_vec_t;

/* The lexer state, for the tokenize line function. */
typedef enum {
  LEXST_ID,
  LEXST_NUM,
  LEXST_ALNUM,
  LEXST_SPACE,
  LEXST_ETC,
} lexer_lexer_state_t;

/*
This function creates an lexer_lextok_vec_t datatype, just for you
to not do it manually. The ptr will be heap allocated by default.
*/
LEXER_STATIC lexer_lextok_vec_t lexer_ltvec_creat(void);

/*
This function is used to push to the lexer token vector
(lexer_lextoken_vec_t).

The reason why it's static, and not part of the library is
that it's not supposed to give function mutators to the vector,
and only view it.
*/
LEXER_STATIC int lexer_ltvec_push(lexer_lextok_vec_t *restr ltv, const lexer_lextoken_t p);

/*
Thi fetches the token type from lexer_lexer_state_t, and also
fetches spans_strspan_t from the parameters given. This is repeated
twice so it is coupled.

This is supposed to be a helper function for lexer_tokenize_line,
so I don't think you'd want this in the library anyway.
*/
LEXER_STATIC void lexer_fetch_n_push(const spans_linespan_t    *restr l,
                                           lexer_lextok_vec_t  *restr out,
                                     const lexer_lexer_state_t        stat,
                                     const size_t                     l_idx,
                                     const size_t                     start,
                                     const size_t                     len);

/*
This is a function that frees the lexer token's strspan, for the
sake of verbosity.
*/
LEXER_LIB int lexer_ltok_free(lexer_lextoken_t *restr ltok);

/*
This frees the datatype `lexer_lextok_vec_t`, which may contain several
`spans_strspans_t`, so this function will free the string spans for you,
though it doesn't free the `ltv` parameter given, so if it is heap
allocated you may free it on the callee, not here.
*/
LEXER_LIB int lexer_ltvec_free(lexer_lextok_vec_t *restr ltvec);

/*
This function is supposed to be the main tokenizer, so it should
be the only one in this header.

Why not make a lexer_tokenize_file(), you may ask? Well, it's
because BASIC is line based, and the main loop is simple enough
for you to inline yourself.

Remember to free the returned value, as it is heap allocated.
*/
LEXER_LIB lexer_lextok_vec_t lexer_tokenize_line(const spans_linespan_t *restr l,
                                                  const size_t                  l_idx);

#define tokentype_t   lexer_tokentype_t
#define lextoken_t    lexer_lextoken_t
#define lextok_vec_t  lexer_lextok_vec_t
#define lexer_state_t lexer_lexer_state_t

#define ltvec_creat()                                 lexer_ltvec_creat()
#define ltvec_push(ltv, p)                            lexer_ltvec_push(ltv, p)
#define fetch_n_push(l, out, stat, l_idx, start, len) lexer_fetch_n_push(l, out, stat, l_idx, start, len)

#define ltok_free(ltok)         lexer_ltok_free(ltok)
#define ltvec_free(ltvec)       lexer_ltvec_free(ltvec)
#define tokenize_line(l, l_idx) lexer_tokenize_line(l, l_idx)

/* --- IMPLEMENTATION --- */

#define LEXER_IMPL
#ifdef LEXER_IMPL
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#ifndef LEXER_LTVEC_INITSZ
#define LEXER_LTVEC_INITSZ 8
#endif

LEXER_STATIC lexer_lextok_vec_t lexer_ltvec_creat(void)
{
  lexer_lextoken_t *ptr = calloc(sizeof(*ptr), LEXER_LTVEC_INITSZ);
  assert(ptr && "Failed ot allocate in lexer_ltvec_creat()");

  return (lexer_lextok_vec_t) {
    .ltok_ptr = ptr,
    .len      = 0,
    .cap      = LEXER_LTVEC_INITSZ
  };
}

LEXER_STATIC int lexer_ltvec_push(lexer_lextok_vec_t *restr ltvec, const lexer_lextoken_t p)
{
  if (!ltvec) return -1;

  int rval = 0;
  if (ltvec->len >= ltvec->cap) {
    while (ltvec->len >= ltvec->cap)
      ltvec->cap *= 2;
    ltvec->ltok_ptr = realloc(ltvec->ltok_ptr, ltvec->cap);
    rval = 1;
  }

  ltvec->ltok_ptr[ltvec->len++] = p;
  return rval;
}

LEXER_STATIC int lexer_ss_to_lower(spans_strspan_t *restr ss)
{
  if (!ss) return -1;
  for (size_t i = 0; i < ss->str.len; i++)
  {
    ss->str.__s[i] = tolower(ss->str.__s[i]);
  }
  return 0;
}

LEXER_STATIC void lexer_fetch_n_push(const spans_linespan_t    *restr l,
                                           lexer_lextok_vec_t  *restr out,
                                     const lexer_lexer_state_t        stat,
                                     const size_t                     l_idx,
                                     const size_t                     start,
                                     const size_t                     len)
{
  assert(stat != LEXST_ETC && "State is an impossible state (LEXST_ETC in lexer_tokenize_line())");
  strspan_t ss = spans_ss_init(l, l_idx, start, len);
  lexer_ss_to_lower(&ss);
  lexer_tokentype_t token_type;

  switch (stat)
  {
  case LEXST_ID:
    token_type = TT_NS_ID;
    if (ss_equal_chr(&ss, "run"))
      token_type = TT_KW_RUN;
    else if (ss_equal_chr(&ss, "call"))
      token_type = TT_KW_CALL;
    else if (ss_equal_chr(&ss, "chain"))
      token_type = TT_KW_CHAIN;
    else if (ss_equal_chr(&ss, "return"))
      token_type = TT_KW_RETURN;

    else if (ss_equal_chr(&ss, "common"))
      token_type = TT_KW_COMMON;

    else if (ss_equal_chr(&ss, "declare"))
      token_type = TT_KW_DECLARE;
    else if (ss_equal_chr(&ss, "def"))
      token_type = TT_KW_DEF;

    else if (ss_equal_chr(&ss, "do"))
      token_type = TT_KW_DO;
    else if (ss_equal_chr(&ss, "to"))
      token_type = TT_KW_TO;
    else if (ss_equal_chr(&ss, "until"))
      token_type = TT_KW_UNTIL;

    else if (ss_equal_chr(&ss, "if"))
      token_type = TT_KW_IF;
    else if (ss_equal_chr(&ss, "else"))
      token_type = TT_KW_ELSE;
    else if (ss_equal_chr(&ss, "elseif"))
      token_type = TT_KW_ELSEIF;
    else if (ss_equal_chr(&ss, "then"))
      token_type = TT_KW_THEN;

    else if (ss_equal_chr(&ss, "case"))
      token_type = TT_KW_CASE;

    else if (ss_equal_chr(&ss, "end"))
      token_type = TT_KW_END;
    else if (ss_equal_chr(&ss, "enddef"))
      token_type = TT_KW_ENDDEF;

    else if (ss_equal_chr(&ss, "exit"))
      token_type = TT_KW_EXIT;

    else if (ss_equal_chr(&ss, "for"))
      token_type = TT_KW_FOR;
    else if (ss_equal_chr(&ss, "while"))
      token_type = TT_KW_WHILE;
    else if (ss_equal_chr(&ss, "loop"))
      token_type = TT_KW_LOOP;

    else if (ss_equal_chr(&ss, "next"))
      token_type = TT_KW_NEXT;
    else if (ss_equal_chr(&ss, "stop"))
      token_type = TT_KW_STOP;

    else if (ss_equal_chr(&ss, "gosub"))
      token_type = TT_KW_GOSUB;
    else if (ss_equal_chr(&ss, "goto"))
      token_type = TT_KW_GOTO;

    else if (ss_equal_chr(&ss, "rem"))
      token_type = TT_KW_REM;

    else if (ss_equal_chr(&ss, "select"))
      token_type = TT_KW_SELECT;

    else if (ss_equal_chr(&ss, "sub"))
      token_type = TT_KW_SUB;
    else if (ss_equal_chr(&ss, "wend"))
      token_type = TT_KW_WEND;

    else if (ss_equal_chr(&ss, "beep"))
      token_type = TT_KW_BEEP;
    else if (ss_equal_chr(&ss, "play"))
      token_type = TT_KW_PLAY;
    else if (ss_equal_chr(&ss, "sound"))
      token_type = TT_KW_SOUND;

    else if (ss_equal_chr(&ss, "clear"))
      token_type = TT_KW_CLEAR;
    else if (ss_equal_chr(&ss, "cls"))
      token_type = TT_KW_CLS;

    else if (ss_equal_chr(&ss, "close"))
      token_type = TT_KW_CLOSE;
    else if (ss_equal_chr(&ss, "open"))
      token_type = TT_KW_OPEN;

    else if (ss_equal_chr(&ss, "color"))
      token_type = TT_KW_COLOR;
    else if (ss_equal_chr(&ss, "paint"))
      token_type = TT_KW_PAINT;
    else if (ss_equal_chr(&ss, "pallete"))
      token_type = TT_KW_PALLETE;
    else if (ss_equal_chr(&ss, "pmap"))
      token_type = TT_KW_PMAP;
    else if (ss_equal_chr(&ss, "preset"))
      token_type = TT_KW_PRESET;
    else if (ss_equal_chr(&ss, "pset"))
      token_type = TT_KW_PSET;

    else if (ss_equal_chr(&ss, "draw"))
      token_type = TT_KW_DRAW;
    else if (ss_equal_chr(&ss, "line"))
      token_type = TT_KW_LINE;

    else if (ss_equal_chr(&ss, "screen"))
      token_type = TT_KW_SCREEN;
    else if (ss_equal_chr(&ss, "view"))
      token_type = TT_KW_VIEW;
    else if (ss_equal_chr(&ss, "window"))
      token_type = TT_KW_WINDOW;
    else if (ss_equal_chr(&ss, "width"))
      token_type = TT_KW_WIDTH;

    else if (ss_equal_chr(&ss, "com"))
      token_type = TT_KW_COM;
    else if (ss_equal_chr(&ss, "data"))
      token_type = TT_KW_DATA;
    else if (ss_equal_chr(&ss, "error"))
      token_type = TT_KW_ERROR;

    else if (ss_equal_chr(&ss, "field"))
      token_type = TT_KW_FIELD;
    else if (ss_equal_chr(&ss, "locate"))
      token_type = TT_KW_LOCATE;

    else if (ss_equal_chr(&ss, "get"))
      token_type = TT_KW_GET;
    else if (ss_equal_chr(&ss, "input"))
      token_type = TT_KW_INPUT;
    else if (ss_equal_chr(&ss, "key"))
      token_type = TT_KW_KEY;

    else if (ss_equal_chr(&ss, "lprint"))
      token_type = TT_KW_LPRINT;

    else if (ss_equal_chr(&ss, "out"))
      token_type = TT_KW_OUT;

    else if (ss_equal_chr(&ss, "print"))
      token_type = TT_KW_PRINT;
    else if (ss_equal_chr(&ss, "put"))
      token_type = TT_KW_PUT;

    else if (ss_equal_chr(&ss, "randomize"))
      token_type = TT_KW_RANDOMIZE;

    else if (ss_equal_chr(&ss, "read"))
      token_type = TT_KW_READ;
    else if (ss_equal_chr(&ss, "write"))
      token_type = TT_KW_WRITE;

    else if (ss_equal_chr(&ss, "restore"))
      token_type = TT_KW_RESTORE;

    else if (ss_equal_chr(&ss, "sleep"))
      token_type = TT_KW_SLEEP;

    else if (ss_equal_chr(&ss, "as"))
      token_type = TT_KW_AS;
    else if (ss_equal_chr(&ss, "const"))
      token_type = TT_KW_CONST;
    else if (ss_equal_chr(&ss, "static"))
      token_type = TT_KW_STATIC;
    else if (ss_equal_chr(&ss, "shared"))
      token_type = TT_KW_SHARED;

    else if (ss_equal_chr(&ss, "erase"))
      token_type = TT_KW_ERASE;

    else if (ss_equal_chr(&ss, "dim"))
      token_type = TT_KW_DIM;
    else if (ss_equal_chr(&ss, "redim"))
      token_type = TT_KW_REDIM;

    else if (ss_equal_chr(&ss, "type"))
      token_type = TT_KW_TYPE;

    else if (ss_equal_chr(&ss, "defdbl"))
      token_type = TT_KW_DEFDBL;
    else if (ss_equal_chr(&ss, "defint"))
      token_type = TT_KW_DEFINT;
    else if (ss_equal_chr(&ss, "deflng"))
      token_type = TT_KW_DEFLNG;
    else if (ss_equal_chr(&ss, "defsng"))
      token_type = TT_KW_DEFSNG;
    else if (ss_equal_chr(&ss, "defstr"))
      token_type = TT_KW_DEFSTR;

    else if (ss_equal_chr(&ss, "cdbl"))
      token_type = TT_KW_CDBL;
    else if (ss_equal_chr(&ss, "cint"))
      token_type = TT_KW_CINT;
    else if (ss_equal_chr(&ss, "clng"))
      token_type = TT_KW_CLNG;
    else if (ss_equal_chr(&ss, "cos"))
      token_type = TT_KW_COS;
    else if (ss_equal_chr(&ss, "csng"))
      token_type = TT_KW_CSNG;
    else if (ss_equal_chr(&ss, "csrlin"))
      token_type = TT_KW_CSRLIN;

    else if (ss_equal_chr(&ss, "abs"))
      token_type = TT_KW_ABS;
    else if (ss_equal_chr(&ss, "atn"))
      token_type = TT_KW_ATN;
    else if (ss_equal_chr(&ss, "exp"))
      token_type = TT_KW_EXP;
    else if (ss_equal_chr(&ss, "fix"))
      token_type = TT_KW_FIX;
    else if (ss_equal_chr(&ss, "int"))
      token_type = TT_KW_INT;
    else if (ss_equal_chr(&ss, "log"))
      token_type = TT_KW_LOG;
    else if (ss_equal_chr(&ss, "sgn"))
      token_type = TT_KW_SGN;
    else if (ss_equal_chr(&ss, "sin"))
      token_type = TT_KW_SIN;
    else if (ss_equal_chr(&ss, "sqr"))
      token_type = TT_KW_SQR;
    else if (ss_equal_chr(&ss, "tan"))
      token_type = TT_KW_TAN;

    else if (ss_equal_chr(&ss, "asc"))
      token_type = TT_KW_ASC;
    else if (ss_equal_chr(&ss, "hex"))
      token_type = TT_KW_HEX;
    else if (ss_equal_chr(&ss, "oct"))
      token_type = TT_KW_OCT;

    else if (ss_equal_chr(&ss, "inkey"))
      token_type = TT_KW_INKEY;
    else if (ss_equal_chr(&ss, "instr"))
      token_type = TT_KW_INSTR;

    else if (ss_equal_chr(&ss, "lcase"))
      token_type = TT_KW_LCASE;
    else if (ss_equal_chr(&ss, "ucase"))
      token_type = TT_KW_UCASE;

    else if (ss_equal_chr(&ss, "left"))
      token_type = TT_KW_LEFT;
    else if (ss_equal_chr(&ss, "mid"))
      token_type = TT_KW_MID;
    else if (ss_equal_chr(&ss, "right"))
      token_type = TT_KW_RIGHT;

    else if (ss_equal_chr(&ss, "ltrim"))
      token_type = TT_KW_LTRIM;
    else if (ss_equal_chr(&ss, "rtrim"))
      token_type = TT_KW_RTRIM;

    else if (ss_equal_chr(&ss, "space"))
      token_type = TT_KW_SPACE;

    else if (ss_equal_chr(&ss, "str"))
      token_type = TT_KW_STR;
    else if (ss_equal_chr(&ss, "string"))
      token_type = TT_KW_STRING;

    else if (ss_equal_chr(&ss, "val"))
      token_type = TT_KW_VAL;

    else if (ss_equal_chr(&ss, "eof"))
      token_type = TT_KW_EOF;
    else if (ss_equal_chr(&ss, "erl"))
      token_type = TT_KW_ERL;
    else if (ss_equal_chr(&ss, "err"))
      token_type = TT_KW_ERR;
    else if (ss_equal_chr(&ss, "fre"))
      token_type = TT_KW_FRE;
    else if (ss_equal_chr(&ss, "inp"))
      token_type = TT_KW_INP;
    else if (ss_equal_chr(&ss, "lof"))
      token_type = TT_KW_LOF;
    else if (ss_equal_chr(&ss, "pos"))
      token_type = TT_KW_POS;

    else if (ss_equal_chr(&ss, "point"))
      token_type = TT_KW_POINT;
    else if (ss_equal_chr(&ss, "peek"))
      token_type = TT_KW_PEEK;

    else if (ss_equal_chr(&ss, "varptr"))
      token_type = TT_KW_VARPTR;
    else if (ss_equal_chr(&ss, "varseg"))
      token_type = TT_KW_VARSEG;

    else if (ss_equal_chr(&ss, "time"))
      token_type = TT_KW_TIME;
    else if (ss_equal_chr(&ss, "date"))
      token_type = TT_KW_DATE;

    else if (ss_equal_chr(&ss, "and"))
      token_type = TT_KW_AND;
    else if (ss_equal_chr(&ss, "eqv"))
      token_type = TT_KW_EQV;
    else if (ss_equal_chr(&ss, "imp"))
      token_type = TT_KW_IMP;
    else if (ss_equal_chr(&ss, "mod"))
      token_type = TT_KW_MOD;
    else if (ss_equal_chr(&ss, "not"))
      token_type = TT_KW_NOT;
    else if (ss_equal_chr(&ss, "or"))
      token_type = TT_KW_OR;
    else if (ss_equal_chr(&ss, "xor"))
      token_type = TT_KW_XOR;

    else if (ss_equal_chr(&ss, "circle"))
      token_type = TT_KW_CIRCLE;

    else if (ss_equal_chr(&ss, "chdir"))
      token_type = TT_KW_CHDIR;
    else if (ss_equal_chr(&ss, "mkdir"))
      token_type = TT_KW_MKDIR;
    else if (ss_equal_chr(&ss, "rmdir"))
      token_type = TT_KW_RMDIR;

    else if (ss_equal_chr(&ss, "files"))
      token_type = TT_KW_FILES;
    else if (ss_equal_chr(&ss, "name"))
      token_type = TT_KW_NAME;

    else if (ss_equal_chr(&ss, "kill"))
      token_type = TT_KW_KILL;

    else if (ss_equal_chr(&ss, "shell"))
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

  const lexer_lextoken_t ltok = (lexer_lextoken_t) {
    .type = token_type,
    .span = ss,
  };
  lexer_ltvec_push(out, ltok);
}

LEXER_LIB int lexer_ltok_free(lexer_lextoken_t *restr ltok)
{
  if (!ltok)
    return -1;

  spans_ss_free(&ltok->span);

  return 0;
}

LEXER_LIB int lexer_ltvec_free(lexer_lextok_vec_t *restr ltvec)
{
  if (!ltvec) return -2;
  int rval = 0;
  for (size_t i = 0; i < ltvec->len; i++)
  {
    if (lexer_ltok_free(&ltvec->ltok_ptr[i]) != 0) rval++;
  }
  free(ltvec->ltok_ptr);
  return 0;
}

LEXER_LIB lexer_lextok_vec_t lexer_tokenize_line(const spans_linespan_t *restr l,
                                                 const size_t                  l_idx)
{
  lexer_lextok_vec_t   out   = lexer_ltvec_creat();
  lexer_lexer_state_t  stat  = LEXST_ETC;
  long                 start = -1;
  size_t               len   = 0;

  for (size_t i = 0; i < l->str.len; i++)
  {
    const char c = l->str.__s[i];

    if (start == -1) start = i;
    len++;

    if (isalpha(c))
    {
      switch (stat)
      {
      case LEXST_NUM:
        stat = LEXST_ALNUM;
        /* Fallthrough*/
      case LEXST_ALNUM:
        /* Fallthrough */
      case LEXST_ID:
        break;

      case LEXST_SPACE:
        lexer_fetch_n_push(l, &out, stat, l_idx, start, len);
        len   = 1;
        start = i;
        /* Fallthrough*/
      case LEXST_ETC:
        stat  = LEXST_ID;
        break;

      default:
        assert(0 && "Invalid state in lexer_tokenize_line() (1)");
      }
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
        lexer_fetch_n_push(l, &out, stat, l_idx, start, len);
        len   = 1;
        start = i;
        /* Fallthrough */
      case LEXST_ETC:
        stat = LEXST_NUM;
        break;

      default:
        assert(0 && "Invalid state in lexer-tokenize_line() (2)");
      }
      continue;
    } else if (c == ' ' && stat == LEXST_SPACE) continue;

    if (len > 1)
    {
      lexer_fetch_n_push(l, &out, stat, l_idx, start, len - 1);
      len   = 0;
      start = i;
    } else if (c == '\n')
    {
      if (len - 1 != 0) lexer_fetch_n_push(l, &out, stat, l_idx, start, len - 1);
      len   = 1;
      start = i;
      const spans_strspan_t ss = spans_ss_init(l, l_idx, start, len);
      lexer_ltvec_push(&out, (lexer_lextoken_t) {
        .span = ss,
        .type = TT_NEWLINE,
      });
      break;
    }

    lexer_tokentype_t token_type = TT_NS_ID;
    switch (c)
    {
    case ' ':
      stat = LEXST_SPACE;
      continue;

    case '=':
      token_type = TT_EQUAL;
      break;

    case '+':
      token_type = TT_PLUS;
      break;

    case '-':
      token_type = TT_MINUS;
      break;

    case '*':
      token_type = TT_STAR;
      break;

    case '/':
      token_type = TT_SLASH;
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
      token_type = TT_DOUB_QUOT;
      break;

    case ';':
      token_type = TT_SEMICOLON;
      break;

    case ':':
      token_type = TT_COLON;
      break;

    case '&':
      token_type = TT_AMPERSAND;
      break;

    case '?':
      token_type = TT_QUEST;
      break;

    case '<':
      token_type = TT_L_ANG_BRACK;
      break;

    case '>':
      token_type = TT_R_ANG_BRACK;
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
    const lexer_lextoken_t ltok = (lexer_lextoken_t) {
      .span = ss,
      .type = token_type,
    };

    lexer_ltvec_push(&out, ltok);

    start = -1;
    len   =  0;
  }

  if (len > 0) lexer_fetch_n_push(l, &out, stat, l_idx, start, len);

  return out;
}

#endif /* LEXER_IMPL */

#ifdef __cplusplus
}
#endif

#endif /* LEXER_DEF */

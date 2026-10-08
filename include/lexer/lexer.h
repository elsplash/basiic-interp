/* lexer.h
This header provides the functions for lexing a file, or line.
The lexing process is aligned with BASIIC.
*/

#ifndef LEXER_DEF
#define LEXER_DEF

/* NOTE ABOUT PREFIXES:
I prefer the prefixes be used anyway, just for
the sake of consistency.

This is to make sure the compiler enforces the
function/struct/enum replacing, and not us.
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

#include "spans.h"

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

typedef enum {
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

  TT_AND,
  TT_QUEST,
  
  TT_L_ANG_BRACK,
  TT_R_ANG_BRACK,

  TT_BACKSLASH,
  TT_AT,
  TT_UNDER,

  TT_ID,
  TT_NUM,
  TT_ALNUM,
} lexer_tokentype_t;

typedef struct {
  lexer_tokentype_t type;
  spans_strspan_t   span;
} lexer_lextoken_t;

typedef struct {
  lexer_lextoken_t * ltok_ptr;
  unsigned int       len;
  unsigned int       cap;
} lexer_lextok_vec_t;

typedef enum {
  LEXST_ID,
  LEXST_NUM,
  LEXST_ALNUM,
  LEXST_ETC,
} lexer_lexer_state_t;

#define tokentype_t   lexer_tokentype_t
#define lextoken_t    lexer_lextoken_t
#define lextok_vec_t  lexer_lextok_vec_t
#define lexer_state_t lexer_lexer_state_t

/* --- IMPLEMENTATION --- */

#ifdef LEXER_IMPL
#include <ctype.h>

LEXER_STATIC int lexer_ltv_push(lexer_lextok_vec_t * ltv, const lexer_lextoken_t p)
{
  if (!ltv) return -1;

  int rval = 0;
  if (ltv->len >= ltv->cap) {
    while (ltv->len >= ltv->cap)
      ltv->cap *= 2;
    ltv->ltok_ptr = realloc(ltv->ltok_ptr, ltv->cap);
    rval = 1;
  }

  ltv->ltok_ptr[ltv->len++] = p;
  return rval;
}

LEXER_STATIC void lexer_fetch_n_push(const spans_linespan_t    *restr l,
                                     const lexer_lextok_vec_t  *      out,
                                     const lexer_lexer_state_t        stat,
                                     const unsigned long              l_idx,
                                     const unsigned long              start,
                                     const unsigned long              len)
{
  assert(stat != LEXST_ETC && "State is an impossible state (LEXST_ETC in lexer_tokenize_line())");
  const strspan_t ss = spans_ss_init(l, l_idx, start, len);
  lexer_tokentype_t token_type;

  switch (stat)
  {
  case LEXST_ID:
    token_type = TT_ID;
    break;

  case LEXST_NUM:
    token_type = TT_NUM;
    break;

  case LEXST_ALNUM:
    token_type = TT_ALNUM;

  default:
    break;
  }

  const lexer_lextoken_t ltok = (lexer_lextoken_t) {
    .type = token_type,
    .span = ss,
  };
  lexer_ltv_push(out, ltok);
}

LEXER_LIB lexer_lextok_vec_t* lexer_tokenize_line(const spans_linespan_t *restr l,
                                                  const unsigned long           l_idx)
{
  unsigned long        start = 0;
  unsigned long        len   = 0;
  lexer_lextok_vec_t * out   = calloc(1, sizeof(*out));
  lexer_lexer_state_t  stat  = LEXST_ETC;

  for (unsigned long i = 0; i < l->str.len; i++)
  {
    const char c = l->str.__s[i];
    if (start == 0) start = i + 1;
    len++;

    if ((isalpha(c) && stat != LEXST_NUM)
        ||
        (isdigit(c) && stat != LEXST_ID))
    {
      if (stat == LEXST_ETC) stat = LEXST_ID;
      continue;
    }

    if (len >= 2)
    {
      lexer_fetch_n_push(l, out, stat, l_idx, start, len);
      len = start = 0;
    } if (c == '\n')
    {
      if (len != 0) lexer_fetch_n_push(l, out, stat, l_idx, start, len);
      break;
    }

    lexer_tokentype_t token_type = TT_ID;
    switch (c)
    {
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

    case '^';
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

    case '$';
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
      token_type = TT_DOT;
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
      token_type = TT_AND;
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
    const lexer_lextoken ltok = (lexer_lextoken) {
      .span = ss,
      .type = token_type,
    };
  }

  return out;
}

// lex_file

#endif /* LEXER_IMPL */

/*
#ifdef __cplusplus
}
#endif
*/

#endif /* LEXER_DEF */

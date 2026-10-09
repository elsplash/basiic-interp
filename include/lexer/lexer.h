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

#include "spans.h"

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

/* lexer_tokentype_t
This enum is based off the qBasic documentation of every valid
character by their interpreter (I believe).

Though with some tokens added in for other things not included
there, like the TT_ID for identifiers, TT_NUM for numbers, and
TT_ALNUM for alphanumeric labels.
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
  TT_NEWLINE,
  TT_AT,
  TT_UNDER,

  TT_ID,
  TT_NUM,
  TT_ALNUM,
} lexer_tokentype_t;

/* lexer_lextoken_t
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

/* lexer_lextok_vec_t
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
  LEXST_ETC,
} lexer_lexer_state_t;

/* lexer_ltv_push
This function is used to push to the lexer token vector
(lexer_lextoken_vec_t).

The reason why it's static, and not part of the library is
that it's not supposed to give function mutators to the vector,
and only view it.
*/
LEXER_STATIC int lexer_ltv_push(lexer_lextok_vec_t *restr ltv, const lexer_lextoken_t p);

/* lexer_fetch_n_push
Thi fetches the token type from lexer_lexer_state_t, and also
fetches spans_strspan_t from the parameters given. This is repeated
twice so it is coupled.

This is supposed to be a helper function for lexer_tokenize_line,
so I don't think you'd want this in the library anyway.
*/
LEXER_STATIC void lexer_fetch_n_push(const spans_linespan_t    *restr l,
                                           lexer_lextok_vec_t  *restr out,
                                     const lexer_lexer_state_t        stat,
                                     const unsigned long              l_idx,
                                     const unsigned long              start,
                                     const unsigned long              len);

/* lexer_tokenize_line
This function is supposed to be the main tokenizer, so it should
be the only one in this header.

Why not make a lexer_tokenize_file(), you may ask? Well, it's
because BASIC is line based, and the main loop is simple enough
for you to inline yourself.

Remember to free the returned value, as it is heap allocated.
*/
LEXER_LIB lexer_lextok_vec_t* lexer_tokenize_line(const spans_linespan_t *restr l,
                                                  const unsigned long           l_idx);

#define tokentype_t   lexer_tokentype_t
#define lextoken_t    lexer_lextoken_t
#define lextok_vec_t  lexer_lextok_vec_t
#define lexer_state_t lexer_lexer_state_t

/* --- IMPLEMENTATION --- */

#ifdef LEXER_IMPL
#include <ctype.h>

LEXER_STATIC int lexer_ltv_push(lexer_lextok_vec_t *restr ltv, const lexer_lextoken_t p)
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
                                           lexer_lextok_vec_t  *restr out,
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
      if (len - 1 != 0) lexer_fetch_n_push(l, out, stat, l_idx, start, len - 1);
      len = 1;
      start = i;
      const spans_strspan_t ss = spans_ss_init(l, l_idx, start, len);
      lexer_ltv_push(out, (lexer_lextoken_t) {
          .span = ss,
          .type = TT_NEWLINE,
        });
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
    const lexer_lextoken_t ltok = (lexer_lextoken_t) {
      .span = ss,
      .type = token_type,
    };
  }

  return out;
}

#endif /* LEXER_IMPL */

/*
#ifdef __cplusplus
}
#endif
*/

#endif /* LEXER_DEF */

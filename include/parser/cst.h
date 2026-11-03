/* cst.h
COPYRIGHT (C) 2026 ELSPLASH (electrsplash@gmail.com)
----------------------------------------------------

A simple CST Parsing header library.

This will parse the tokens from "lexer/lexer.h".
So it can be parsed into an AST later on.

-------------------------------------------------
Licensed under the GNU General Public License V3.
*/

#ifndef CST_DEF
#define CST_DEF


#if defined(CST_IMPL) && !defined(LEXER_IMPL)
#include "../macro.h"
#define LEXER_STATIC glob_extern

#define LEXER_IMPL
#endif
#include "lexer/blexer.h"

#ifndef CST_ASSERT

#include <assert.h>
#define CST_ASSERT(check, message) assert(check && message)

#endif

#ifndef CST_PANIC

#include <stdio.h>
#include <stdlib.h>
#define CST_PANIC(message) \
do {                       \
  puts("[CST PANIC!]");    \
  puts(message);           \
  exit(1);                 \
} while (0)

#endif

#ifndef CST_LIB
#define CST_LIB glob_extern
#endif

#ifndef CST_STATIC
#define CST_STATIC glob_static
#endif

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

/*
This is the type of a CST Node.

Which are labelled accordingly:
CNT => CST Node Type

CNT_CH => Node children
CNT_PR => Node parent

CNT_..._V => Variable
CNT_..._S => Statement
CNT_..._P => Parenthesis
CNT_..._M => Miscellanious
*/
typedef enum {
  /* Data Types */
  /* NOTE
  These datatype types must point to the character span
  in the CST Node struct.
  */
  CNT_PR_V_SHORT,
  CNT_PR_V_LONG,

  CNT_PR_V_SINGLE,
  CNT_PR_V_DOUBLE,

  CNT_PR_V_STRING,

  CNT_PR_V_IMPLICIT,

  /* Statements */
  CNT_PR_S_VAR_DEF,

  CNT_PR_S_RUN,

  /* Parenthesis */
  CNT_PR_PAREN,
  CNT_CH_L_PAREN,
  CNT_CH_COMMA,
  CNT_CH_R_PAREN,

  CNT_CH_LABEL,

  CNT_PR_APOST_COMMENT,
  CNT_PR_REM_COMMENT,

  CNT_CH_CMNT_KEY,
  CNT_CH_CMNT_CONTENT,

  CNT_PR_BIN_OP,
  CNT_PR_ERR,
}
cst_cstnode_type_t;

/*
The basic struct for the CST.

This struct is supposed to be a line-worth of nodes,
so it should be a standalone node for a line, which
is why it was a `next' element.

Though for multiple lines, you may use the vector
instead.
*/
typedef struct cst_cstnode_t
{
  spans_strspan_t        span;

  /* NOTE
  This element is used for the children of
  this CST Node, and not pointing to the
  next CST Node, that is for the cst_cnvec_t.
  */
  struct cst_cstnode_t * children;

  /* NOTE
  This element is for the nex CST Node on the
  line, since of the binary operator existing
  we must have a next pointer, instead of storing
  that next pointer at the end of the children
  pointer chain.
  */
  struct cst_cstnode_t * next;

  cst_cstnode_type_t     type;
}
cst_cstnode_t;

/* A vector for the CST Nodes we will contain. */
typedef struct
{
  struct cst_cstnode_t * ptr;
  size_t                 len;
  size_t                 cap;
}
cst_cnvec_t;

/*
Creates a CST Node vector for you, so that you can call this
function instead of doing it manually.
*/
CST_STATIC cst_cnvec_t cst_cnvec_creat(void);

/*
This function provides a way to validate a CST Node vector.

This behaves like any other vector shaped like cnvec, and also
returns like any other vector like cnvec.
*/
CST_STATIC int cst_cnvec_validate(const cst_cnvec_t *restr cnvec);

/*
Pushes a CST Node to a vector and also expands the array if
necessary, this can return an error code which are the
following:

-2 - Failed to allocate using calloc.
-1 - cnvec is NULL.
 0 - No issues.
*/
CST_STATIC int cst_cnvec_push(cst_cnvec_t *restr cnvec, const cst_cstnode_t cn);

/*
This function is a coupled function for fetching an element
while also doing bounds checking.
*/
CST_STATIC cst_cstnode_t cst_cnvec_fetch(const cst_cnvec_t *restr cnvec,
                                         const size_t             idx);

/* This checks if a lexer token is of type `tt`. */
CST_STATIC int cst_expect_ltok(const blexer_lextok_vec_t *restr ltvec,
                               const size_t                     ltv_idx,
                               const blexer_tokentype_t         tt);

/*
This function pushes a CST Node to the cn's children linked list

This function assumes that push is heap allocated so it may
survive for the cn's duration, and it will be freed by
`cst_cstnode_free()'.

There are error codes:
-2 - push is NULL.
-1 - cn is NULL.
 0 - No issues.
*/
CST_STATIC int cst_cstnode_push_children(cst_cstnode_t *restr cn,
                                         cst_cstnode_t *restr push);

/*
This function pushes a CST Node to the cn's next linked list.

Similar to `cst_cstnode_push_children', it assumes that push is
heap allocated, and to be freed by  cst_cstnode_free()'

There are error codes:
-2 - push is NULL.
-1 - cn is NULL.
 0 - No issues.
*/
CST_STATIC int cst_cstnode_push_next(cst_cstnode_t *restr cn,
                                     cst_cstnode_t *restr push);

/*
The interface function you may call if you want all the tokens
to be parsed.

The returned value may be NULL if it ran into a problem when
parsing.
*/
CST_LIB cst_cstnode_t cst_parse_cst_line(const blexer_lextok_vec_t *restr ltvec);

/*
This function will be caled if the parsed line is invalid.

This is not supposed to be reported as of the state right now
since we don't have enough structure.

This has asserts so it may exit prematurely here.
*/
CST_LIB cst_cstnode_t cst_error_cst(const blexer_lextok_vec_t *restr ltvec,
                                    const size_t                     ltstart);

/*
This function parses the first possible token that could be a label.
Which is why no start is given.

The returned value's .contents is always NULL. So you must check if
the .type is not CNT_S_LABEL instead.
*/
CST_LIB cst_cstnode_t cst_parse_label_cst(const blexer_lextok_vec_t *restr ltvec);

/* Parses parenthesises of all types, function parameters, and expressions. */
CST_LIB cst_cstnode_t cst_parse_paren_cst(const blexer_lextok_vec_t *restr ltvec,
                                                size_t              *restr ltstart);

/*
This consumes the comments and returns the CST Node for it.

This assumes that the ltvec, starts at the detected comment context
label, which are the apostrophe symbol token, or the REM keyword.
*/
CST_LIB cst_cstnode_t cst_consume_cst_comment(const blexer_lextok_vec_t *restr ltvec,
                                                    size_t              *restr ltstart);

/*
This function consumes and concatenates to a string span if the
indexed lexer token is a space.

This returns an int for verbosity, which has errors of the following:

// These return values are from blexer_ltvec_validate().
-4 - ltvec->len is more than ltvec->cap
-3 - ltvec->ptr is NULL.
-2 - ltvec is NULL.

// These come from the function itself now.
-1 - idx is out of bounds.
 0 - No issues.
*/
CST_LIB int cst_consume_cst_if_space(      blexer_lextok_vec_t *restr ltvec,
                                           spans_strspan_t     *restr ss,
                                     const size_t                     idx);

/*
This function parses the expression for the CST to fetch and put on
a node's contents
*/
CST_LIB cst_cstnode_t cst_parse_expr_cst(const blexer_lextok_vec_t *restr ltvec,
                                         const size_t              *restr ltstart);

CST_LIB void cst_cstnode_free(cst_cstnode_t *cn);

#define cstnode_type_t cst_cstnode_type_t
#define cstnode_t      cst_cstnode_t
#define cnvec_t        cst_cnvec_t

/* -- STATIC -- */
/* - Misc. - */
#define expect_ltok(ltvec, ltstart, tt) cst_expect_ltok(ltvec, ltstart, tt)

/* - CST Node - */
#define cstnode_push_children(cn, push) cst_cstnode_push_children(cn, push)
#define cstnode_push_next(cn, push)     cst_cstnode_push_next(cn, push)

/* - CST Node Vector helper functions - */
#define cnvec_push(cnvec, cn) cst_cnvec_push(cnvec, cn)
#define cnvec_creat()         cst_cnvec_creat()

/* -- Library -- */
/* - Misc. - */
#define cstnode_free(cn) cst_cstnode_free(cn)
#define error_cst(ltvec, ltstart) cst_error_cst(ltvec, ltstart)

/* - Interface - */
#define parse_cst_line(lt, ltvec) cst_parse_cst_line(lt, ltvec)

/* - Parsers - */
#define parse_label_cst(ltvec) cst_parse_label(ltvec)
#define parse_paren_cst(ltvec, ltstart) cst_parse_paren_cst(ltvec, ltstart)
#define parse_expr_cst(ltvec, ltstart)  cst_parse_expr_cst(ltvec, ltstart)

/* - Consumers - */
#define consume_cst_comment(ltvec, cnvec, ltstart) cst_consume_cst_comment(ltvec, cnvec, ltstart)

/* --- IMPLEMENTATION --- */

/* NOTE: Development leftover!! REMOVE AT RELEASE! */
#define CST_IMPL

#ifdef CST_IMPL
#include <memory.h>

#define CST_CNVEC_INITSZ 8

CST_STATIC cst_cnvec_t cst_cnvec_creat(void)
{
  cst_cstnode_t *ptr = calloc(sizeof(*ptr), CST_CNVEC_INITSZ);
  CST_ASSERT(ptr, "Failed to allocate in cst_cnvec_creat().");

  return (cst_cnvec_t) {
    .ptr = ptr,
    .len = 0,
    .cap = CST_CNVEC_INITSZ,
  };
}

CST_STATIC int cst_cnvec_validate(const cst_cnvec_t *restr cnvec)
{
  if (!cnvec)
    return 0;

  if (!cnvec->ptr)
    return -1;

  if (cnvec->len * sizeof(*cnvec->ptr) > cnvec->cap)
    return -2;

  return 1;
}

CST_STATIC int cst_cnvec_push(      cst_cnvec_t   *restr cnvec,
                              const cst_cstnode_t        cn)
{
  if (!cnvec) return -1;

  if ((cnvec->len + 1) * sizeof(*cnvec->ptr) > cnvec->cap)
  {
    cnvec->cap *= 2;
    cnvec->ptr = realloc(cnvec->ptr, cnvec->cap);
    if (!cnvec->ptr) return -2;
  }

  cnvec->ptr[cnvec->len] = cn;
  cnvec->len++;

  return 0;
}

CST_STATIC cst_cstnode_t cst_cnvec_fetch(const cst_cnvec_t *restr cnvec,
                                         const size_t             idx)
{
  CST_ASSERT(cst_cnvec_validate(cnvec), "cnvec is not valid in cst_cnvec_fetch().");
  CST_ASSERT(idx >= cnvec->len, "idx is out of bounds in cst_cnvec_fetch().");

  return cnvec->ptr[idx];
}

CST_STATIC int cst_expect_ltok(const blexer_lextok_vec_t *restr ltvec,
                               const size_t                     ltv_idx,
                               const blexer_tokentype_t         tt)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is not valid in cst_expect_ltok().");

  return ltvec->ptr[ltv_idx].type == tt;
}

CST_STATIC int cst_cstnode_push_children(cst_cstnode_t *restr cn,
                                         cst_cstnode_t *restr push)
{
  if (!cn)
    return -1;

  if (!push)
    return -2;

  cst_cstnode_t *ptr = cn;

  while (ptr->children)
    ptr = ptr->children;

  ptr->children = push;

  return 0;
}

CST_STATIC int cst_cstnode_push_next(cst_cstnode_t *restr cn,
                                     cst_cstnode_t *restr push)
{
  if (!cn)
    return -1;

  if (!push)
    return -2;

  cst_cstnode_t *ptr = cn;

  while (ptr->next)
    ptr = ptr->next;

  ptr->next = push;

  return 0;
}

CST_LIB cst_cstnode_t cst_parse_cst_line(const blexer_lextok_vec_t *restr ltvec)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is not valid in cst_parse_cst_line().");

  cst_cstnode_t out = {0};
  size_t cur_idx = 0;

  const cst_cstnode_t label = cst_parse_label_cst(ltvec);
  if (label.type == CNT_CH_LABEL)
  {
    if (ltvec->ptr[1].type == TT_COLON)
      cur_idx = 2;
    else
      cur_idx = 1;
  }

  spans_strspan_t ss = ltvec->ptr[cur_idx].span;
  cur_idx++;

  switch (ss.type)
  {
  case TT_SING_QUOT:
    return cst_consume_cst_comment(ltvec, &cur_idx);

  case TT_NS_ID:
  {
    cst_cstnode_type_t ctype;
    switch (ltvec->ptr[cur_idx].type)
    {
    case TT_PERCENT:
      ctype = CNT_PR_V_SHORT;
      break;

    case TT_AMPERSAND:
      ctype = CNT_PR_V_LONG;
      break;

    case TT_EXCL:
      ctype = CNT_PR_V_SINGLE;
      break;

    case TT_POUND:
      ctype = CNT_PR_V_DOUBLE;
      break;

    case TT_DOLLAR:
      ctype = CNT_PR_V_STRING;
      break;

    default:
      goto Error;
    }

    if (cst_expect_ltok(ltvec, cur_idx, TT_SPACE))
      goto Error;

    spans_ss_concat(&ss, &ltvec->ptr[cur_idx].span);
    cur_idx++;

    if (cst_expect_ltok(ltvec, cur_idx, TT_EQUAL))
      goto Error;

    spans_ss_concat(&ss, &ltvec->ptr[cur_idx].span);
    cur_idx++;

    if (cst_expect_ltok(ltvec, cur_idx, TT_SPACE))
      goto Error;

    spans_ss_concat(&ss, &ltvec->ptr[cur_idx].span);
    cur_idx++;

    cst_cstnode_t expr = cst_parse_expr_cst(ltvec, &cur_idx);
    spans_ss_concat(&ss, &expr.span);

    cst_cstnode_t cn = (cst_cstnode_t) {
      .type     = ctype,
      .span     = ss,
      .children = NULL,
    };

    cst_cstnode_push_next(&cn, &expr);

    cst_consume_cst_if_space();

    return cn;
  }

  case TT_KW_RUN:
    if (!cst_expect_ltok(ltvec, cur_idx, TT_SPACE)) goto Error;
    ss_concat(&ss, &ltvec->ptr[cur_idx].span);
    cur_idx++;

    if (!cst_expect_ltok(ltvec, cur_idx, TT_NS_STRING) &&
        !cst_expect_ltok(ltvec, cur_idx, TT_NS_NUM))
      goto Error;
    ss_concat(&ss, &ltvec->ptr[cur_idx].span);

  {
    cst_cstnode_t expr = cst_parse_expr_cst(ltvec, &cur_idx);
    cst_cstnode_t *alloc_expr = malloc(sizeof(*alloc_expr));
    if (!alloc_expr)
    {
      cst_cstnode_free(&expr);
      goto Error;
    }

    memcpy(alloc_expr, &expr, sizeof(expr));

    cur_idx++;

    cst_cstnode_t cn = (cst_cstnode_t) {
      .type     = CNT_S_RUN,
      .span     = ss,
      .children = alloc_expr,
    };

    cst_cnvec_push(cnvec, cn);
  }

    if (!cst_consume_cst_comment(ltvec, cnvec, &cur_idx))
      goto Error;

    break;

  case TT_KW_CALL:
    if (!cst_expect_ltok(ltvec, cur_idx, TT_SPACE))
      goto Error;
    cur_idx++;
    if (!cst_expect_ltok(ltvec, cur_idx, TT_NS_ALNUM) ||
        !cst_expect_ltok(ltvec, cur_idx, TT_NS_ID))
      goto Error;
    cur_idx++;

    cst_parse_paren_cst(ltvec, &cur_idx);

    if (!cst_consume_cst_comment(ltvec, cnvec, &cur_idx))
      goto Error;

    break;

  case TT_KW_CHAIN:
    if (!cst_expect_ltok(ltvec, cur_idx, TT_SPACE))
      goto Error;
    cur_idx++;

    if (!cst_expect_ltok(ltvec, cur_idx, TT_NS_STRING))
      goto Error;
    cur_idx++;

    /* ... */

    if (!cst_consume_cst_comment(ltvec, cnvec, &cur_idx))
      goto Error;

    break;

  case TT_KW_RETURN:
    break;

  case TT_KW_COMMON:
    break;

  case TT_KW_DECLARE:
    break;
  case TT_KW_DEF:
    break;

  case TT_KW_DO:
    break;
  case TT_KW_TO:
    break;
  case TT_KW_UNTIL:
    break;

  case TT_KW_IF:
    break;
  case TT_KW_ELSE:
    break;
  case TT_KW_ELSEIF:
    break;
  case TT_KW_THEN:
    break;

  case TT_KW_CASE:
    break;

  case TT_KW_END:
    break;
  case TT_KW_ENDDEF:
    break;

  case TT_KW_EXIT:
    break;

  case TT_KW_FOR:
    break;
  case TT_KW_WHILE:
    break;
  case TT_KW_LOOP:
    break;

  case TT_KW_NEXT:
    break;
  case TT_KW_STOP:
    break;

  case TT_KW_GOSUB:
    break;
  case TT_KW_GOTO:
    break;

  case TT_KW_REM:
    break;

  case TT_KW_SELECT:
    break;

  case TT_KW_SUB:
    break;
  case TT_KW_WEND:
    break;

  case TT_KW_BEEP:
    break;
  case TT_KW_PLAY:
    break;
  case TT_KW_SOUND:
    break;

  case TT_KW_CLEAR:
    break;
  case TT_KW_CLS:
    break;

  case TT_KW_CLOSE:
    break;
  case TT_KW_OPEN:
    break;

  case TT_KW_COLOR:
    break;
  case TT_KW_PAINT:
    break;
  case TT_KW_PALLETE:
    break;
  case TT_KW_PMAP:
    break;
  case TT_KW_PRESET:
    break;
  case TT_KW_PSET:
    break;

  case TT_KW_DRAW:
    break;
  case TT_KW_LINE:
    break;

  case TT_KW_SCREEN:
    break;
  case TT_KW_VIEW:
    break;
  case TT_KW_WINDOW:
    break;
  case TT_KW_WIDTH:
    break;

  case TT_KW_COM:
    break;
  case TT_KW_DATA:
    break;
  case TT_KW_ERROR:
    break;

  case TT_KW_FIELD:
    break;
  case TT_KW_LOCATE:
    break;

  case TT_KW_GET:
    break;
  case TT_KW_INPUT:
    break;
  case TT_KW_KEY:
    break;

  case TT_KW_LPRINT:
    break;

  case TT_KW_OUT:
    break;

  case TT_KW_PRINT:
    break;
  case TT_KW_PUT:
    break;

  case TT_KW_RANDOMIZE:
    break;

  case TT_KW_READ:
    break;
  case TT_KW_WRITE:
    break;

  case TT_KW_RESTORE:
    break;

  case TT_KW_SLEEP:
    break;

  case TT_KW_AS:
    break;
  case TT_KW_CONST:
    break;
  case TT_KW_STATIC:
    break;
  case TT_KW_SHARED:
    break;

  case TT_KW_ERASE:
    break;

  case TT_KW_DIM:
    break;
  case TT_KW_REDIM:
    break;

  case TT_KW_TYPE:
    break;

  case TT_KW_DEFDBL:
    break;
  case TT_KW_DEFINT:
    break;
  case TT_KW_DEFLNG:
    break;
  case TT_KW_DEFSNG:
    break;
  case TT_KW_DEFSTR:
    break;

  case TT_KW_CDBL:
    break;
  case TT_KW_CINT:
    break;
  case TT_KW_CLNG:
    break;
  case TT_KW_COS:
    break;
  case TT_KW_CSNG:
    break;
  case TT_KW_CSRLIN:
    break;

  case TT_KW_ABS:
    break;
  case TT_KW_ATN:
    break;
  case TT_KW_EXP:
    break;
  case TT_KW_FIX:
    break;
  case TT_KW_INT:
    break;
  case TT_KW_LOG:
    break;
  case TT_KW_SGN:
    break;
  case TT_KW_SIN:
    break;
  case TT_KW_SQR:
    break;
  case TT_KW_TAN:
    break;

  case TT_KW_ASC:
    break;
  case TT_KW_HEXS:
    break;
  case TT_KW_OCTS:
    break;
  case TT_KW_CHRS:
    break;

  case TT_KW_INKEYS:
    break;
  case TT_KW_INSTR:
    break;

  case TT_KW_LCASES:
    break;
  case TT_KW_UCASES:
    break;

  case TT_KW_LEFTS:
    break;
  case TT_KW_MIDS:
    break;
  case TT_KW_RIGHTS:
    break;

  case TT_KW_LTRIMS:
    break;
  case TT_KW_RTRIMS:
    break;

  case TT_KW_LEN:
    break;

  case TT_KW_SPACES:
    break;

  case TT_KW_STRS:
    break;
  case TT_KW_STRINGS:
    break;

  case TT_KW_VAL:
    break;

  case TT_KW_EOF:
    break;
  case TT_KW_ERL:
    break;
  case TT_KW_ERR:
    break;
  case TT_KW_FRE:
    break;
  case TT_KW_INP:
    break;
  case TT_KW_LOF:
    break;
  case TT_KW_POS:
    break;

  case TT_KW_POINT:
    break;
  case TT_KW_PEEK:
    break;
  case TT_KW_PEEKS:
    break;

  case TT_KW_VARPTR:
    break;
  case TT_KW_VARSEG:
    break;

  case TT_KW_TIME:
    break;
  case TT_KW_DATE:
    break;

  case TT_KW_AND:
    break;
  case TT_KW_EQV:
    break;
  case TT_KW_IMP:
    break;
  case TT_KW_MOD:
    break;
  case TT_KW_NOT:
    break;
  case TT_KW_OR:
    break;
  case TT_KW_XOR:
    break;

  case TT_KW_CIRCLE:
    break;

  case TT_KW_CHDIR:
    break;
  case TT_KW_MKDIR:
    break;
  case TT_KW_RMDIR:
    break;

  case TT_KW_FILES:
    break;
  case TT_KW_NAME:
    break;

  case TT_KW_KILL:
    break;

  case TT_KW_SHELL:
    break;

  default:
    break;
  }

Exit:
  return 0;

Error:
  spans_ss_free(&ss);

  cst_cnvec_push(cnvec, cst_error_cst(ltvec, cur_idx));

  goto Exit;
}

CST_LIB cst_cstnode_t cst_error_cst(const blexer_lextok_vec_t *restr ltvec,
                                    const size_t                     ltstart)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is not valid in cst_error_cst()");
  CST_ASSERT(cnvec, "cnvec is NULL in cst_error_cst()");

  cst_cstnode_t cn = (cst_cstnode_t) {
    .type     = CNT_M_ERR,
    .span     = ltvec->ptr[ltstart].span,
    .children = NULL,
  };

  for (size_t i = ltstart + 1; i < ltvec->len; i++)
    spans_ss_concat(&cn.span, &ltvec->ptr[i].span);

  return cn;
}

CST_LIB cst_cstnode_t cst_parse_label(const blexer_lextok_vec_t *restr ltvec)
{
  if (ltvec->ptr[0].type == TT_NS_NUM)
  {
    return (cst_cstnode_t) {
      .type     = CNT_S_LABEL,
      .span     = ltvec->ptr[0].span,
      .children = NULL
    };
  }
  else if ((ltvec->ptr[0].type == TT_NS_ID ||
            ltvec->ptr[0].type == TT_NS_ALNUM) &&
           ltvec->ptr[1].type == TT_COLON)
  {
    spans_strspan_t ss = ltvec->ptr[0].span;
    spans_ss_concat(&ss, &ltvec->ptr[1].span);
    return (cst_cstnode_t) {
      .type     = CNT_S_LABEL,
      .span     = ss,
      .children = NULL,
    };
  }

  return (cst_cstnode_t) {
    .type     = CNT_LABEL,
    .span     = ltvec->ptr[0].span,
    .children = NULL,
  };
}

CST_LIB cst_cstnode_t cst_parse_paren_cst(const blexer_lextok_vec_t *restr ltvec,
                                                size_t              *restr ltstart)
{
  CST_ASSERT(ltvec, "ltvec is NULL in cst_parse_paren().");
  CST_ASSERT(ltstart, "ltstart is NULL in cst_parse_paren().");
  if (!cst_expect_ltok(ltvec, *ltstart, TT_R_PAREN))
    return cst_error_cst(ltvec, *ltstart);

  const size_t og_ltstart = *ltstart;

  cst_cstnode_t cn = (cst_cstnode_t) {
    .type     = CNT_P_PAREN,
    .span     = ltvec->ptr[*ltstart].span,
    .children = NULL,
  };
  
  /* Push the left side Parenthesis */
  {
    cst_cstnode_t lparen = (cst_cstnode_t) {
      .type     = CNT_P_L_PAREN,
      .span     = ltvec->ptr[*ltstart].span,
      .children = NULL,
    };
    cst_cstnode_push_children(&cn, &lparen);
  }

  for (size_t *i = ltstart + 1; *i < ltvec->len; (*i)++)
  {
    if (cst_expect_ltok(ltvec, *i, TT_SPACE))
      (*i)++;

    if (cst_expect_ltok(ltvec, *i, TT_R_PAREN)) {
      cst_cstnode_t rparen = (cst_cstnode_t) {
        .type     = CNT_P_R_PAREN,
        .span     = ltvec->ptr[*i].span,
        .children = NULL,
      };
      cst_cstnode_push_children(&cn, &rparen);
      break;
    }

    if (cst_expect_ltok(ltvec, *i, TT_COMMA))
    {
      cst_cstnode_t comma = (cst_cstnode_t) {
        .type     = CNT_P_COMMA,
        .span     = ltvec->ptr[*i].span,
        .children = NULL,
      };
      cst_cstnode_push_children(&cn, &comma);
      continue;
    }

    cst_cstnode_t expr = cst_parse_expr_cst(ltvec, i);
    cst_cstnode_push_children(&cn, &expr);
  }

  return cn;

Error:
  cst_cstnode_free(&cn);
  return cst_error_cst(ltvec, og_ltstart);
}

CST_LIB cst_cstnode_t cst_parse_bin_op_cst(const blexer_lextok_vec_t *restr ltvec,
                                                 size_t              *restr ltstart)
{
  cst_cstnode_t cn = (cst_cstnode_t) {
    .type     = CNT_M_BIN_OP,
    .span     = ltvec->ptr[*ltstart].span,
    .children = NULL,
  };

  // ...

  return cn;
}

CST_LIB cst_cstnode_t cst_parse_expr_cst(const blexer_lextok_vec_t *restr ltvec,
                                         const size_t              *restr ltstart)
{
  CST_ASSERT(ltvec, "ltvec is NULL in cst_parse_expr_cst().");
  CST_ASSERT(ltvec->len <= *ltstart, "ltstart is out of bounds in ltvec. (cst_parse_expr_cst())");

  const blexer_lextoken_t ltok = ltvec->ptr[*ltstart];

  switch (ltok.type)
  {
  case TT_NS_ALNUM:
    /* Fallthrough */
  case TT_NS_ID:
  {
    cst_cstnode_type_t cnt;

    switch (ltok.span.str.__s[ltok.span.str.len - 1])
    {
    case '!':
      cnt = CNT_PR_V_SHORT;
      break;

    case '&':
      cnt = CNT_PR_V_LONG;
      break;

    case '#':
      cnt = CNT_PR_V_SINGLE;
      break;

    case '%':
      cnt = CNT_PR_V_DOUBLE;
      break;

    case '$':
      cnt = CNT_PR_V_STRING;
      break;

    default:
      cnt = CNT_PR_V_IMPLICIT;
      break;
    }
    return (cst_cstnode_t) {
      .type     = cnt,
      .span     = ltok.span,
      .children = NULL,
    };
  }

  case TT_NS_STRING:
    return (cst_cstnode_t) {
      .type = CNT_V_STRING,
      .span = ltok.span,
      .children = NULL
    };

  case TT_NS_NUM:
  {
    cst_cstnode_type_t cnt = CNT_V_LONG;
    int dots = 0;

    for (size_t i = 0; i < ltok.span.len; i++)
    {
      if (ltok.span.str.__s[i] == '.') dots++;
    }

    if (dots == 1)
      cnt = CNT_V_DOUBLE;
    else if (dots > 1)
    {
      cst_cstnode_t cn = (cst_cstnode_t) {
        .type = CNT_M_ERR,
        .span = ltok.span,
      };
      cn.children = malloc(sizeof(*cn.children));
      const cst_cstnode_t temp = (cst_cstnode_t) {
        .type = CNT_V_DOUBLE,
        .span = ltok.span,
        .children = NULL
      };
      memcpy(cn.children, &temp, sizeof(temp));
      return cn;
    }

    return (cst_cstnode_t) {
      .type     = cnt,
      .span     = ltok.span,
      .children = NULL
    };
  }

  default:
    break;
  }

  /* Return invalid CST Node to return */
  return (cst_cstnode_t) {
    .type     = CNT_S_LABEL,
    .span     = ltok.span,
    .children = NULL,
  };
}

CST_LIB cst_cstnode_t cst_consume_cst_comment(const blexer_lextok_vec_t *restr ltvec,
                                              const size_t                     ltstart)
{
  CST_ASSERT(ltvec, "ltvec is NULL in cst_consume_cst_comment.");
  CST_ASSERT(ltstart < ltvec->len, "ltstart is out of bounds in cst_consume_cst_comment.");

  const cst_cstnode_type_t cnt;

  {
    const blexer_lextoken_t lt = blexer_ltvec_fetch(ltvec, ltstart);
    if (ltok.type == TT_SING_QUOT)
      cnt = CNT_PR_APOST_COMMENT;
    else if (ltok.type == TT_KW_REM)
      cnt = CNT_PR_REM_COMMENT;
    else 
      CST_PANIC("cst_consume_cst_comment() was called on a non comment lexer token.");
  }

  for (size_t i = ltstart + 1; i < ltvec->len; i++)
  {
    const blexer_lextoken_t lt = 
    cst_cstnode_t cn = (cst_cstnode_t) {
      .span =,
      .
    };
  }
}

CST_LIB cst_cstnode_t cst_consume_cst_if_space(      blexer_lextok_vec_t *restr ltvec,
													 spans_strspan_t     *restr ss,
                                               const size_t idx)
{
}

CST_LIB void cst_cstnode_free(cst_cstnode_t *cn)
{
  cst_cstnode_t *free_cn;
  cst_cstnode_t *cur_cn = cn;
  while (cur_cn->children)
  {
    free_cn = cur_cn;
    cur_cn = free_cn->children;
    free(free_cn);
  }
}

#endif

/*
#ifdef __cplusplus
}
#endif
*/

#endif

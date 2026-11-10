/* cst.= h
COPYRIGHT (C) 2026 ELSPLASH (electrsplash@gmail.com)
----------------------------------------------------

A simple CST Parsing header library.

This will parse the tokens from "lexer/blexer.h".
So it can be parsed into an AST later on.

-------------------------------------------------
Licensed under the GNU General Public License V3.
*/

#ifndef CST_DEF
#define CST_DEF

#define CST_IMPL

#if defined(CST_IMPL)
#include "macros.h"
#define BLEXER_STATIC glob_extern

#define BLEXER_IMPL
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
do                         \
{                          \
  puts("[CST PANIC!]");    \
  puts(message);           \
  exit(1);                 \
}                          \
while (0)

#endif

#ifndef CST_LIB
#define CST_LIB glob_extern
#endif

#ifndef CST_STATIC
#define CST_STATIC glob_static
#endif

/*
#ifdef __cplusplus
extern "C"
{
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
  /* Variables */
  /* NOTE
  These datatype types must point to the character span
  in the CST Node struct.
  */
  CNT_CH_V_SHORT,
  CNT_CH_V_LONG,

  CNT_CH_V_SINGLE,
  CNT_CH_V_DOUBLE,

  CNT_CH_V_STRING,

  CNT_CH_V_IMPLICIT,

  /* Statements */
  CNT_PR_S_VAR_DEF,

  CNT_PR_S_RUN,

  /* Parenthesis */
  CNT_PR_PAREN,

  CNT_CH_L_PAREN,
  CNT_CH_R_PAREN,

  CNT_CH_ITEM,

  CNT_CH_COMMA,

  /* Labels */
  CNT_CH_NUM_LABEL,
  CNT_CH_ALNUM_LABEL,

  /* Comments */
  CNT_PR_APOST_COMMENT,
  CNT_PR_REM_COMMENT,

  CNT_CH_CMNT_KEY,
  CNT_CH_CMNT_CONTENT,

  /* Unary Operator */
  CNT_PR_UN_OP,

  CNT_CH_INCR,
  CNT_CH_DECR,

  /* Binary Operator */
  CNT_PR_BIN_OP,

  CNT_CH_PLUS,
  CNT_CH_MINUS,
  CNT_CH_TIMES,
  CNT_CH_DIVIDE,

  CNT_CH_MODULO,

  CNT_CH_EQEQ,

  CNT_CH_LESSER,
  CNT_CH_LESSEQ,

  CNT_CH_GREATER,
  CNT_CH_GREATEQ,

  CNT_CH_NOT,

  CNT_CH_ADDEQ,
  CNT_CH_SUBEQ,
  CNT_CH_MULEQ,
  CNT_CH_DIVEQ,

  /* Misc. */
  CNT_CH_EQUAL,

  /* Error */
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
  This element is used to go to this node's children,
  which leads to it's own linked list, via it's own
  `next', variable.
  */
  struct cst_cstnode_t * child;

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

/* - The CNT helper functions -
I beileve these are already obvious enough, so I will not be
adding any docstrings to these unless necessary.
*/

CST_STATIC int cst_cnt_is_variable(const cst_cstnode_type_t cnt);

CST_STATIC cst_cstnode_type_t cst_tt_op_to_cnt(const blexer_tokentype_t tt);

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
This function creates and returns a CST Node for pushing to the
linked list of a parent CST Node.

You'd have to free this CST Node by the function that frees it's
contents and free this heap allocated variable.
*/
CST_STATIC cst_cstnode_t* cst_convert_cn_to_alloc(const cst_cstnode_t cn);

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
CST_STATIC int cst_cstnode_push_child(cst_cstnode_t *restr cn,
                                      cst_cstnode_t *restr push);

/*
This function pushes a CST Node to the cn's next linked list.

Similar to `cst_cstnode_push_children', it assumes that push is
heap allocated, and to be freed by `cst_cstnode_free()'

There are error codes:
-2 - push is NULL.
-1 - cn is NULL.
 0 - No issues.
*/
CST_STATIC int cst_cstnode_push_next(cst_cstnode_t *restr cn,
                                     cst_cstnode_t *restr push);

/*
This function will use `cst_parse_cst_line' to loop over an
extracted linespan table from a file. In which it will tokenize
these lines along the way.

This expects you to extract the linespans from a file first,
from a function given from `spans.h', which is the function
`spans_ls_fetch_file()'.
*/
CST_LIB cst_cnvec_t cst_parse_cst_file(const spans_ls_table_t *restr lt);

/*
This function will parse only a line, using the below functions.

You may call this function as it is supposed to be an interface
function.
*/
CST_LIB cst_cstnode_t cst_parse_cst_line(const blexer_lextok_vec_t *restr ltvec);

/*
This function will consume all remaining lexer tokens in the
lexer token vector.

If anything fails during a main line parsing, you may call
this function, and push the returned value to the main node's
child.
*/
CST_LIB int cst_error_cst(const blexer_lextok_vec_t *restr ltvec,
                                cst_cstnode_t       *restr cn,
                          const size_t                     ltstart);

/*
This function parses the label, the probable first to be
parsed in a line.

This is labelled either as a CNT_CH_NUM_LABEL for labels
that are only numbers, or CNT_CH_ALNUM_LABEL for alphanumeric
labels, that has a child of a colon, if that colon is missing,
it will be accompanied by an error node instead.
*/
CST_LIB cst_cstnode_t cst_parse_cst_label(const blexer_lextok_vec_t *restr ltvec,
                                                size_t              *restr cur_idx);

/*
This function will parse the parenthesis, which is an item.

This first expects that the passed `cn' variable has the
seen left parenthesis, and so will just continue parsing
the items from there.

An item consists of an expression, and a comma. There can be
an error variant, which is contained within the item.

If it is the expression it is missing, the first child
linked, will be the error, else, it is the proceeding
link that is the error.

This will end with a parenthesis.
*/
CST_LIB int cst_parse_paren_cst(const blexer_lextok_vec_t *restr ltvec,
                                      cst_cstnode_t       *restr main_node,
                                      size_t              *restr ltstart);

/*
This function will parse the preceeding unary operator, or
it's proceeeding unary operator.
*/
CST_LIB int cst_parse_un_op_cst(const blexer_lextok_vec_t *restr ltvec,
                                      cst_cstnode_t       *restr main_node,
                                      size_t              *restr ltstart);

/*
This function will parse the binary operators, this will start
in a described binary operator level, and if the detected
next operator is not an operator, or is not the same precedence
level, it will exit with a return code.

This function expects that you have checked that the upcoming
lexer token is an operator, if it isn't it will return
prematurely with an error node, or if CST_ASSERT is not disabled
it will be caught by said assert.

The precedence levels are as follows:
-1 - NOT SET, will detect automatically what the operator will
     be.

0 - HIGHEST PRECEDENCE, for example, the assignment, and
    mutate and assign operators.

1 - HIGH PRECEDENCE, these are for the comparison operators, and
the highest arithmetic precedence, like parenthesises.

2 - MID PRECEDENCE, these are arithmetic operators that take
    more precedence like the multiplication symbol.

3 - LOW PRECEDENCE, these are the rest of the arithmetic operators.

This can return an error code which are:
-2 - Non expression detected.
-1 - Detected non-operator.
 0 - No issues, and no more operators.
 1 - Encountered an operator of higher precedence.
*/
CST_LIB int cst_parse_bin_op_cst(const blexer_lextok_vec_t *restr ltvec,
                                 const int                        bin_op_lvl,
                                       cst_cstnode_t       *restr lhs,
                                       size_t              *restr ltstart);

/*
This function only parses one expression.

Meant to be used for the binary, and unary functions.
This may return an error node, as so, since this will just
attempt to parse the expression anyway.
*/
CST_LIB cst_cstnode_t cst_parse_primary_cst(const blexer_lextok_vec_t *restr ltvec,
                                                   size_t              *restr ltstart);

/*
This function will parse all possible expressions, along with
their operators.

This should be used over the other functions, like the one
expression version, the binary operator parser function, and
the unary operator parser function, as this includes all of
them.
*/
CST_LIB int cst_parse_expr_cst(const blexer_lextok_vec_t *restr ltvec,
                                              cst_cstnode_t       *restr main_node,
                                              size_t              *restr ltstart);

/*
This function will consume any remaining comments.

The function wil not account if it should try to parse the
remaining lexer tokens, as it is the caller's job to check
for it. A price for it having to return a cst_cstnode_t
*/
CST_LIB int cst_consume_cst_comment(const blexer_lextok_vec_t *restr ltvec,
                                          cst_cstnode_t       *restr main_node,
                                          size_t              *restr ltstart);

/*
This function will increment `idx', and concatenate to ss, if
the indexed lexer token on ltvec, is indeed a space.

If it isn't, it will simply return 0.
*/
CST_LIB int cst_consume_cst_if_space(const blexer_lextok_vec_t *restr ltvec,
                                           spans_strspan_t     *restr ss,
                                           size_t              *restr idx);

/*
This function will check if there are any more lexxer tokens,
and if so, it will put them into a Error CST and push it into
the main node.
*/
CST_LIB int cst_consume_remaining_cst(const blexer_lextok_vec_t *restr ltvec,
                                            cst_cstnode_t       *restr main_node,
                                      const size_t                     ltstart);

/*
This function frees the CST Node for you.

In which it goes down the linked list, and frees each and every
one of them, and the children as well}.

This does not account if `cn' is heap allocated itself, so if
you must, free it yourself.
*/
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
#define cstnode_free(cn)          cst_cstnode_free(cn)
#define error_cst(ltvec, ltstart) cst_error_cst(ltvec, ltstart)

/* - Interface - */
#define parse_cst_file(lt)    cst_parse_cst_file(lt)
#define parse_cst_line(ltvec) cst_parse_cst_line(ltvec)

/* - Parsers - */
#define parse_cst_label(ltvec)                      cst_parse_cst_label(ltvec)
#define parse_paren_cst(ltvec, ltstart)             cst_parse_paren_cst(ltvec, ltstart)
#define parse_bin_op_cst(ltvec, prev_expr, ltstart) cst_parse_bin_op_cst(ltvec, prev_expr, ltstart)
#define parse__primary_cst(ltvec, ltstart)          cst_parse__primary_cst(ltvec, ltstart)
#define parse_expr_cst(ltvec, main_node, ltstart)   cst_parse_expr_cst(ltvecm main_node, ltstart)

/* - Consumers - */
#define consume_cst_comment(ltvec, ltstart)  cst_consume_cst_comment(ltvec, ltstart)
#define consume_cst_if_space(ltvec, ltstart) cst_consume_cst_if_space(ltvec, ltstart)

/* --- IMPLEMENTATION --- */

#ifdef CST_IMPL
#include <memory.h>

#define CST_CNVEC_INITSZ 8

CST_STATIC inline int cst_cnt_is_variable(const cst_cstnode_type_t cnt)
{
  return cnt >= CNT_CH_V_SHORT || cnt <= CNT_CH_V_IMPLICIT;
}

CST_STATIC cst_cnvec_t cst_cnvec_creat(void)
{
  cst_cstnode_t *ptr = calloc(sizeof(*ptr), CST_CNVEC_INITSZ);
  if (!ptr)
    CST_PANIC("Failed to allocate in cst_cnvec_creat().");

  return (cst_cnvec_t) {
    .ptr = ptr,
    .len = 0,
    .cap = CST_CNVEC_INITSZ,
  };
}

CST_STATIC cst_cstnode_type_t cst_tt_op_to_cnt(const blexer_tokentype_t tt)
{
  if (!blexer_tt_is_op(tt))
    CST_PANIC("Token Type given is not an operator in cst_tt_op_to_cnt().");

  cst_cstnode_type_t cnt;

  switch (tt)
  {

  case TT_EQUAL:
    cnt = CNT_CH_EQUAL;
    break;

  case TT_EQEQ:
    cnt = CNT_CH_EQEQ;
    break;

  case TT_PLUS:
    cnt = CNT_CH_PLUS;
    break;

  case TT_INCR:
    cnt = CNT_CH_INCR;
    break;

  case TT_ADDEQ:
    cnt = CNT_CH_ADDEQ;
    break;

  case TT_MINUS:
    cnt = CNT_CH_MINUS;
    break;

  case TT_DECR:
    cnt = CNT_CH_DECR;
    break;

  case TT_SUBEQ:
    cnt = CNT_CH_SUBEQ;
    break;

  case TT_STAR:
    cnt = CNT_CH_TIMES;
    break;

  case TT_MULEQ:
    cnt = CNT_CH_MULEQ;
    break;

  case TT_SLASH:
    cnt = CNT_CH_DIVIDE;
    break;

  case TT_DIVEQ:
    cnt = CNT_CH_DIVEQ;
    break;

  case TT_L_ANG_BRACK:
    cnt = CNT_CH_LESSER;
    break;

  case TT_LESSEQ:
    cnt = CNT_CH_LESSEQ;
    break;

  case TT_R_ANG_BRACK:
    cnt = CNT_CH_GREATER;
    break;

  case TT_GREATEQ:
    cnt = CNT_CH_GREATEQ;
    break;

  case TT_NOT:
    cnt = CNT_CH_NOT;
    break;

  default:
    CST_PANIC("Caught a non-operator in cst_tt_op_to_cnt().");
  }

  return cnt;
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
  if (cst_cnvec_validate(cnvec))
    CST_PANIC("cnvec is not valid in cst_cnvec_fetch().");
  if (idx >= cnvec->len)
    CST_PANIC("idx is out of bounds in cst_cnvec_fetch().");

  return cnvec->ptr[idx];
}

CST_STATIC int cst_expect_ltok(const blexer_lextok_vec_t *restr ltvec,
                               const size_t                     ltv_idx,
                               const blexer_tokentype_t         tt)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is not valid in cst_expect_ltok().");
  if (!blexer_ltvec_validate(ltvec)) return 0;

  return ltvec->ptr[ltv_idx].type == tt;
}

CST_STATIC cst_cstnode_t* cst_convert_cn_to_alloc(const cst_cstnode_t cn)
{
  cst_cstnode_t *alloc_node = malloc(sizeof(*alloc_node));
  if (!alloc_node) return NULL;

  memcpy(alloc_node, &cn, sizeof(cn));

  return alloc_node;
}

CST_STATIC int cst_cstnode_push_child(cst_cstnode_t *restr cn,
                                      cst_cstnode_t *restr push)
{
  return cst_cstnode_push_next(cn->child, push);
}

CST_STATIC int cst_cstnode_push_next(cst_cstnode_t *restr cn, cst_cstnode_t *restr push)
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

CST_LIB cst_cnvec_t cst_parse_cst_file(const spans_ls_table_t *restr lt)
{
  /* Do not do this up until the very end. */
}

CST_LIB cst_cstnode_t cst_parse_cst_line(const blexer_lextok_vec_t *restr ltvec)
{
  if (blexer_ltvec_validate(ltvec))
    CST_PANIC("ltvec is not valid in cst_parse_cst_line().");

  cst_cstnode_t out = {0};
  size_t cur_idx = 0;

  {
    const cst_cstnode_t label = cst_parse_cst_label(ltvec, &cur_idx);
    if (label.type != CNT_PR_ERR)
    {
      out.span = label.span;
      cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(label);

      if (!alloc_node) CST_PANIC("Failed to allocate label node in cst_parse_cst_line().");

      cst_cstnode_push_child(&out, alloc_node);
    }
  }

  switch (ltvec->ptr[cur_idx].type)
  {
  case TT_SING_QUOT:
  {
    const int rval = cst_consume_cst_comment(ltvec, &out, &cur_idx);
    if (rval != 0) goto Error;
  }

    break;

  case TT_NS_ID:
  {
    const blexer_lextoken_t ltok = ltvec->ptr[cur_idx];

    cst_cstnode_type_t ctype;

    switch (ltok.span.str.__s[ltok.span.str.len - 1])
    {
    case '%':
      ctype = CNT_CH_V_SHORT;
      break;

    case '&':
      ctype = CNT_CH_V_LONG;
      break;

    case '!':
      ctype = CNT_CH_V_SINGLE;
      break;

    case '#':
      ctype = CNT_CH_V_DOUBLE;
      break;

    case '$':
      ctype = CNT_CH_V_STRING;
      break;

    default:
      ctype = CNT_CH_V_IMPLICIT;
      break;
    }

    if (!cst_consume_cst_if_space(ltvec, &out.span, &cur_idx))
      goto Error;

    if (cst_expect_ltok(ltvec, cur_idx, TT_EQUAL))
      goto Error;

    spans_ss_concat(&out.span, &ltvec->ptr[cur_idx].span);
    cur_idx++;

    if (!cst_consume_cst_if_space(ltvec, &out.span, &cur_idx))
      goto Error;

    {
      const int rval = cst_parse_expr_cst(ltvec, &out, &cur_idx);
      if (rval != 0) goto Error;
    }

    cst_consume_cst_if_space(ltvec, &out.span, &cur_idx);

    if (cst_expect_ltok(ltvec, cur_idx, TT_SING_QUOT))
    {
      const int rval = cst_consume_cst_comment(ltvec, &out, &cur_idx);
      if (rval != 0) goto Error;
    }
    else
      cst_consume_remaining_cst(ltvec, &out, cur_idx);
  }

    break;

  case TT_KW_RUN:
    if (!cst_consume_cst_if_space(ltvec, &out.span, &cur_idx)) goto Error;

    if (!cst_expect_ltok(ltvec, cur_idx, TT_NS_STRING) &&
        !cst_expect_ltok(ltvec, cur_idx, TT_NS_NUM))
      goto Error;

  {
    const int rval = cst_parse_expr_cst(ltvec, &out, &cur_idx);
    if (rval != 0) goto Error;

    cur_idx++;

    cst_consume_cst_if_space(ltvec, &out.span, &cur_idx);

    if (cst_expect_ltok(ltvec, cur_idx, TT_SING_QUOT) ||
        cst_expect_ltok(ltvec, cur_idx, TT_KW_REM))
    {
      const int rval = cst_consume_cst_comment(ltvec, &out, &cur_idx);
      if (rval != 0) goto Error;
    }
    else {/* There should be a function that checks if there's stil more lextokens, and errors if so. */}

  }

    break;

  case TT_KW_CALL:
    if (!cst_consume_cst_if_space(ltvec, &out.span, &cur_idx)) goto Error;

    if (!cst_expect_ltok(ltvec, cur_idx, TT_NS_ALNUM) ||
        !cst_expect_ltok(ltvec, cur_idx, TT_NS_ID))
      goto Error;

    /* ... */

    cst_consume_cst_if_space(ltvec, &out.span, &cur_idx);

    if (blexer_ltvec_fetch(ltvec, cur_idx).type == TT_SING_QUOT) {
      const int rval = cst_consume_cst_comment(ltvec, &out, &cur_idx);
      if (rval != 0) goto Error;
    }

    break;

  case TT_KW_CHAIN:
    if (!cst_expect_ltok(ltvec, cur_idx, TT_SPACE))
      goto Error;
    cur_idx++;

    if (!cst_expect_ltok(ltvec, cur_idx, TT_NS_STRING))
      goto Error;
    cur_idx++;

    /* ... */

    cst_consume_cst_if_space(ltvec, &out.span, &cur_idx);

    if (blexer_ltvec_fetch(ltvec, cur_idx).type == TT_SING_QUOT) {
      const int rval = cst_consume_cst_comment(ltvec, &out, &cur_idx);
      if (rval != 0) goto Error;
    }

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

Error:
  spans_ss_free(&out.span);

  const int err = cst_error_cst(ltvec, &out, cur_idx);
  if (!err)
    CST_PANIC("cst_error_cst() failed in cst_parse_cst_line().");

  return out;
}

CST_LIB int cst_error_cst(const blexer_lextok_vec_t *restr ltvec,
                                cst_cstnode_t       *restr main_node,
                          const size_t                     ltstart)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is not valid in cst_error_cst()");
  if (!blexer_ltvec_validate(ltvec)) return 0; 

  CST_ASSERT(main_node, "cnvec is NULL in cst_error_cst()");
  if (!main_node) return -1;

  cst_cstnode_t error = (cst_cstnode_t) {
    .type = CNT_PR_ERR,
    .span = ltvec->ptr[ltstart].span,
  };

  for (size_t i = ltstart + 1; i < ltvec->len; i++)
    spans_ss_concat(&error.span, &ltvec->ptr[i].span);

  cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(error);
  if (!alloc_node)
    CST_PANIC("Failed to allocate error node in cst_error_cst().");

  cst_cstnode_push_child(main_node, alloc_node);
  return 1;
}

CST_LIB cst_cstnode_t cst_parse_cst_label(const blexer_lextok_vec_t *restr ltvec,
                                                size_t              *restr cur_idx)
{
  if (ltvec->ptr[0].type == TT_NS_NUM)
  {
    (*cur_idx)++;
    return (cst_cstnode_t) {
      .type = CNT_CH_NUM_LABEL,
      .span = ltvec->ptr[0].span,
    };
  }
  else if ((ltvec->ptr[0].type == TT_NS_ID ||
            ltvec->ptr[0].type == TT_NS_ALNUM) &&
           ltvec->ptr[1].type == TT_COLON)
  {
    (*cur_idx) += 2;
    spans_strspan_t ss = ltvec->ptr[0].span;
    spans_ss_concat(&ss, &ltvec->ptr[1].span);
    return (cst_cstnode_t) {
      .type = CNT_CH_ALNUM_LABEL,
      .span = ss,
    };
  }

  return (cst_cstnode_t) {
    .type = CNT_PR_ERR,
    .span = ltvec->ptr[0].span,
  };
}

CST_LIB int cst_parse_paren_cst(const blexer_lextok_vec_t *restr ltvec,
                                      cst_cstnode_t       *restr cn,
                                      size_t              *restr ltstart)
{
  if (!blexer_ltvec_validate(ltvec))
    CST_PANIC("ltvec is invalid in cst_parse_paren().");

  if (!ltstart)
    CST_PANIC("ltstart is NULL in cst_parse_paren().");

  const size_t og_ltstart = *ltstart;

  int is_expect_comma = 0;

  for (size_t *i = ltstart; *i < ltvec->len; (*i)++)
  {
    if (cst_expect_ltok(ltvec, *i, TT_SPACE))
      (*i)++;

    if (cst_expect_ltok(ltvec, *i, TT_R_PAREN)) {
      cst_cstnode_t *alloc_node = malloc(sizeof(*alloc_node));
      if (!alloc_node)
        CST_PANIC("Failed to allocate Right parenthesis node in cst_parse_paren_cst().");

      alloc_node->type = CNT_CH_R_PAREN;
      alloc_node->span = ltvec->ptr[*i].span;

      cst_cstnode_push_child(cn, alloc_node);

      break;
    }

    if (cst_expect_ltok(ltvec, *i, TT_COMMA))
    {
      cst_cstnode_t item = (cst_cstnode_t) {
        .type = CNT_CH_ITEM,
      };

      cst_cstnode_t *alloc_node = malloc(sizeof(*alloc_node));
      if (!alloc_node)
        CST_PANIC("Failed to allocate comma node in cst_parse_paren_cst().");

      alloc_node->type = CNT_CH_COMMA;
      alloc_node->span = ltvec->ptr[*i].span;

      if (!is_expect_comma)
      {
        cst_cstnode_t *err = malloc(sizeof(*alloc_node));
        err->type = CNT_PR_ERR;
        err->span = alloc_node->span;
        cst_cstnode_push_child(err, alloc_node);
        cst_cstnode_push_next(&item, err);
      }

      cst_cstnode_push_child(&item, alloc_node);

      alloc_node = cst_convert_cn_to_alloc(item);
      cst_cstnode_push_next(cn, alloc_node);

      continue;
    }

    cst_cstnode_t item = {
      .type = CNT_CH_ITEM,
    };

    const int expr = cst_parse_expr_cst(ltvec, &item, i);
    if (expr != 0)
    {
      // ...
    }

    cst_cstnode_t *alloc_node;

    if (is_expect_comma)
    {
      const spans_strspan_t ss = alloc_node->span;

      alloc_node = malloc(sizeof(*alloc_node));
      if (!alloc_node)
        CST_PANIC("Failed to allocate expression node in cst_parse_paren_cst().");

      alloc_node->type = CNT_PR_ERR;
      alloc_node->span = ss;

      cst_cstnode_push_child(cn, alloc_node);
    }

    alloc_node = cst_convert_cn_to_alloc(item);
    if (!alloc_node)
      CST_PANIC("Failed to allocate item node for item in cst_parse_paren_cst().");

    cst_cstnode_push_child(cn, alloc_node);
  }

  return 1;
}

CST_LIB int cst_parse_un_op_cst(const blexer_lextok_vec_t *restr ltvec,
                                      cst_cstnode_t       *restr main_node,
                                      size_t              *restr ltstart)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is not valid in cst_parse_un_op_cst().");
  if (!blexer_ltvec_validate(ltvec))
    return -1;

  CST_ASSERT(main_node, "expr is NULL in cst_parse_un_op_cst().");
  if (!main_node)
    return -2;

  CST_ASSERT(ltstart, "ltstart is NULL in cst_parse_un_op_cst().");
  if (!ltstart)
    return -3;

  CST_ASSERT(ltvec->len <= *ltstart, "ltstart is out of bounds in ltvec in cst_parse_primary_cst().");
  if (ltvec->len > *ltstart)
    return -4;

  cst_cstnode_t cn = (cst_cstnode_t) {
    .type = CNT_PR_UN_OP,
  };
  int has_parsed_unary = 0;

  blexer_lextoken_t ltok = blexer_ltvec_fetch(ltvec, *ltstart);

 ParseUnary:
  if (blexer_tt_is_un_op(ltok.type))
  {
    cst_cstnode_t un_op = (cst_cstnode_t) {
      .span = ltok.span,
    };
    switch (ltok.type)
    {
    case TT_PLUS:
      un_op.type = CNT_CH_PLUS;
      break;

    case TT_INCR:
      un_op.type = CNT_CH_INCR;
      break;

    case TT_MINUS:
      un_op.type = CNT_CH_MINUS;
      break;

    case TT_DECR:
      un_op.type = CNT_CH_DECR;
      break;

    default:
      CST_PANIC("Unary type checker not updated in cst_parse_un_op_cst().");
    }

    cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(un_op);
    if (!alloc_node)
      CST_PANIC("Failed to allocate Unary operator node in cst_parse_un_op_cst().");

    cst_cstnode_push_child(&cn, alloc_node);

    (*ltstart)++;
    ltok = blexer_ltvec_fetch(ltvec, *ltstart);
    if (has_parsed_unary)
      return 1;
    has_parsed_unary = 1;
  }

  const int rval = cst_parse_expr_cst(ltvec, &cn, ltstart);
  if (!rval) return -3;

  cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(cn);
    if (!alloc_node)
      CST_PANIC("Failed to allocate Main Unary node in cst_parse_un_op_cst().");

  cst_cstnode_push_next(main_node, alloc_node);
  spans_ss_concat(&main_node->span, &cn.span);

  if (!has_parsed_unary)
  {
    has_parsed_unary = 1;
    goto ParseUnary;
  }

  return 1;
}

CST_LIB int cst_parse_bin_op_cst(const blexer_lextok_vec_t *restr ltvec,
                                 const int                        bin_op_lvl,
                                       cst_cstnode_t       *restr lhs,
                                       size_t              *restr ltstart)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is invalid in cst_parse_bin_op_cst().");
  if (!blexer_ltvec_validate(ltvec))
    return -1;

  CST_ASSERT(lhs, "lhs is NULL in cst_parse_bin_op_cst().");
  if (!lhs)
    return -2;

  CST_ASSERT(ltstart, "ltstart is NULL in cst_parse_bin_op_cst().");
  if (!ltstart)
    return -3;

  CST_ASSERT(ltvec->len <= *ltstart, "ltstart is out of bounds in ltvec in cst_parse_primary_cst().");
  if (ltvec->len > *ltstart)
    return -4;

  cst_consume_cst_if_space(ltvec, &lhs->span, ltstart);

  const blexer_lextoken_t op = blexer_ltvec_fetch(ltvec, *ltstart);

  if (!blexer_tt_is_bin_op(op.type))
    return -5;

  int detected_bin_op_lvl = 3;

  if (!blexer_tt_is_op_lvl_3(op.type)) detected_bin_op_lvl--;

  if (!blexer_tt_is_op_lvl_2(op.type) &&
      detected_bin_op_lvl == 2) detected_bin_op_lvl--;

  if (!blexer_tt_is_op_lvl_1(op.type) &&
      detected_bin_op_lvl == 1) detected_bin_op_lvl--;

  if (bin_op_lvl == -1)
    return cst_parse_bin_op_cst(ltvec,
                                detected_bin_op_lvl,
                                lhs,
                                ltstart);

  if (bin_op_lvl < detected_bin_op_lvl) return 1;

  const cst_cstnode_type_t cnt = cst_tt_op_to_cnt(op.type);
  cst_cstnode_t op_cn = (cst_cstnode_t) {
    .type = cnt,
    .span = op.span,
  };

  if (bin_op_lvl > detected_bin_op_lvl)
  {
    const int rval = cst_parse_bin_op_cst(ltvec, detected_bin_op_lvl, &op_cn, ltstart);
    if (rval < 0)
      return rval;
  }

  cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(op_cn);
  if (!alloc_node)
    CST_PANIC("Failed to allocate node in cst_parse_bin_op_cst().");
  cst_cstnode_push_next(lhs, alloc_node);

  (*ltstart)++;
  cst_consume_cst_if_space(ltvec, &lhs->span, ltstart);

  const int rval = cst_parse_expr_cst(ltvec, lhs, ltstart);
  if (!rval) return -6;

  size_t local_cur_idx = *ltstart;

  if (cst_expect_ltok(ltvec, local_cur_idx, TT_SPACE)) local_cur_idx++;

  if (blexer_tt_is_bin_op(blexer_ltvec_fetch(ltvec, local_cur_idx).type))
  {
    *ltstart = local_cur_idx;
    return cst_parse_bin_op_cst(ltvec,
                                bin_op_lvl,
                                lhs,
                                ltstart);
  }

  return 0;
}

CST_LIB cst_cstnode_t cst_parse_primary_cst(const blexer_lextok_vec_t *restr ltvec,
                                                   size_t              *restr ltstart)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is invalid in cst_parse_primary_cst().");
  CST_ASSERT(ltstart, "ltstart is NULL in cst_parse_primary_cst().");
  CST_ASSERT(ltvec->len <= *ltstart, "ltstart is out of bounds in ltvec in cst_parse_primary_cst().");

  const blexer_lextoken_t ltok = ltvec->ptr[*ltstart];

  switch (ltok.type)
  {
  case TT_NS_ALNUM:
    /* Fallthrough */
  case TT_NS_ID:
    if (blexer_tt_is_un_op(blexer_ltvec_fetch(ltvec, (*ltstart) + 1).type))
    {
      cst_cstnode_t un_op = (cst_cstnode_t) {
        .type = CNT_PR_UN_OP,
      };
      const int rval
        = cst_parse_un_op_cst(ltvec, &un_op, ltstart);
      if (rval != 0) un_op.type = CNT_PR_ERR;
      return un_op;
    }
    {
      cst_cstnode_type_t cnt;

      switch (ltok.span.str.__s[ltok.span.str.len - 1])
      {
      case '!':
        cnt = CNT_CH_V_SHORT;
        break;

      case '&':
        cnt = CNT_CH_V_LONG;
        break;

      case '#':
        cnt = CNT_CH_V_SINGLE;
        break;

      case '%':
        cnt = CNT_CH_V_DOUBLE;
        break;

      case '$':
        cnt = CNT_CH_V_STRING;
        break;

      default:
        cnt = CNT_CH_V_IMPLICIT;
        break;
      }

      (*ltstart)++;
      return (cst_cstnode_t) {
        .type = cnt,
        .span = ltok.span,
      };
    }

  case TT_NS_STRING:
    (*ltstart)++;
    return (cst_cstnode_t) {
      .type = CNT_CH_V_STRING,
      .span = ltok.span,
    };

  case TT_NS_NUM:
  {
    cst_cstnode_type_t cnt = CNT_CH_V_LONG;
    int dots = 0;

    for (size_t i = 0; i < ltok.span.len; i++)
    {
      if (ltok.span.str.__s[i] == '.') dots++;
    }

    if (dots == 1)
      cnt = CNT_CH_V_DOUBLE;
    else if (dots > 1)
    {
      cst_cstnode_t cn = (cst_cstnode_t) {
        .type = CNT_PR_ERR,
        .span = ltok.span,
      };
      cst_cstnode_t *alloc_node = malloc(sizeof(*alloc_node));
      if (!alloc_node)
        CST_PANIC("Failed to allocate error node in cst_parse_primary_cst().");

      alloc_node->type = CNT_CH_V_DOUBLE;
      alloc_node->span = ltok.span;
      cst_cstnode_push_child(&cn, alloc_node);
      return cn;
    }

    (*ltstart)++;
    return (cst_cstnode_t) {
      .type = cnt,
      .span = ltok.span,
    };
  }

  default:
    break;
  }

  return (cst_cstnode_t) {
    .type  = CNT_PR_ERR,
    .span  = ltok.span,
    .child = NULL,
  };
}

CST_LIB int cst_parse_expr_cst(const blexer_lextok_vec_t *restr ltvec,
                                     cst_cstnode_t       *restr main_node,
                                     size_t              *restr ltstart)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is invalid in cst_parse_expr().");
  if (!blexer_ltvec_validate(ltvec))
    return -1;

  CST_ASSERT(main_node, "main_node is NULL in cst_parse_expr_cst().");
  if (!main_node)
    return -2;

  CST_ASSERT(ltstart, "ltstart is NULL in cst_parse_paren_cst().");
  if (!ltstart)
    return -3;

  CST_ASSERT(ltvec->len <= *ltstart, "ltstart is out of bounds in ltvec in cst_parse_primary_cst().");
  if (ltvec->len > *ltstart)
    return -4;

  cst_cstnode_t expr = cst_parse_primary_cst(ltvec, ltstart);
  if (expr.type == CNT_PR_ERR)
    return 0;

  if (!main_node)
    *main_node = expr;
  else {
    cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(expr);
    if (!alloc_node)
      CST_PANIC("Failed to allocate expression node in cst_parse_expr_cst().");

    cst_cstnode_push_child(main_node, alloc_node);
  }

  size_t local_cur_idx = *ltstart;
  cst_consume_cst_if_space(ltvec, &main_node->span, &local_cur_idx);

  const blexer_lextoken_t ltok = blexer_ltvec_fetch(ltvec, local_cur_idx);

  if (blexer_tt_is_bin_op(ltok.type))
  {
    const int rval = cst_parse_bin_op_cst(ltvec, -1, main_node, ltstart);
    if (!rval)
      return 0;
  }
  else if (blexer_tt_is_un_op(ltok.type))
  {
    const int rval = cst_parse_un_op_cst(ltvec, main_node, ltstart);
    if (!rval)
      return 0;
  }

  return 1;
}

CST_LIB int cst_consume_cst_comment(const blexer_lextok_vec_t *restr ltvec,
                                          cst_cstnode_t       *restr main_node,
                                          size_t              *restr ltstart)
{
  CST_ASSERT(ltvec, "ltvec is NULL in cst_consume_cst_comment().");
  if (!ltvec)
    return 0;

  CST_ASSERT(ltstart, "ltstart is NULL in cst_consume_cst_comment().");
  if (!ltstart)
    return -1;

  CST_ASSERT(*ltstart < ltvec->len, "ltstart is out of bounds in cst_consume_cst_comment().");
  if (*ltstart >= ltvec->len)
    return -2;

  cst_cstnode_t cn = {0};

  {
    const blexer_lextoken_t ltok = blexer_ltvec_fetch(ltvec, *ltstart);
    if (ltok.type == TT_SING_QUOT)
      cn.type = CNT_PR_APOST_COMMENT;
    else if (ltok.type == TT_KW_REM)
      cn.type = CNT_PR_REM_COMMENT;
    else
      CST_PANIC("cst_consume_cst_comment() was called on a non-comment lexer token in cst_consume_cst_comment().");

    cst_cstnode_t cmnt_key = (cst_cstnode_t) {
      .type = CNT_CH_CMNT_KEY,
      .span = ltok.span,
    };

    cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(cmnt_key);
    if (!alloc_node)
    {
      cst_cstnode_free(&cmnt_key);
      CST_PANIC("Failed to allocate comment key in cst_parse_cst_comment().");
    }

    cst_cstnode_push_child(&cn, alloc_node);
  }

  for (size_t i = (*ltstart) + 1; i < ltvec->len; i++)
  {
    const blexer_lextoken_t lt = blexer_ltvec_fetch(ltvec, i);
    cst_cstnode_t cmnt = (cst_cstnode_t) {
      .span     = lt.span,
      .type     = CNT_CH_CMNT_CONTENT,
    };
    cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(cmnt);
    if (!alloc_node) {
      cst_cstnode_free(&cmnt);
      CST_PANIC("Failed to allocate comment content node in cst_parse_cst_comment().");
    }

    cst_cstnode_push_child(&cn, alloc_node);
  }

  return 1;
}

CST_LIB int cst_consume_cst_if_space(const blexer_lextok_vec_t *restr ltvec,
                                           spans_strspan_t     *restr ss,
                                           size_t              *restr idx)
{
  CST_ASSERT(blexer_ltvec_validate(ltvec), "ltvec is invalid in cst_consume_cst_if_space");
  if (!blexer_ltvec_validate(ltvec))
    return -1;

  const blexer_lextoken_t ltok = blexer_ltvec_fetch(ltvec, *idx);
  if (ltok.type != TT_SPACE)
    return 0;

  spans_ss_concat(ss, &ltok.span);
  (*idx)++;
  return 1;
}

CST_LIB int cst_consume_remaining_cst(const blexer_lextok_vec_t *restr ltvec,
                                            cst_cstnode_t       *restr main_node,
                                      const size_t                     ltstart)
{
  if (ltstart == ltvec->len - 1)
    return 0;
  cst_cstnode_t remaining = (cst_cstnode_t) {
    .type = CNT_PR_ERR,
    .span = blexer_ltvec_fetch(ltvec, ltstart).span,
  };

  for (size_t i = ltstart + 1; i < ltvec->len; i++)
  {
    const blexer_lextoken_t ltok = blexer_ltvec_fetch(ltvec, i);
    spans_ss_concat(&remaining.span, &ltok.span);
  }

  cst_cstnode_t *alloc_node = cst_convert_cn_to_alloc(remaining);
  if (!alloc_node)
    CST_PANIC("Failed to allocate remaining token node in cst_consume_remaining_cst().");

  cst_cstnode_push_next(main_node, alloc_node);

  return 1;
}


CST_LIB void cst_cstnode_free(cst_cstnode_t *cn)
{
  if (!cn) return;
  cst_cstnode_t *free_cn;
  cst_cstnode_t *cur_cn = cn;
  while (cur_cn->next)
  {
    free_cn = cur_cn;
    cur_cn = free_cn->next;
    if (free_cn->child) cst_cstnode_free(free_cn->child);
    free(free_cn);
  }
}

#endif /* CST_IMPL*/

/*
#ifdef __cplusplus
}
#endif
*/

#endif

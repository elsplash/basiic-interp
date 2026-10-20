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
#define LEXER_IMPL
#endif
#include "lexer/blexer.h"

#ifndef CST_ASSERT

#include <assert.h>
#define CST_ASSERT(check, message) assert(check && message)

#endif

#ifndef CST_PANIC
#include <stdlib.h>

#define CST_PANIC(message) \
do {                       \
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

CNT_S... => Statement
*/
typedef enum
{
  CNT_LABEL,  /* Alphabetic characters only (TT_NS_ID) */
  CNT_ALNUM,  /* Alphanumeric characters (TT_NS_ALNUM)*/
  CNT_NUMBER, /* Numbers (TT_NS_ALNUM) */

  /* Statements */
  CNT_S_LABEL,
  CNT_S_RUN,

  /* Miscellaneous */
  CNT_M_APOST_COMMENT,
  CNT_M_ERR,
}
cst_cstnode_type_t;

/* The basic struct for the CST */
typedef struct cst_cstnode_t
{
  cst_cstnode_type_t     type;
  spans_strspan_t        span;

  /* NOTE
  This element is used like a linked list,
  which is only one way, so you can link other
  CST nodes to each other.

  This means it can also be NULL.
  */
  struct cst_cstnode_t * contents;
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
Pushes a CST Node to a vector and also expands the array if
necessary, this can return an error code which are the
following:

-2 - Failed to allocate using calloc.
-1 - cnvec is NULL.
 0 - No issues.
*/
CST_STATIC int cst_cnvec_push(cst_cnvec_t *restr cnvec, const cst_cstnode_t cn);

/* This checks if a lexer token is of type `tt`. */
CST_STATIC inline int cst_expect_ltok(const blexer_lextok_vec_t *restr ltvec,
                                      const size_t                     ltv_idx,
                                      const blexer_tokentype_t         tt);

/*
The interface function you may call if you want all the tokens
to be parsed.

The returned value may be NULL if it ran into a problem when
parsing.
*/
CST_LIB cst_cnvec_t* cst_parse_cst_line(const blexer_lextok_vec_t *restr ltvec);

/*
This function will be caled if the parsed line is invalid.

This is not supposed to be reported as of the state right now
since we don't have enough structure.

This can return an error code which are what follows:
-2 - ltvec is NULL
-1 - cnvec is NULL
 0 = No issues
*/
CST_LIB int cst_error_cst(const blexer_lextok_vec_t *restr ltvec,
                          const cst_cnvec_t         *restr cnvec,
                          const size_t                     ltstart);

/*
This function parses the first possible token that could be a label.
Which is why no start is given.

The returned value's .contents is always NULL. So you must check if
the .type is not CNT_S_LABEL instead.
*/
CST_LIB cst_cstnode_t cst_parse_label(const blexer_lextok_vec_t *restr ltvec);

#define cstnode_type_t cst_cstnode_type_t
#define cstnode_t      cst_cstnode_t
#define cnvec_t        cst_cnvec_t

#define cnvec_creat()                   cst_cnvec_creat()
#define cnvec_push(cnvec, cn)           cst_cnvec_push(cnvec, cn)
#define expect_ltok(ltvec, ltstart, tt) cst_expect_ltok(ltvec, ltstart, tt)

#define parse_cst_line(lt, ltvec)        cst_parse_cst_line(lt, ltvec)
#define error_cst(ltvec, cnvec, ltstart) cst_error_cst(ltvec, cnvec, ltstart)
#define parse_label(ltvec)               cst_parse_label(ltvec)

/* --- IMPLEMENTATION --- */

#define CST_IMPL
#ifdef CST_IMPL

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

CST_STATIC inline int cst_expect_ltok(const blexer_lextok_vec_t *restr ltvec,
                                      const size_t                     ltv_idx,
                                      const blexer_tokentype_t         tt)
{
  CST_ASSERT(ltv_idx < ltvec->len, "Attempt to index out of bounds in cst_expect_ltok().");
  CST_ASSERT(ltvec, "Attempt to index ltvec, which is NULL in cst_expect_ltok().");

  return ltvec->ltok_ptr[ltv_idx].type == tt;
}

CST_LIB cst_cnvec_t* cst_parse_cst_line(const blexer_lextok_vec_t *restr ltvec)
{
  cst_cnvec_t *cnvec = calloc(1, sizeof(*cnvec->ptr));
  size_t cur_idx = 0;

  const cst_cstnode_t label = cst_parse_label(ltvec);
  if (label.type == CNT_S_LABEL)
  {
    cst_cnvec_push(cnvec, label);
    if (ltvec->ltok_ptr[1].type == TT_COLON)
      cur_idx = 2;
    else
      cur_idx = 1;
  }

  switch (ltvec->ltok_ptr[cur_idx].type)
  {
  case TT_SING_QUOT:
  {
    const cst_cstnode_t cn = (cst_cstnode_t) {
      .type = CNT_M_APOST_COMMENT,
      .span = ltvec->ltok_ptr[cur_idx].span,
      .contents = NULL,
    };
    cst_cnvec_push(cnvec, cn);
  }
    return cnvec;

  case TT_NS_ID:
    cur_idx++;
    switch (ltvec->ltok_ptr[cur_idx].type)
    {
    case TT_AMPERSAND:
      /* Fallthrough*/
    case TT_EXCL_MARK:
      /* Fallthrough */
    case TT_DOLLAR:
      /* Fallthrough */
    case TT_POUND:
      /* Fallthrough */
    case TT_PERCENT:
      break;

    default:
      goto Error;
    }
    if (cst_expect_ltok(ltvec, cur_idx, TT_SPACE)) goto Error;
    cur_idx++;
    if (cst_expect_ltok(ltvec, cur_idx, TT_EQUAL)) goto Error;
    cur_idx++;
    if (cst_expect_ltok(ltvec, cur_idx, TT_SPACE)) goto Error;
    cur_idx++;
    /* TODO: Make the Variable definition node */
    /* Parse Expression*/
    break;

  case TT_KW_RUN:
  {
    spans_strspan_t ss = ltvec->ltok_ptr[cur_idx].span;
    cur_idx++;

    if (!cst_expect_ltok(ltvec, cur_idx, TT_SPACE)) goto Error;
    ss_concat(&ss, &ltvec->ltok_ptr[cur_idx].span);
    cur_idx++;

    if (!cst_expect_ltok(ltvec, cur_idx, TT_NS_STRING) ||
        !cst_expect_ltok(ltvec, cur_idx, TT_NS_NUM))
      goto Error;
    ss_concat(&ss, &ltvec->ltok_ptr[cur_idx].span);
    /* TODO: Parse expression function */
    cur_idx++;

    cst_cstnode_t cn = (cst_cstnode_t) {
      .type = CNT_S_RUN,
      .span = ss,
      .contents = NULL, /* TODO: Replace this with expression */
    };
    cst_cnvec_push(cn);
  }

    if (!/* TODO: Consume comment token */0)
      goto Error;
    break;

  case TT_KW_CALL:
    break;

  case TT_KW_CHAIN:
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
}

CST_LIB int cst_error_cst(const blexer_lextok_vec_t *restr ltvec,
                          const cst_cnvec_t        *restr cnvec,
                          const size_t					  ltstart)
{
  cst_cstnode_t cn = (cst_cstnode_t) {
    .type = CNT_M_ERR,
    .span = ltvec->ltok_ptr[ltstart].span,
    .contents = NULL,
  };
  
  for (size_t i = ltstart; i < ltvec->len; i++)
  {
    // ...
    cn.contents = (cst_cstnode_t) {
    };
  }

Exit:
  return 0;

Error:
  switch (cst_error_cst(ltvec, cnvec, cur_idx))
  {
  case -2:
    CST_PANIC("CST Error fetch: cnvec is NULL. Aborting...");

  case -1:
    CST_PANIC("CST Error fetch: ltvec is NULL. Aborting...");

  case 0:
    break;

  default:
    CST_PANIC("CST Error fetch: switch case not updated (1)");
  }

  goto Exit;
}

CST_LIB cst_cstnode_t cst_parse_label(const blexer_lextok_vec_t *restr ltvec)
{
  if (ltvec->ltok_ptr[0].type == TT_NS_NUM)
  {
    return (cst_cstnode_t) {
      .type     = CNT_S_LABEL,
      .span     = ltvec->ltok_ptr[0].span,
      .contents = NULL
    };
  }
  else if ((ltvec->ltok_ptr[0].type == TT_NS_ID ||
            ltvec->ltok_ptr[0].type == TT_NS_ALNUM) &&
           ltvec->ltok_ptr[1].type == TT_COLON)
  {
    spans_strspan_t ss = ltvec->ltok_ptr[0].span;
    spans_ss_concat(&ss, &ltvec->ltok_ptr[1].span);
    return (cst_cstnode_t) {
      .type = CNT_S_LABEL,
      .span = ss,
      .contents = NULL,
    };
  }

  return (cst_cstnode_t) {
    .type     = CNT_LABEL,
    .span     = ltvec->ltok_ptr[0].span,
    .contents = NULL,
  };
}

#define CST_FREEARR_INITSZ 8

CST_LIB void cst_cstnode_free(cst_cstnode_t *cn)
{
  cst_cstnode_t *free_cn;
  cst_cstnode_t *cur_cn = cn;
  while (cur_cn->contents)
  {
    free_cn = cur_cn;
    cur_cn = free_cn->contents;
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

/*
A simple CST Parsing header library.

This will parse the tokens from "lexer/lexer.h".
So it can be parsed into an AST later on.
*/

#ifndef CST_DEF
#define CST_DEF

#if defined(CST_IMPL) && !defined(LEXER_IMPL)
#define LEXER_IMPL
#endif
#include "lexer/lexer.h"

#ifndef CST_ASSERT
#define CST_ASSERT(x) assert(x)
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

typedef enum {
  CNT_LABEL,  /* Alphabetic characters only (TT_NS_ID) */
  CNT_ALNUM,  /* Alphanumeric characters (TT_NS_ALNUM)*/
  CNT_NUMBER, /* Numbers (TT_NS_ALNUM) */

  // ...
} cst_cstnode_type_t;

#ifdef CST_IMPL

// ...

#endif

/*
#ifdef __cplusplus
}
#endif
*/

#endif

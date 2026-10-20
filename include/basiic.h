/*

This header library provides the developer
interface for the libraries, and connects them
together.

It also provides automation for each process the
BASIIC interpreter goes through.

 */

#ifndef BASIIC_DEF
#define BASIIC_DEF

/* Define these regardless even if they are already defined. */
#ifdef BASIIC_IMPL
#define BLEXER_IMPL
#define BPARSER_IMPL
#define BANALYZER_IMPL
#define BRUNTIME_IMPL
#endif

#include "lexer/blexer.h"
#include "parser/bparser.h"
#include "sem-analyzer/banalyzer.h"
#include "runtime/bruntime.h"

#ifndef BASIIC_LIB
#define BASIIC_LIB glob_extern
#endif

#ifndef BASIIC_STATIC
#define BASIIC_STATIC glob_static
#endif

#ifndef BASIIC_ASSERT
#define BASIIC_ASSERT(check, message) assert(check && message)
#endif

#ifndef BASIIC_PANIC
#define BASIIC_PANIC(message) \
do {                          \
  puts(message);              \
  exit(1);                    \
} while (0)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* ... */

#ifdef BASIIC_IMPL


/* ... */

#endif

#ifdef __cplusplus
}
#endif

#endif /* BASIIC_DEF */

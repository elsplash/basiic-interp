/* bparser.h
COPYRIGHT (C) 2026 ELSPLASH (electrsplash@gmail.com)
----------------------------------------------------

This simply mixes the two headers in this folder.
Nothing of interest here as well.

-------------------------------------------------
Licensed under the GNU General Public License V3.
*/

#ifndef BPARSER_DEF
#define BPARSER_DEF

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/

#ifdef BPARSER_IMPL

#ifndef CST_IMPL
#define CST_IMPL
#endif

#ifndef AST_IMPL
#define AST_IMPL
#endif

#endif

/* NOTE
Even if redundant, I feel like everyone should
explicitly say what is present, and will be used
later on, even if not necessary.
*/
#include "cst.h"
#include "ast.h"

/* ... */

/* --- IMPLEMENTATION --- */

#ifdef BPARSER_IMPL

/* ... */

#endif

/*
#ifdef __cplusplus
}
#endif
*/

#endif

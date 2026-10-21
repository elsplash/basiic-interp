/*
COPYRIGHT (C) 2026 ELSPLASH (electrsplash@gmail.com)
----------------------------------------------------

This header provides the important macros that all
these header libraries uses.

You may use this separate from the libraries.

-------------------------------------------------
Licensed under the GNU General Public License V3.
*/

#ifndef MACROS_DEF

/*
This macro provides the restrict keyword, though for
some ungodly reason all C compiler providers decieded
to be special in their own ways.
*/
#ifndef restr

#if   defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define restr restrict
#elif defined(_MSC_VER) && _MSC_VER >= 1400
    #define restr __restrict
#elif defined(__GNUC__) || defined(__clang__)
    #define restr __restrict__
#else
    #define restr
#endif

#endif /* restr */

/*
This macro expands to make it so it outputs the function correctly.
*/
#ifndef glob_extern

#ifdef _WIN32
#define glob_extern __declspec(dllexport)

#elif defined(__GNUC__) && __GNUC__ > 4
#define  glob_extern __attribute__((visibility("default")))

#else
#define glob_extern extern

#endif

#endif /* glob_extern */

/*
This macro expands so it makes static functions properly.
*/
#ifndef glob_static

#ifdef _WIN32
#define glob_static __decl_spec(dllimport)

#elif defined(__GNUC__) && __GNUC__ > 4
#define glob_static __attribute__((visibility("hidden")))

#else
#define glob_static static

#endif
#endif /* glob_static */

#endif

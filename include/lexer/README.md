# BLexer

## About the BLexer

Blexer means "BASIC Lexer", in which BASIC means the original langauge's
abbreviation.

This lexer has one dependency which I have written myself, that is the
`span.h`. This single header library will have it's own section.

## BLexer: Overview

This library is a basic foundation of what's to come. This library is
simple, yet, a bit big, if you had me to describe it best.

If you want the most use out of this library, just simply use `spans.h`,
which is styled in STB, coined by Sean Barret.

Do note that most of these function's return values are heap allocated.
So for you developers looking to use this library, please be wary of the
pointers returned by functions, you need to free them.

## BLexer: spans.h

This library serves as a replacement for higher level string manipulation.
Replicates of what C++ or Rust strings have.

This mostly consists of spans, and some manipulations to `str_t`, which is
what it's all based around on.

You may use this as long as it follows the GNU GPL v3 license, since it is
licensed under that.

## BLexer: lexer.h

This library is more BASIIC centered and not intended to be used anywhere
else other than this project, though I still wrote it in the style of
STB, if you really wanted to use it so badly.

It contains just one interfacing library with `ls_table_t` from the `spans.h`
library. It is `tokenize_file`.

Though the returned value of `tokenize_file` is still a vector, so you need
to free it's contents via `ltvec_free`.

## Future Implementations

### Token Casting

For this current date (Oct 15, 2026) I'm writing this section, I haven't written
the token cast for strings (`""` tokens), and comparison tokens, like the greater
or equal to operator (`>=`) I haven't implemented that, since I thought I could
do that in the CST Parser instead.

## Reviewal

The making of this library was rather extraordinary I must say. As I finished the
`lexer.h` library, I accidentally expunged it in Emacs Dired, and I didn't have
a single copy of it, in my machine, so I had to revert it via `git`, and rewrite
it.

Mistakes aside, the journey of making an STB styled header library was pretty
amazing. It was a whole separate experience when I made `MyCraft`. I really hope
that the more I write this it would be more fun.

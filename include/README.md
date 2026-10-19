# BASIIC Library

## About the BASIIC Library

This library is stacked on each other.

Which are the runtime, and semantic analyzer depending
on the AST structure of the parser library, and the parser
library depending on the lexer library, which also depends on
the spans library.

All are written from scratch which are based on qBasic.

## About `basiic.h`

The main library interface, `basiic.h` connects everything
on this library together to create a developer interface usable
anywhere.

This library includes creating bytecode to interpret from
runtime, and also prepending/inserting/appending bytecode.
This also provides functions to run the bytecode.

Else you may use the internal library functions used for more
control.

## Other libraries

The other libraries also has `README.md`s inside them, so I
recommned checking them out, as this `README.md` does not
go into them.

## Future implementation

> To be added

## Review

> To be added

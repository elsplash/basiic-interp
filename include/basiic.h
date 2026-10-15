#define LEXER_IMPL
#include "lexer/lexer.h"

#define CST_IMPL
#include "parser/cst.h"
#define AST_IMPL
#include "parser/ast.h"

#define ANALYZER_IMPL
#include "sem-analyzer/analyzer.h"
#define LINTER_IMPL
#include "sem-analyzer/linter.h"

#define STACK_IMPL
#include "runtime/stack.h"
#define RUNTIME_IMPL
#include "runtime/runtime.h"
#define INTERP_IMPL
#include "runtime/interp.h"

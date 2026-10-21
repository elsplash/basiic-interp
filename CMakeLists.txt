cmake_minimum_required(VERSION 4.2.2)

project(BASIIC-Interpreter VERSION 0.1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD          17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS        OFF)

add_executable(basiic-shell)

add_library(basiic STATIC)

set_target_properties(basiic PROPERTIES LINKER_LANGUAGE C)

target_sources(
  basiic-shell

  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

target_sources(
  basiic

  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/basiic.h

  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/macros.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/lexer/spans.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/lexer/blexer.h

  ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/bparser.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/cst.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/ast.h

  ${CMAKE_CURRENT_SOURCE_DIR}/include/sem-analyzer/banalyzer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/sem-analyzer/linter.h

  ${CMAKE_CURRENT_SOURCE_DIR}/include/runtime/stack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/runtime/interp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/runtime/bruntime.h
)

target_include_directories(
  basiic

  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/

  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/lexer
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parser
  ${CMAKE_CURRENT_SOURCE_DIR}/include/sem-analyzer
  ${CMAKE_CURRENT_SOURCE_DIR}/include/runtime
)

target_link_libraries(
  basiic-shell

  PRIVATE
  basiic
)

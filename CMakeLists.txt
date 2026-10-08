cmake_minimum_required(VERSION 4.2.2)

project(BASIIC-Interpreter VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD          17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS        OFF)

add_executable(basiic-shell)

# Lexer
add_library(spans STATIC)
add_library(basiic_lexer STATIC)

# Parser
add_library(basiic_cst STATIC)
add_library(basiic_ast STATIC)



# The main library
add_library(basiic STATIC)

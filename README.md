# BASIIC Interpreter

## About BASIIC Interpreter

This project is for me to replicate the programming language qBasic
from scratch (C). Just for me to ~~Suffer~~ *learn* more about
lexers, parsers, semantic analyzers, and runtime.

This is mostly meant to be a library, but for fun, I've made it
just a simple replication of the normal BASIC computers, essentially
a shell.

While this is a bit of a stretch in and of itself, this is a part
of a bigger project. Which the next part is going to be my next
project

## Usage

### FOR NORMAL USE

If you intend to just use the program as it is in this repository
right here, you may follow these steps.

First, you may need to have `cmake` to compile this codebase.

On Linux, you can install it via package manager:

```bash
sudo apt install cmake # Debian

sudo pacman -Syu cmake # Arch (Updates and isntalls)

sudo dnf install cmake # Fedora
```

On MacOS, you can either use HomeBrew:

```bash
brew install cmake
```

Or some installer image like `.dmg`, though I won't provide any
links here.

On Windows, you may use the official installer over on the
official CMake website. Or you can install through `winget`.

```bash
winget install cmake
```

### FOR DEVELOPER USE

You may use the STB-styled header files over in the `include`
folder, sorted in folders for your sake.

They're only made for this purpose however, though I can list
some that might pique your interest.

```none
-> spans.h
  A header file with string spans, line spans, and a dynamic
  string.
-> [MORE!! I HOPE!!!]
```

## Future implementations

> To be added

## Review

> To be added

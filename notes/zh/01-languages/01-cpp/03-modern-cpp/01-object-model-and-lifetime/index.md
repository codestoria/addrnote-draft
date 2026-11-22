---
title: Object Model and Lifetime
description: Notes on names, types, const qualifiers, pointers, references, value categories, storage duration, and object lifetime in C++
---

# Object Model and Lifetime

Notes on variables and memory, top-level vs. low-level `const`, pointers and references, expressions and value categories (`lvalue`, `prvalue`, `xvalue`), storage duration, copy elision, and reference lifetime extension.

## Core Modules

- **Variables, Memory, and `const`** (`01-variables-memory-and-const.md`): Separates source-code identifiers from runtime storage (`address`, `size`, `alignment`), distinguishes **objects** from broader **entities** (such as functions and references), and clarifies **top-level `const`** vs. **low-level `const`** across `int`, `int*`, `const int*`, and `int* const`.
- **Pointers and References** (`02-pointers-and-references.md`): Contrasts address-holding pointers (`T*`) with alias references (`T&`), explains why references are not standalone objects (`&ref` and `sizeof(ref)`), covers parameter passing (`value`, `T&`, `const T&`) and array-to-pointer decay, and highlights dangling return traps.
- **Expressions and Value Categories** (`03-expressions-and-value-categories.md`): Classifies expressions by **identity** and **movability** into `lvalue`, `prvalue`, and `xvalue` (and umbrella groups `glvalue` and `rvalue`), demonstrates overload resolution (`f(int&)` vs. `f(int&&)`), and addresses common myths—such as named rvalue references being `lvalue`s and `std::move` being a cast rather than an immediate move.
- **Object Lifetime, Temporaries, and Dangling References** (`04-object-lifetime-temporaries-and-dangling-references.md`): Separates **storage duration** (automatic, static, dynamic, thread) from **object lifetime** (constructor completion to destructor start), covers full-expression temporary destruction, C++17 temporary materialization and guaranteed copy elision, and maps out when reference lifetime extension succeeds or fails.

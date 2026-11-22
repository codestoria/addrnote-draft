---
title: Value Categories and References
description: Value categories and reference binding in C++
---

# Value Categories and References

## Introduction

Most tutorials start directly with rvalue references and `std::move`, which easily leads to a deep-rooted misconception:

> The purpose of rvalue references is to move objects.

This reverses the cause and effect.
**Value categories are the underlying rules; reference binding is the constraint; move semantics and perfect forwarding are higher-level capabilities.**

Every expression in C++ has two independent properties:

1. **type**: the static type of data that the expression represents
2. **value category**: the classification of the expression, which governs which kinds of references it can bind to

The logical chain of this topic:

1. Expressions are classified into lvalue / prvalue / xvalue / glvalue / rvalue;
2. Each value category follows its own reference binding rules;
3. C++11 introduced rvalue references `T&&`, enabling move constructors and move assignment — that is, move semantics;
4. Template argument deduction combined with reference collapsing gives rise to forwarding references;
5. `std::forward` preserves the original value category of an expression, achieving perfect forwarding.

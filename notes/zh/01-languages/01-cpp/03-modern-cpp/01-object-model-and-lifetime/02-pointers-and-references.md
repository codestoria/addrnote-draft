---
title: 'Pointers and References'
description: Master C++ pointers and references — how they differ, why references are aliases rather than separate objects, how parameter passing and array decay work, and how to avoid classic lifetime bugs.
outline: deep
---

# Pointers and References: Aliases, Addresses, and Parameter Passing

::: tip What you'll learn
- Understand the real motivation for using pointers and references instead of copying everything
- Distinguish **pointers** (address holders that can rebind or be null) from **references** (aliases that must bind on creation and cannot rebind)
- See why a reference is **not an independent object** in the C++ object model, and what `&ref` and `sizeof(ref)` actually reveal
- Choose safely among **pass-by-value**, **pass-by-reference (`T&`)**, and **pass-by-const-reference (`const T&`)**
- Understand **array-to-pointer decay** and why passing an array to a function loses size information
- Avoid common traps such as returning references or pointers to local variables and accidental mutations
:::

## Motivation

When you write a basic C++ function like `int add(int a, int b)`, passing arguments by value is effortless: `a` and `b` are small integers, and copying them costs almost nothing.

Problems appear as soon as your programs grow:

1. **Performance cost of copying:** A large struct (such as a `std::string`, a `std::vector`, or a record containing dozens of fields) may allocate heap memory or copy many bytes every time it is passed or returned by value.
2. **Mutating caller state:** A function often needs to modify a variable that lives in the caller's scope, or select between different existing objects at runtime.
3. **Optional or reseatable targets:** Sometimes an API needs to represent "no target available" (`nullptr`) or switch which object it points to over time, while other APIs want a guarantee that a valid object is already present.

C++ gives you two primary indirection tools to solve these problems: **pointers** and **references**. Understanding how they work—and where they differ—is essential before moving on to value categories, move semantics, and forwarding.

::: tip When this matters most
Passing large objects to functions, modifying caller data cleanly, building data structures (linked nodes, trees, graphs), working with C APIs or optional targets, and preparing for move semantics and references in templates.
:::

::: info When you can ignore it
Small, trivially copyable types (like `int`, `double`, `char`, or tiny POD structs) passed to pure functions that do not need to mutate caller state—though even there, knowing how references behave prevents surprises.
:::

::: danger First, unlearn the "second variable" myth
A **reference** is not a separate variable that secretly carries a hidden pointer inside every expression. In C++, a reference is an **alias**—another name for an existing object. Treating a reference as a standalone object will trip you up on addresses, `sizeof`, lifetime, and later topics like value categories and `decltype`.
:::

## Core Idea & Intuition

### Objects, addresses, and indirection

At runtime, an **object** in C++ occupies a region of storage with a **type**, a **value**, and (usually) a **memory address**.

- A **pointer** stores a memory address (or `nullptr`). Because it holds an address as its own value, you can inspect the address, reassign the pointer to point somewhere else, or set it to `nullptr`.
- A **reference** is an alias bound to an existing object at the moment of initialization. Once bound, using the reference's name refers directly to that underlying object.

### Pointers vs. references: the core contrast

| Property | Pointer (`T*`) | Reference (`T&`) |
| --- | --- | --- |
| **What it represents** | An object holding a memory address (or null) | An alias (another name) for an existing object |
| **Must initialize on creation?** | No (uninitialized pointers hold indeterminate addresses—dangerous!) | Yes (must bind to a valid object or valid reference when declared) |
| **Can rebind to another object?** | Yes (`ptr = &other;`) | No (assignment `ref = other;` assigns to the *referenced object*, not rebinding `ref`) |
| **Can be null (`nullptr`)?** | Yes | No (in well-defined C++, a reference must refer to a valid object/function) |
| **Has its own address (`&`)?** | Yes (`&ptr` is the address of the pointer variable itself) | No (`&ref` yields the address of the *referenced object*) |
| **Indirection / member syntax** | `*ptr` to dereference; `ptr->member` for members | `ref` directly; `ref.member` for members |

### Why "a reference is not an independent object"

In the C++ language specification, a reference is **not an object**. That means:

- There is no array of references (`T& arr[3]` is ill-formed).
- There is no pointer to a reference (`T&* p` is ill-formed).
- Taking `&ref` does **not** give you the address of a "reference object"; it gives you the address of the object `ref` is bound to.
- Applying `sizeof(ref)` does **not** return the size of a pointer; it returns `sizeof(T)`—the size of the referenced type (evaluated without creating a runtime object in unevaluated operands).

::: tip Two names for one house
Imagine a house has an official street name on the city map (`x`), and your family also calls it "the lake house" (`ref`).

- If you paint the front door of "the lake house" (`ref = 100;`), the house at `x` now has a freshly painted door (`x` is `100`).
- If you ask for the street address of "the lake house" (`&ref`), you get the exact same address as `&x`.
- You cannot peel the nickname "the lake house" off that house and stick it onto a different house later.

By contrast, a **pointer** (`int* p = &x;`) is like a sticky note with the street address written on it. You can read the note (`*p`), throw it away, or write a different house's address on a new sticky note (`p = &y;`).
:::

::: warning Where the analogy breaks
Compilers often implement references *under the hood* using registers or pointer-like addresses in machine code when an alias cannot be completely optimized away. However, **implementation is not language semantics**. In C++ rules, a reference is an alias, not a pointer object, and you cannot perform pointer arithmetic (`++ref`) or set it to `nullptr`.
:::

### Why do we have both?

- **Use `const T&`** as your default when reading a non-trivial object without copying it and without modifying it. It is clean, safe from null dereferences, and works seamlessly with temporary objects (rvalues).
- **Use `T&`** when a function *must* modify a caller's existing object and a target is always required.
- **Use `T*` (or `const T*`)** when "no object" (`nullptr`) is a meaningful state, when you need to reseat/rebind which object is targeted over time, when building pointer-linked data structures, or when interfacing with APIs that use pointers.

## Formal Definition / Mechanism

### Pointer declaration and dereferencing mechanics

A pointer type `T*` names a type whose values are addresses pointing to objects of type `T` (or functions, or `nullptr`).

```cpp
int value = 10;
int* ptr = &value;  // ptr stores the address of value

*ptr = 25;          // dereferences ptr and modifies value to 25
```

- `&value` (the address-of operator) produces a pointer value of type `int*` pointing to `value`.
- `*ptr` (the indirection/dereference operator) accesses the object at the address stored in `ptr`. Dereferencing a null pointer or a pointer that does not point to a valid live object is **undefined behavior**.

### Reference binding mechanics

A reference declaration binds a name to an existing entity at initialization:

```cpp
int value = 10;
int& ref = value;   // ref is an lvalue reference bound to value

ref = 30;           // modifies value; does NOT rebind ref
```

Key rules of reference binding:

1. **Initialization is mandatory:** A reference must be initialized when defined (except in special cases like function parameters, class members initialized in a constructor initializer list, or `extern` declarations).
2. **No reseating:** Once `int& ref = value;` is established, writing `ref = other;` invokes assignment on the `int` object `value`, leaving `ref` bound to `value`.
3. **Constness and binding rules:**
   - A non-const lvalue reference (`T&`) can bind only to a modifiable lvalue of type `T` (and cannot bind to a `const` object or a temporary).
   - A `const` lvalue reference (`const T&`) can bind to lvalues (both `const` and non-const) **and** to rvalues (such as literals or temporaries), extending the lifetime of a bound temporary to match the lifetime of the reference (with important lifetime caveats—see Common Pitfalls).

### Parameter passing: value vs. `T&` vs. `const T&`

When calling a function `f(arg)`:

- **Pass by value (`void f(BigObj x)`):** Initializes a brand-new `BigObj` parameter inside `f` from `arg` (via copy or move). Changes to `x` inside `f` do **not** affect the caller's argument.
- **Pass by reference (`void f(BigObj& x)`):** Binds `x` directly to the caller's lvalue argument without copying. Changes to `x` inside `f` **directly modify** the caller's object. Because it takes `BigObj&`, you cannot pass a temporary like `makeBigObj()` directly to it.
- **Pass by const reference (`void f(const BigObj& x)`):** Binds `x` without copying, while forbidding modification of `x` through that reference inside `f`. Because it is `const`, it can bind to lvalues *and* temporaries (rvalues).

::: info Rule of thumb for function parameters
- Small cheap-to-copy types (`int`, `double`, `bool`, small enums, `std::string_view`, `std::span` of small elements): **pass by value** is often fastest and clearest.
- Larger or non-trivial types you only read: **pass by `const T&`**.
- Objects you intend to mutate in place: **pass by `T&`** (or `T*` if `nullptr` is a valid/expected case).
:::

### Array-to-pointer decay

An **array** in C++ is a contiguous sequence of elements of the same type, such as `int arr[4] = {10, 20, 30, 40};`. Its true type includes its length (`int[4]`), and `sizeof(arr)` is `4 * sizeof(int)`.

However, in most value expressions—including when you pass an array name to a function taking `int*`—an array expression undergoes **array-to-pointer decay**: it implicitly converts to a **prvalue pointer** to its first element (`&arr[0]`, of type `int*`).

```cpp
void print_size_wrong(int* p) {
    // p is just a pointer; it has NO idea how many elements arr had!
    // sizeof(p) is sizeof(int*), NOT the size of the original array.
}
```

Why does C++ do this? Historically, C and C++ passed arrays by pointer rather than copying entire arrays on every function call, and pointer arithmetic (`p + 1`) gave a uniform way to walk through elements. The trade-off is that a raw `T*` loses the array's bound unless you pass the size separately or use a size-aware type like `std::span` or `std::array`.

## Step-by-Step Example

Let's walk through a complete program that demonstrates pointers, references, address identity, parameter passing modes, and array decay.

### Step 1 — Observe identity with `&` and `sizeof`

```cpp
#include <iostream>

int main() {
    int original = 42;
    int& alias = original;
    int* pointer = &original;

    std::cout << "&original = " << static_cast<const void*>(&original) << '\n';
    std::cout << "&alias    = " << static_cast<const void*>(&alias) << '\n';
    std::cout << "pointer   = " << static_cast<const void*>(pointer) << '\n';
    std::cout << "&pointer  = " << static_cast<const void*>(&pointer) << '\n';

    std::cout << "sizeof(original) = " << sizeof(original) << '\n';
    std::cout << "sizeof(alias)    = " << sizeof(alias) << '\n';
    std::cout << "sizeof(pointer)  = " << sizeof(pointer) << '\n';
}
```

What happens here:
1. `&original` and `&alias` print the **same address** because `alias` is an alias for `original`, not a separate object.
2. `pointer` holds that same address, while `&pointer` is a **different address** because `pointer` is a real object in memory storing an address.
3. `sizeof(alias)` equals `sizeof(original)` (for example, `4` on a typical 32-bit `int`), whereas `sizeof(pointer)` is the size of a pointer (for example, `8` on a 64-bit platform).

### Step 2 — Rebinding a pointer vs. assigning through a reference

```cpp
#include <iostream>

int main() {
    int a = 10;
    int b = 20;

    int& ref = a;
    ref = b;  // assigns 20 to 'a' — does NOT make 'ref' refer to 'b'!

    int* ptr = &a;
    ptr = &b;  // rebinds 'ptr' so it now points to 'b'

    std::cout << "a = " << a << ", b = " << b << '\n';
    std::cout << "*ptr = " << *ptr << '\n';
}
```

Output:

```text
a = 20, b = 20
*ptr = 20
```

Notice that `ref = b;` changed `a` to `20`, while `ref` stayed bound to `a`. By contrast, `ptr = &b;` changed which object `ptr` points to, so `*ptr` is now `20` (`b`), leaving `a` untouched by that reassignment.

### Step 3 — Compare pass-by-value, `T&`, and `const T&`

```cpp
#include <iostream>
#include <string>

struct Profile {
    std::string name;
    int score;
};

void show_by_value(Profile p) {
    p.score += 100;  // modifies only the local copy 'p'
    std::cout << "[by value] inside: " << p.name << " (" << p.score << ")\n";
}

void show_by_const_ref(const Profile& p) {
    // p.score += 100;  // Error: 'p' is const, cannot modify through it
    std::cout << "[by const&] inside: " << p.name << " (" << p.score << ")\n";
}

void award_bonus(Profile& p, int bonus) {
    p.score += bonus;  // modifies the caller's Profile directly
}

int main() {
    Profile user{"Maya", 50};

    show_by_value(user);      // copies 'user' (including std::string data)
    std::cout << "after show_by_value: score = " << user.score << '\n';

    show_by_const_ref(user);  // no copy, read-only view
    show_by_const_ref(Profile{"Temporary", 10});  // OK: const& binds to temporary

    award_bonus(user, 25);    // mutates 'user.score' in place
    std::cout << "after award_bonus: score = " << user.score << '\n';
}
```

Output:

```text
[by value] inside: Maya (150)
after show_by_value: score = 50
[by const&] inside: Maya (50)
[by const&] inside: Temporary (10)
after award_bonus: score = 75
```

Why this matters:
- `show_by_value` copies `user` into `p`, so `p.score += 100` leaves `user.score` at `50`.
- `show_by_const_ref` avoids copying `user` and even accepts a temporary `Profile{"Temporary", 10}`, while the compiler stops accidental writes to `p`.
- `award_bonus` takes `Profile&` because its explicit job is to update `user.score` in place.

### Step 4 — See array decay in action

```cpp
#include <cstddef>
#include <iostream>

// 'nums' decays to 'int*', so 'n' must be passed explicitly
void print_elements(const int* nums, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        std::cout << nums[i] << (i + 1 < n ? ", " : "\n");
    }
}

int main() {
    int scores[4] = {88, 92, 79, 95};

    std::cout << "sizeof(scores) in main = " << sizeof(scores) << '\n';
    print_elements(scores, 4);  // 'scores' decays from int[4] to int*
}
```

In `main()`, `scores` has type `int[4]`, so `sizeof(scores)` is `4 * sizeof(int)`. Inside `print_elements`, the parameter is `const int*`, so `scores` decayed to a pointer to `scores[0]` and lost its compile-time length.

::: tip Key insight
Array decay happens automatically whenever an array is converted to a pointer in expressions—except in specific contexts like `sizeof(arr)`, `alignof(arr)`, `decltype(arr)`, the unary `&` operator (`&arr` has type `int(*)[4]`), or binding directly to an array reference (`int (&ref)[4] = arr;`).
:::

## Common Misunderstandings & Pitfalls

### Pitfall 1 — Returning a reference or pointer to a local variable

::: danger Undefined behavior (dangling reference / dangling pointer)
```cpp
int& bad_ref() {
    int local = 10;
    return local;  // 'local' is destroyed when bad_ref() returns!
}

int* bad_ptr() {
    int local = 10;
    return &local;  // points to stack memory that is no longer valid
}
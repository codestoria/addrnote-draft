# Section 13: C++ classes

Each subdirectory is an independent example. Use a C++17 compiler and compile
all `.cpp` files in that directory together. For example, from the repository root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic .github/Section13/Friends/*.cpp -o /tmp/section13-friends
/tmp/section13-friends
```

Replace `Friends` with another directory name to build that example. Most examples
print their results; `DeclareClassAndObjects` and `ConstructorInitializationLists`
finish without output. Some class declarations intentionally omit method definitions
because those methods are not called in the demonstration.

**ShallowCopy is an intentionally unsafe example.** Passing its object by value
shares the owned pointer; destruction leaves a dangling pointer and subsequent
access or deletion causes undefined behavior. Running it may crash or appear to
succeed. Use it only to study the failure; compare it with `DeepCopy`, which
implements both copying and copy assignment.

In `MoveConstructor`, a moved-from object has no value: it can be destroyed,
assigned a new value with `set_data_value`, or moved/copied again. Calling
`get_data_value` while it is empty throws `std::logic_error`. Assignment operators
are deleted to keep the example focused on constructors.

The movie challenge illustrates the operations in its exercise, not input
validation: rating strings and nonnegative watch counts are assumed valid.
Before redistributing this material, verify the source and permission to publish it;
no license for these examples is stated here.

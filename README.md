# CppND-System-Monitor

This repository contains code for the System Monitor Project in the C++ OOP course provided by Udacity. The code is based on the [CppND-System-Monitor Starter Code](https://github.com/udacity/CppND-System-Monitor/).

Although only a slight modification is required, I decided to have a little more in-depth practice by splitting the headers and implementations into separate files.

A good portion of the starter code have also been modified to hopefully achieve a better performance. The code is also reformatted to closely follow the Google C++ Style. See the following section for all the changes.

## Modifications

1. All implementation files have the extension `.cc`.
2. Except `constants.h`, all function bodies are included in their corresponding `.cc` files.
3. Most of the functions are modified to take `const` reference arguments, as new copies are mostly not necessary.
4. `CPUStates` in `constants.h` is changed to a scoped enum class. A function `GetCPUStatesValue` is added for retrieving the underlying `int`.
5. Added header guards to header files.
6. Functions are renamed to follow the Capitalized First Letter convention.
7. Class members are renamed to snake case, with trailing underscores.
8. Added some utility functions to `Util`

## Install Dependencies

```bash
$ sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Compilation & Execution

```bash
$ g++ -std=c++17 *.cc -lncurses
$ ./a.out
```

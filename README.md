# stl

'stl' is a recursive acronym. It stands for "STL for Tim's Learning".
It's a toy project where I can reimplement parts of the STL as a learning exercise.
I'll try to adapt tests from llvm libc++.
I may add an animation layer in future to try to visualize the algorithms / containers.

AI Disclaimer: 
- Some/much of the build infrastructure
- Some/much of the tests
- *None* of the STL implementation

## TODO

- Extend the libc++ test harness to cover compile-only (`*.compile.pass.cpp`), verification, and `assert.*` tests once we have the necessary death-test and diagnostic infrastructure in place.
- Add a license. Ensure it's compatible with llvm because we are copy/pasting their tests

# stl

'stl' is a recursive acronym. It stands for "STL for Tim's Learning".
It's a toy project where I can reimplement parts of the STL as a learning exercise.
I'll try to adapt tests from llvm libc++.
I may add an animation layer in future to try to visualize the algorithms / containers.

AI Disclaimer: 
- Some/much of the build infrastructure
- Some/much of the tests
- *None* of the STL implementation

This project is released under the MIT License (see `LICENSE`).
Upstream tests (`tests/libcxx/...`) are copied directly from the LLVM libc++ project; they retain their original copyright and licensing (Apache 2.0 with LLVM exceptions), and no ownership is claimed here.

# Development

Generally when adding a new container/algo I'll:
- Ask codex to copy the llvm tests and get them compiling against the system header
- Ask codex to add a small shim to pick up the new file
- Take the API from cppreference
- Disable copilot and start coding

## TODO

- Extend the libc++ test harness to cover compile-only (`*.compile.pass.cpp`), verification, and `assert.*` tests once we have the necessary death-test and diagnostic infrastructure in place.

## Add

- Custom exception walking/handling code could be good

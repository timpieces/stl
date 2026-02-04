# AGENTS

## Human Developer
- Owns the STL implementations, API design decisions, and algorithm visualizations.
- Reviews all non-trivial infrastructure patches before they land in main.
- Provides direction on which pieces of libc++/LLVM tests should be imported.

## AI Assistant
- Sets up and maintains build/test infrastructure (CMake, conda, scripts).
- Helps port or adapt tests and automation from upstream LLVM.
- Documents workflows so additional contributors can reproduce the setup easily.
- Avoids making authoritative design decisions about library semantics without human sign-off.

## Project Notes for Agents

- Layout: headers under `include/stl`, sources in `src`, tests in `tests`, helper scripts in `scripts`, external checkouts in `third_party` (ignored by git).
- Imported libc++ tests live under `tests/libcxx`; they're copied from `third_party/llvm-project` (e.g., `array/`) plus the necessary `support/` headers, and only runtime `.pass.cpp` files (non-assert) are currently wired into CTest.
- STL headers may shadow the system ones (e.g., `include/array`) to route libc++ tests toward our implementations; define `STL_IGNORE_ARRAY=ON` at configure time if you need to fall back to the system header temporarily.
- Toolchain: prefer the latest clang/libc++ from `environment.yml` via `conda` while relying on system git/make/python/cmake. No local conda cache directories should be created—use the global cache instead.
- Build workflows: always drive configuration/build/test through the `Makefile` targets (`configure`, `build`, `test`, `run`, `clean`, `llvm`). `make configure` must symlink `compile_commands.json` from the build tree.
- LLVM tests: use `scripts/fetch_llvm.sh` (or `make llvm`) for a depth-1 clone of `llvm-project` on `main` before copying tests; the checkout lives under `third_party/llvm-project` and stays out of git.
- Formatting: follow `.clang-format` (copied from `../cgym`).

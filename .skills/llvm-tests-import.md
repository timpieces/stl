# Skill: Importing LLVM libc++ Tests for STL work

Use this when wiring up libc++ tests (e.g., `array`) for new STL features.

1. Ensure LLVM checkout exists (via `make llvm` once); if `third_party/llvm-project` already exists, reuse it without syncing.
2. Locate upstream tests inside `third_party/llvm-project/libcxx/test/std/...` matching the feature. Mirror that relative path inside `tests/libcxx/` (e.g., `rsync -a third_party/llvm-project/libcxx/test/std/... tests/libcxx/`).
3. Copy any needed headers from `libcxx/test/support/` into `tests/libcxx/support/` if they’re missing.
4. CMake auto-discovers runtime `.pass.cpp` files via `file(GLOB_RECURSE ...)`; make sure new files have that suffix and live under the mirrored path. For compile-only / verify / assert tests add TODO comments when skipping.
5. Reconfigure/build: `make clean && make build`, then `make test` to ensure new `libcxx_...` targets pass.
6. Document notable skips or local edits in `AGENTS.md` or commit messages so future contributors understand the state.

Tips:
- Keep target names unique by deriving from relative paths (already in `CMakeLists.txt`).
- Leave upstream tests unmodified unless necessary; annotate any local tweaks.
- Remember `.gitignore` already excludes `third_party/llvm-project/`, so the checkout stays out of git.

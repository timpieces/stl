#include "stl/hello.h"

#include <string_view>

namespace {
constexpr std::string_view kGreeting{"hello world"};
// Use string_view::contains to ensure we only build with a C++23-capable toolchain.
static_assert(kGreeting.contains("hello"), "Requires C++23 string_view::contains");
}  // namespace

namespace stl {

std::string hello() {
    return std::string{kGreeting};
}

}  // namespace stl

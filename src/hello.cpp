#include "stl/hello.h"

#include <string_view>

namespace {
constexpr std::string_view kGreeting{"hello world"};
static_assert(kGreeting.contains("hello"), "Requires C++23 string_view::contains");
}  // namespace

namespace stl {

std::string hello() {
    return std::string{kGreeting};
}

}  // namespace stl

#pragma once

#include <cstddef>
#include <type_traits>

namespace stl::detail {
template <class T>
inline constexpr bool dependent_false_v = false;
}

namespace std {

template <class T, std::size_t N>
struct array {
    static_assert(stl::detail::dependent_false_v<T>,
                  "stl array shim placeholder: not implemented yet");
};

}  // namespace std

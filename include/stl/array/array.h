#pragma once

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>

namespace std {

namespace detail {
// we implement a base class to customize storage for when
// there are zero elements and so we don't have to access elems through
// a storage member
template <typename T, size_t N>
struct array_base {
    T mElems[N];

    constexpr T& elem(size_t pos) { return mElems[pos]; }
    constexpr const T& elem(size_t pos) const { return mElems[pos]; }
};

template <typename T>
struct array_base<T, 0> {
    constexpr T& elem(size_t) {
        static_assert(false, "Cannot access storage in 0-length array");
    }
    constexpr const T& elem(size_t) const {
        static_assert(false, "Cannot access storage in 0-length array");
    }
};
}  // namespace detail

// use inline namespace to match libc++ namespaces, otherwise
// we get conflicts from the forward declarations
inline namespace __1 {

// T must be move constructible and move assignable
// N can be zero
template <typename T, std::size_t N>
struct array : public detail::array_base<T, N> {
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using detail::array_base<T, N>::elem;

    //     T mElems[N];
    // };

    constexpr T& at(size_type pos) {
        if (pos >= N) {
            throw std::out_of_range("out of range");
        }
        return (*this)[pos];
    }

    constexpr const T& at(size_type pos) const {
        if (pos >= N) {
            throw std::out_of_range("out of range");
        }
        return (*this)[pos];
    }

    constexpr T& operator[](size_type pos) { return elem(pos); }
    constexpr const T& operator[](size_type pos) const { return elem(pos); }
    constexpr size_type size() const { return N; }
};

template <typename T, typename... Ts>
array(T&& t, Ts&&... ts)
    -> array<T, sizeof...(ts) + 1>;  // TODO: require Ts are same
template <typename T>
array(std::initializer_list<T> elems) -> array<T, elems.size()>;

}  // namespace __1
}  // namespace std

#pragma once

#include <algorithm>  // for lexicographic compare
#include <cstddef>
#include <exception>  // terminate
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace std {

namespace detail {
// we implement a base class to customize storage for when
// there are zero elements and so we don't have to access elems through
// a storage member
template <typename T, size_t N>
struct array_base {
    T mElems[N];

    // TODO: simplify const overloads with deducing this
    constexpr T& elem(size_t pos) { return mElems[pos]; }
    constexpr const T& elem(size_t pos) const { return mElems[pos]; }

    constexpr T* data() noexcept { return mElems; }
    constexpr const T* data() const noexcept { return mElems; }
};

template <typename T>
struct array_base<T, 0> {
    // TODO: I've replaced these with std::terminate so I can probably remove
    // all the if constexpr now
    constexpr T& elem(size_t) {
        // UB
        std::terminate();
    }
    constexpr const T& elem(size_t) const { std::terminate(); }

    // TODO: why?
    constexpr T* data() noexcept { return nullptr; }
    constexpr const T* data() const noexcept { return nullptr; }
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
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using detail::array_base<T, N>::elem;
    using detail::array_base<T, N>::data;

    constexpr T& at(size_type pos) {
        if constexpr (empty()) {
            throw_out_of_range();
        } else {
            if (pos >= N) {
                throw_out_of_range();
            }
            return (*this)[pos];
        }
    }

    constexpr const T& at(size_type pos) const {
        if constexpr (empty()) {
            throw_out_of_range();
        } else {
            if (pos >= N) {
                throw_out_of_range();
            }
        }
        return (*this)[pos];
    }

    constexpr T& operator[](size_type pos) noexcept { return elem(pos); }
    constexpr const T& operator[](size_type pos) const noexcept {
        return elem(pos);
    }
    static constexpr size_type size() noexcept { return N; }

    constexpr void fill(const T& value) {
        if constexpr (N == 0) {
            return;
        } else {
            // TODO: replace with iterator?
            for (auto i = size_t{}; i < size(); i++) {
                elem(i) = value;
            }
        }
    }

    constexpr size_type max_size() const noexcept { return size(); }

    static constexpr bool empty() noexcept { return size() == 0; }

    constexpr void swap(array& other) noexcept {
        if constexpr (empty()) {
            return;
        } else {
            for (size_type i{}; i < size(); i++) {
                ::std::swap((*this)[i], other[i]);
            }
        }
    }

    constexpr T& front() noexcept {
        if constexpr (empty()) {
            std::terminate();
        } else {
            return (*this)[0];
        }
    }

    constexpr const T& front() const noexcept {
        if constexpr (empty()) {
            std::terminate();
        } else {
            return (*this)[0];
        }
    }

    constexpr T& back() noexcept {
        if constexpr (empty()) {
            std::terminate();
        } else {
            return (*this)[N - 1];
        }
    }

    constexpr const T& back() const noexcept {
        if constexpr (empty()) {
            std::terminate();
        } else {
            return (*this)[N - 1];
        }
    }

    // constexpr iterator begin() { return iterator{0, this}; }
    // constexpr const_iterator begin() const { return const_iterator{0, this};
    // } constexpr iterator end() { return iterator{N, this}; } constexpr
    // const_iterator end() const { return const_iterator{N, this}; }

    constexpr iterator begin() noexcept { return data(); }
    constexpr const_iterator begin() const noexcept { return data(); }
    constexpr iterator end() noexcept { return data() + N; }
    constexpr const_iterator end() const noexcept { return data() + N; }
    constexpr const T* const cbegin() noexcept { return data(); }
    constexpr const T* const cend() noexcept { return data() + N; }
    constexpr reverse_iterator rbegin() noexcept {
        return reverse_iterator{data() + N};
    }
    constexpr reverse_iterator rend() noexcept {
        return reverse_iterator{data()};
    }
    constexpr const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator{data() + N};
    }
    constexpr const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator{data()};
    }
    constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator{data() + N};
    }
    constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator{data()};
    }

private:
    [[noreturn]] void throw_out_of_range() const {
        throw std::out_of_range("out of range");
    }
};

// Non-Member Operators

template <typename T, size_t N>
constexpr bool operator==(const std::array<T, N>& a1,
                          const std::array<T, N>& a2) {
    using type = std::array<T, N>;
    if constexpr (type::empty()) {
        return true;
    } else {
        for (typename type::size_type i{}; i < type::size(); i++) {
            if (a1[i] != a2[i]) {
                return false;
            }
        }
        return true;
    }
}

// TODO: Reimplement lexicographical_compare_three_way and
// synth_three_way_result using auto breaks sfinae that is done here
template <typename T, size_t N>
constexpr __synth_three_way_result<T> operator<=>(const array<T, N>& a1,
                                                  const array<T, N>& a2) {
    // TODO: implement manually
    return std::lexicographical_compare_three_way(
        a1.begin(), a1.end(), a2.begin(), a2.end(), std::__synth_three_way);
    // using type = array<T, N>;
    // if constexpr (type::empty()) {
    //     return std::strong_ordering::equal;
    // } else {
    //     for (typename type::size_type i{}; i < type::size(); i++) {
    //         if (a1[i] < a2[i]) {
    //             return std::strong_ordering::less;
    //         } else if (a1[i] != a2[i]) {
    //             return std::strong_ordering::greater;
    //         }
    //     }
    //     return std::strong_ordering::equal;
    // }
}

// Deduction Guides

template <typename T, typename... Ts>
array(T&& t, Ts&&... ts)
    -> array<T, sizeof...(ts) + 1>;  // TODO: require Ts are same
template <typename T>
array(initializer_list<T> elems) -> array<T, elems.size()>;

// Non-Member Utilities

// TODO: use one overload and constrain
// template <typename Array, size_t Pos>
// auto get(Array&& a) -> requires { } {
//
// }

template <size_t Pos, typename T, size_t N>
constexpr T& get(array<T, N>& a) noexcept {
    return a[Pos];
}

template <size_t Pos, typename T, size_t N>
constexpr const T& get(const array<T, N>& a) noexcept {
    return a[Pos];
}

template <size_t Pos, typename T, size_t N>
constexpr T&& get(array<T, N>&& a) noexcept {
    return std::move(a[Pos]);
}

template <size_t Pos, typename T, size_t N>
constexpr const T&& get(const array<T, N>&& a) noexcept {
    return std::move(a[Pos]);
}

template <size_t Idx, typename Array>
struct tuple_element;
template <size_t Idx, typename T, size_t N>
struct tuple_element<Idx, array<T, N>> {
	using type = T;
};

template <typename Array>
struct tuple_size;
template <typename T, size_t N>
struct tuple_size<array<T, N>> {
    static constexpr size_t value = N;
};

namespace detail {
// TODO: can we use concepts to simplify this?
template <typename T, size_t N, typename Array>
constexpr array<remove_cv_t<T>, N> to_array_impl(Array&& a) {
    return [&a]<size_t... Is>(index_sequence<Is...>) {
        return array<remove_cv_t<T>, N>{std::forward_like<Array>(a[Is])...};
    }(make_index_sequence<N>{});
}
}  // namespace detail

template <class T, size_t N>
constexpr array<remove_cv_t<T>, N> to_array(T (&a)[N]) {
    return detail::to_array_impl<T, N>(a);
}

template <class T, size_t N>
constexpr array<remove_cv_t<T>, N> to_array(T (&&a)[N]) {
    // TODO: maybe we need to do a variadic for this ??
    // if we create the array and then assign we get an undesired constructor
    // call for each element in the defaulted array
    return [&a]<size_t... Is>(index_sequence<Is...>) {
        return array<remove_cv_t<T>, N>{std::move(a[Is])...};
    }(make_index_sequence<N>{});
}

}  // namespace __1
}  // namespace std

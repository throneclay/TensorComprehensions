#include <type_traits>

namespace tc {
template <class...>
using void_t = void;

template <typename T, typename = void>
struct is_std_container : std::false_type {};

template <typename T>
struct is_std_container<
    T,
    void_t<
        decltype(std::declval<T&>().begin()),
        decltype(std::declval<T&>().end()),
        typename T::value_type>> : std::true_type {};
} // namespace tc

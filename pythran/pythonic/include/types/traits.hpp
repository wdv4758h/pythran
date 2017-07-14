#ifndef PYTHONIC_INCLUDE_TYPES_TRAITS_HPP
#define PYTHONIC_INCLUDE_TYPES_TRAITS_HPP

#include <complex>

namespace pythonic
{

  namespace types
  {

    template <class T>
    struct is_complex : std::false_type {
      using type = T;
    };

    template <class T>
    struct is_complex<std::complex<T>> : std::true_type {
      using type = T;
    };

    template <class T>
    struct is_dtype {
      static constexpr bool value =
          std::is_scalar<T>::value or is_complex<T>::value;
    };

#define MEMBER_TYPE_TRAIT(check_struct, member)                                \
  template <typename T>                                                        \
  struct check_struct {                                                        \
    using yes = char;                                                          \
    using no = struct {                                                        \
      char _[2];                                                               \
    };                                                                         \
    template <class C>                                                         \
    static yes _test(typename C::member *);                                    \
    template <class C>                                                         \
    static no _test(...);                                                      \
    static const bool value =                                                  \
        sizeof(_test<typename std::remove_reference<T>::type>(nullptr)) ==     \
        sizeof(yes);                                                           \
  };

#define MEMBER_ATTR_TRAIT(check_struct, member)                                \
  template <typename T>                                                        \
  struct check_struct {                                                        \
    using yes = char;                                                          \
    using no = struct {                                                        \
      char _[2];                                                               \
    };                                                                         \
    template <class C>                                                         \
    static yes _test(decltype(&C::member));                                    \
    template <class C>                                                         \
    static no _test(...);                                                      \
    static const bool value =                                                  \
        sizeof(_test<typename std::remove_reference<T>::type>(nullptr)) ==     \
        sizeof(yes);                                                           \
  };

    /* trait to check if a type is iterable*/
    MEMBER_TYPE_TRAIT(is_iterable, iterator);

    /* trait to check if a type is callable */
    MEMBER_TYPE_TRAIT(is_callable, callable);

    /* trait to check if a type is pure */
    MEMBER_TYPE_TRAIT(is_pure, pure);

    /* trait to check if the type has a size member */
    MEMBER_ATTR_TRAIT(has_size, size);

    /* trait to check if a type has a fast iterator */
    MEMBER_TYPE_TRAIT(has_fast_iterator, const_fast_iterator);

    /* trait to check if the type has a contains member */
    template <typename T, class V>
    struct has_contains {
      using yes = char;
      using no = struct {
        char _[2];
      };
      template <class C>
      static yes _test(decltype(&C::contains));
      template <class C>
      static yes _test(decltype(&C::template contains<V>));
      template <class C>
      static no _test(...);
      static const bool value =
          sizeof(_test<typename std::remove_reference<T>::type>(nullptr)) ==
          sizeof(yes);
    };

    /* trait to check if the type has a shape member */
    MEMBER_ATTR_TRAIT(has_shape, shape);

    /* trait to check if the type has a static size */
    template <class T>
    struct len_of {
      static long constexpr value = -1;
    };
  }
}

#endif

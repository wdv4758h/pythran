#ifndef PYTHONIC_INCLUDE_TYPES_LIST_HPP
#define PYTHONIC_INCLUDE_TYPES_LIST_HPP

#include "pythonic/include/types/assignable.hpp"
#include "pythonic/include/types/empty_iterator.hpp"
#include "pythonic/include/types/nditerator.hpp"
#include "pythonic/include/utils/shared_ref.hpp"
#include "pythonic/include/utils/nested_container.hpp"
#include "pythonic/include/utils/int_.hpp"
#include "pythonic/include/utils/reserve.hpp"
#include "pythonic/include/types/tuple.hpp"
#include "pythonic/include/types/slice.hpp"
#include "pythonic/include/types/vectorizable_type.hpp"
#include "pythonic/include/types/nditerator.hpp"

#include <ostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>

PYTHONIC_NS_BEGIN

namespace types
{
  template <class T>
  using container = std::vector<T>;

  static const size_t DEFAULT_LIST_CAPACITY = 16;

  /* forward declaration */
  struct empty_list;
  template <class T>
  class list;
  template <class T, class S>
  class sliced_list;
  template <class T, size_t N>
  struct ndarray;
  template <class T>
  struct is_list {
    static const bool value = false;
  };
  template <class T>
  struct is_list<list<T>> {
    static const bool value = true;
  };
  template <class T, class S>
  struct is_list<sliced_list<T, S>> {
    static const bool value = true;
  };

  /* for type disambiguification */
  struct single_value {
  };

  /* list view */
  template <class T, class S = slice>
  class sliced_list
  {

    // data holder
    typedef
        typename std::remove_cv<typename std::remove_reference<T>::type>::type
            _type;
    typedef container<_type> container_type;
    utils::shared_ref<container_type> data;

    template <class U>
    friend class list;

    typename S::normalized_type slicing;

  public:
    //  types
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;
    typedef nditerator<sliced_list> iterator;
    typedef const_nditerator<sliced_list> const_iterator;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::difference_type difference_type;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::allocator_type allocator_type;
    typedef typename container_type::pointer pointer;
    typedef typename container_type::const_pointer const_pointer;
    typedef typename container_type::reverse_iterator reverse_iterator;
    typedef
        typename container_type::const_reverse_iterator const_reverse_iterator;

    // minimal ndarray interface
    typedef
        typename utils::nested_container_value_type<sliced_list>::type dtype;
    static const size_t value =
        utils::nested_container_depth<sliced_list>::value;
    static const bool is_vectorizable = false; // overly cautious \simeq lazy

    // constructor
    sliced_list();
    sliced_list(sliced_list<T, S> const &s);
    sliced_list(list<T> const &other, S const &s);
    template <class Sn>
    sliced_list(utils::shared_ref<container_type> const &other, Sn const &s);

    // assignment
    sliced_list &operator=(list<T> const &);
    sliced_list &operator=(sliced_list<T, S> const &);
    list<T> operator+(list<T> const &);
    list<T> operator+(sliced_list<T, S> const &);

    // iterators
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    // size
    long size() const;
    operator bool() const;

    // accessor
    T const &fast(long i) const;
    T const &operator[](long i) const;
    T &operator[](long i);
    sliced_list<T, S> operator[](contiguous_slice s) const;
    sliced_list<T, decltype(std::declval<S>() * std::declval<slice>())>
    operator[](slice s) const;

    // comparison
    template <class K>
    bool operator==(list<K> const &other) const;
    bool operator==(empty_list const &other) const;
  };

  /* list */
  template <class T>
  class list
  {

    // data holder
    typedef
        typename std::remove_cv<typename std::remove_reference<T>::type>::type
            _type;
    typedef container<_type> container_type;
    utils::shared_ref<container_type> data;

    template <class U, class S>
    friend class sliced_list;

    template <class U>
    friend class list;

  public:
    // types
    typedef typename container_type::value_type value_type;
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::difference_type difference_type;
    typedef typename container_type::allocator_type allocator_type;
    typedef typename container_type::pointer pointer;
    typedef typename container_type::const_pointer const_pointer;
    typedef typename container_type::reverse_iterator reverse_iterator;
    typedef
        typename container_type::const_reverse_iterator const_reverse_iterator;

    // minimal ndarray interface
    typedef typename utils::nested_container_value_type<list>::type dtype;
    static const size_t value = utils::nested_container_depth<list>::value;
    static const bool is_vectorizable = types::is_vectorizable<dtype>::value;
    static const bool is_strided = false;

    // constructors
    list();
    template <class InputIterator>
    list(InputIterator start, InputIterator stop);
    list(empty_list const &);
    list(size_type sz);
    list(T const &value, single_value);
    list(std::initializer_list<T> l);
    list(list<T> &&other);
    list(list<T> const &other);
    template <class F>
    list(list<F> const &other);
    template <class S>
    list(sliced_list<T, S> const &other);
    list<T> &operator=(list<T> &&other);
    template <class F>
    list<T> &operator=(list<F> const &other);
    list<T> &operator=(list<T> const &other);
    list<T> &operator=(empty_list const &);
    template <class S>
    list<T> &operator=(sliced_list<T, S> const &other);

    list &operator=(ndarray<T, 1> const &); // implemented in ndarray.hpp

    template <class S>
    list<T> &operator+=(sliced_list<T, S> const &other);
    template <class S>
    list<T> operator+(sliced_list<T, S> const &other) const;

    // io
    template <class S>
    friend std::ostream &operator<<(std::ostream &os, list<S> const &v);

    // comparison
    template <class K>
    bool operator==(list<K> const &other) const;
    bool operator==(empty_list const &) const;
    template <class K>
    bool operator!=(list<K> const &other) const;
    bool operator!=(empty_list const &) const;

    // iterators
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;

    // comparison
    int operator<(list<T> const &other) const;

// element access
#ifdef USE_BOOST_SIMD
    using simd_iterator = const_simd_nditerator<list>;
    using simd_iterator_nobroadcast = simd_iterator;
    template <class vectorizer>
    simd_iterator vbegin(vectorizer) const;
    template <class vectorizer>
    simd_iterator vend(vectorizer) const;
#endif
    reference fast(long n);
    reference operator[](long n);

    const_reference fast(long n) const;
    const_reference operator[](long n) const;

    sliced_list<T, slice> operator[](slice const &s) const;
    sliced_list<T, contiguous_slice>
    operator[](contiguous_slice const &s) const;

    // modifiers
    template <class Tp>
    void push_back(Tp &&x);
    template <class Tp>
    void insert(long i, Tp &&x);

    void reserve(size_t n);
    void resize(size_t n);
    iterator erase(size_t n);

    T pop(long x = -1);

    // TODO: have to raise a valueError
    none_type remove(T const &x);

    // Misc
    // TODO: have to raise a valueError
    long index(T const &x) const;

    // list interface
    explicit operator bool() const;

    template <class F>
    list<decltype(std::declval<T>() +
                  std::declval<typename list<F>::value_type>())>
    operator+(list<F> const &s) const;

    template <class F, class S>
    list<decltype(std::declval<T>() +
                  std::declval<typename sliced_list<F, S>::value_type>())>
    operator+(sliced_list<F, S> const &s) const;

    list<T> operator+(empty_list const &) const;
    list<T> operator*(long t) const;

    template <class F>
    list<T> &operator+=(list<F> const &s);
    template <class Tp, size_t N>
    list<T> &operator+=(array<Tp, N> const &s);

    long size() const;
    template <class E>
    long _flat_size(E const &e, utils::int_<1>) const;
    template <class E, size_t L>
    long _flat_size(E const &e, utils::int_<L>) const;
    long flat_size() const;

    template <class V>
    bool contains(V const &v) const;
    intptr_t id() const;

    long count(T const &x) const;
    array<long, value> shape() const
    {
      array<long, value> res;
      details::init_shape(res, *this, utils::int_<value>{});
      return res;
    }
  };

  /* empty list implementation */
  struct empty_list {
    typedef void value_type;
    typedef empty_iterator iterator;
    typedef empty_iterator const_iterator;
    template <class T>
    list<T> operator+(list<T> const &s) const;
    template <class T, class S>
    sliced_list<T, S> operator+(sliced_list<T, S> const &s) const;
    template <class T, size_t N>
    array<T, N> operator+(array<T, N> const &s) const;
    empty_list operator+(empty_list const &) const;
    operator bool() const;
    template <class T>
    operator list<T>() const;
    static constexpr long size();
  };

  std::ostream &operator<<(std::ostream &os, empty_list const &);
}

namespace utils
{
  /**
   * Reserve enough space to save all values generated from f.
   *
   * We use a dummy arguments (p) to reserve only when f have a
   * const_iterator type.
   */
  template <class T, class From>
  void reserve(types::list<T> &l, From const &f,
               typename From::const_iterator *p = nullptr);
}

template <class T>
struct assignable<types::list<T>> {
  typedef types::list<typename assignable<T>::type> type;
};

// to cope with std::vector<bool> specialization
template <>
struct returnable<types::list<bool>::reference> {
  using type = bool;
};
PYTHONIC_NS_END

/* overload std::get */
namespace std
{
  template <size_t I, class T>
  typename pythonic::types::list<T>::reference get(pythonic::types::list<T> &t);

  template <size_t I, class T>
  typename pythonic::types::list<T>::const_reference
  get(pythonic::types::list<T> const &t);

  template <size_t I, class T>
  typename pythonic::types::list<T>::value_type
  get(pythonic::types::list<T> &&t);

  template <size_t I, class T, class S>
  typename pythonic::types::sliced_list<T, S>::reference
  get(pythonic::types::sliced_list<T, S> &t);

  template <size_t I, class T, class S>
  typename pythonic::types::sliced_list<T, S>::const_reference
  get(pythonic::types::sliced_list<T, S> const &t);

  template <size_t I, class T, class S>
  typename pythonic::types::sliced_list<T, S>::value_type
  get(pythonic::types::sliced_list<T, S> &&t);

  template <size_t I, class T>
  struct tuple_element<I, pythonic::types::list<T>> {
    typedef typename pythonic::types::list<T>::value_type type;
  };
  template <size_t I, class T, class S>
  struct tuple_element<I, pythonic::types::sliced_list<T, S>> {
    typedef typename pythonic::types::sliced_list<T, S>::value_type type;
  };
}

/* type inference stuff  {*/
#include "pythonic/include/types/combined.hpp"

template <class A>
struct __combined<container<A>, pythonic::types::empty_list> {
  typedef pythonic::types::list<A> type;
};

template <class A>
struct __combined<pythonic::types::empty_list, container<A>> {
  typedef pythonic::types::list<A> type;
};

template <class A, class B>
struct __combined<container<A>, pythonic::types::list<B>> {
  typedef pythonic::types::list<typename __combined<A, B>::type> type;
};

template <class A, class B>
struct __combined<pythonic::types::list<B>, container<A>> {
  typedef pythonic::types::list<typename __combined<A, B>::type> type;
};

template <class K, class V>
struct __combined<indexable<K>, pythonic::types::list<V>> {
  typedef pythonic::types::list<V> type;
};

template <class V, class K>
struct __combined<pythonic::types::list<V>, indexable<K>> {
  typedef pythonic::types::list<V> type;
};

template <class K, class V0, class V1>
struct __combined<indexable_container<K, V0>, pythonic::types::list<V1>> {
  typedef pythonic::types::list<typename __combined<V0, V1>::type> type;
};

template <class K, class V0, class V1>
struct __combined<pythonic::types::list<V1>, indexable_container<K, V0>> {
  typedef pythonic::types::list<typename __combined<V0, V1>::type> type;
};

template <class K, class V>
struct __combined<indexable_container<K, V>, pythonic::types::empty_list> {
  typedef pythonic::types::list<V> type;
};

template <class K, class V>
struct __combined<pythonic::types::empty_list, indexable_container<K, V>> {
  typedef pythonic::types::list<V> type;
};

template <class T0, class T1>
struct __combined<pythonic::types::list<T0>, pythonic::types::list<T1>> {
  typedef pythonic::types::list<typename __combined<T0, T1>::type> type;
};

template <class T, class S>
struct __combined<pythonic::types::sliced_list<T, S>,
                  pythonic::types::empty_list> {
  typedef pythonic::types::list<T> type;
};
template <class T, class S>
struct __combined<pythonic::types::empty_list,
                  pythonic::types::sliced_list<T, S>> {
  typedef pythonic::types::list<T> type;
};

template <class T, size_t N>
struct __combined<pythonic::types::array<T, N>, pythonic::types::empty_list> {
  typedef pythonic::types::list<T> type;
};
template <class T, size_t N>
struct __combined<pythonic::types::empty_list, pythonic::types::array<T, N>> {
  typedef pythonic::types::list<T> type;
};

/* } */

#ifdef ENABLE_PYTHON_MODULE

PYTHONIC_NS_BEGIN
template <typename T>
struct to_python<types::list<T>> {
  static PyObject *convert(types::list<T> const &v);
};
template <typename T, typename S>
struct to_python<types::sliced_list<T, S>> {
  static PyObject *convert(types::sliced_list<T, S> const &v);
};
template <>
struct to_python<types::empty_list> {
  static PyObject *convert(types::empty_list const &);
};

template <class T>
struct from_python<types::list<T>> {

  static bool is_convertible(PyObject *obj);

  static types::list<T> convert(PyObject *obj);
};
PYTHONIC_NS_END

#endif

#endif

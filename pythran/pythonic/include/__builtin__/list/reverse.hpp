#ifndef PYTHONIC_INCLUDE_BUILTIN_LIST_REVERSE_HPP
#define PYTHONIC_INCLUDE_BUILTIN_LIST_REVERSE_HPP

#include "pythonic/include/types/list.hpp"
#include "pythonic/include/types/NoneType.hpp"
#include "pythonic/include/utils/functor.hpp"

PYTHONIC_NS_BEGIN

namespace __builtin__
{

  namespace list
  {

    template <class T>
    types::none_type reverse(types::list<T> &seq);

    DECLARE_FUNCTOR(pythonic::__builtin__::list, reverse);
  }
}
PYTHONIC_NS_END
#endif

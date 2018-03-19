#pragma once

#include <boost/optional.hpp>

#include "function_traits.h"

namespace functional {

  template <class T>
  class maybe;

  template <class T>
  struct is_maybe {
    static constexpr bool value = false;
  };

  template <class T>
  struct is_maybe<maybe<T>> {
    static constexpr bool value = true;
  };

  template <typename T>
  class maybe {
  public:
    maybe(void) {}
    maybe(T&& t) : _optional(std::move(t)) {}
    maybe(const T& t) : _optional(t) {}
    maybe(maybe<T>&& m) : _optional(std::move(m._optional)) {}
    maybe(const maybe<T>& m) : _optional(m._optional) {}

    template <class F,
              class = typename std::enable_if<is_maybe<
                  typename function_traits<F>::return_type>::value>::type>
    typename function_traits<F>::return_type bind(F&& f) {
      if (*this)
        return f(**this);
      return typename function_traits<F>::return_type();
    }

    // TODO enable_if function_traits<F>::return_type == maybe<K>
    template <class F,
              class = typename std::enable_if<
                  std::is_same<typename function_traits<F>::return_type,
                               void>::value>::type>
    void bind(F&& f) {
      if (*this)
        f(**this);
    }

    template <class F>
    inline typename function_traits<F>::return_type operator>>=(F&& f) {
      return bind(f);
    }

    T& operator*(void) { return *_optional; }

    explicit operator bool(void) { return (bool)_optional; }

  private:
    boost::optional<T> _optional;
  };

  // TODO handle void return type
  template <class F>
  std::function<maybe<typename function_traits<F>::return_type>(
      maybe<typename function_traits<F>::template argument<0>::type>)>
  fmap_maybe(F&& f) {
    return
        [f](maybe<typename function_traits<F>::template argument<0>::type> a) {
          if (!a)
            return maybe<typename function_traits<F>::return_type>();
          return maybe<typename function_traits<F>::return_type>(f(*a));
        };
  }

  template <class T>
  maybe<T> join(maybe<maybe<T> > m) {
    if (m)
      return *m;
    return maybe<T>();
  }

  template <class T>
  maybe<
      typename std::remove_const<typename std::remove_reference<T>::type>::type>
  make_maybe(T&& t) {
    return maybe<typename std::remove_const<
        typename std::remove_reference<T>::type>::type>(std::forward<T>(t));
  }
};

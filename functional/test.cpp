#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE functional
#include <boost/test/unit_test.hpp>

#include "maybe.h"

using functional::maybe;

BOOST_AUTO_TEST_CASE(maybe_test) {
  auto multiply = [](int a) { return a * 5; };
  maybe<int> a;
  maybe<int> b(3);
  maybe<int> c = functional::make_maybe<int>(10);

  BOOST_CHECK_EQUAL((bool)a, false);
  BOOST_CHECK_EQUAL((bool)b, true);
  BOOST_CHECK_EQUAL((bool)c, true);

  BOOST_CHECK_THROW(*a, std::exception);
  BOOST_CHECK_NO_THROW(*b);
  BOOST_CHECK_NO_THROW(*c);

  BOOST_CHECK_EQUAL(*b, 3);
  BOOST_CHECK_EQUAL(*c, 10);

  // BOOST_CHECK_EQUAL(expr, expr)
  // BOOST_CHECK_NO_THROW(expr)
  // BOOST_CHECK_MESSAGE(bool, message)
}

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestXMASComponent
#include <boost/test/unit_test.hpp>
#include "xmas.h"

BOOST_AUTO_TEST_CASE( simple_test )
{
    XMASComponent *comp = new XMASFork("test_fork");
    BOOST_CHECK(comp->getName() == "test_comp");
}

// EOF

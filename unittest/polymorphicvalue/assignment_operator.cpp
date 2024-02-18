#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "nfse.h"

#define LOCAL_CONST 1337 /* an arbitrary rvalue */

using namespace testing;
using namespace NFSE;

TEST(PolymorphicValue_Suite, PMV_Assign_NonClassType)
{
    auto pmv = PolymorphicValue<int>();
    ASSERT_NE(pmv.expose(), nullptr);
    ASSERT_EQ(*pmv.expose(), 0);

    pmv = LOCAL_CONST;
    ASSERT_NE(pmv.expose(), nullptr);
    ASSERT_EQ(*pmv.expose(), LOCAL_CONST);
}

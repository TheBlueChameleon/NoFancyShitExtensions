#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "nfse.h"

using namespace testing;
using namespace NFSE;

struct InstanceCountTrackable
{
    int& instanceCount;

    InstanceCountTrackable(const InstanceCountTrackable& other) :
        instanceCount(other.instanceCount)
    {
        ++instanceCount;
    }

    InstanceCountTrackable(const InstanceCountTrackable&& other) :
        instanceCount(other.instanceCount)
    {
        ++instanceCount;
    }

    InstanceCountTrackable(int& refCount) : instanceCount(refCount)
    {
        ++refCount;
    }

    ~InstanceCountTrackable()
    {
        --instanceCount;
    }
};

TEST(PolymorphicValue_Suite, PMV_DefaultCTor_Test)
{
    PolymorphicValue<int> pmv;
    ASSERT_NE(pmv.expose(), nullptr);
    EXPECT_EQ(*pmv.expose(), 0);
}

TEST(PolymorphicValue_Suite, PMV_CopyCTor_Test)
{
    int value = 1337;

    auto defPmv = PolymorphicValue(value);
    ASSERT_NE(defPmv.expose(), nullptr);
    EXPECT_NE(defPmv.expose(), &value);
    EXPECT_EQ(*defPmv.expose(), value);

    int instanceCount = 0;
    {
        auto nonDefaultConstructible = InstanceCountTrackable(instanceCount);
        {
            auto ndPmv = PolymorphicValue(nonDefaultConstructible);
            ASSERT_NE(defPmv.expose(), nullptr);
            EXPECT_NE(ndPmv.expose(), &nonDefaultConstructible);
            EXPECT_EQ(ndPmv.expose()->instanceCount, nonDefaultConstructible.instanceCount);
            EXPECT_EQ(instanceCount, 2);
        }
        EXPECT_EQ(instanceCount, 1);
    }
    EXPECT_EQ(instanceCount, 0);
}

TEST(PolymorphicValue_Suite, PMV_MoveCTor_Test)
{
#define LOCAL_CONST 1337
    auto defPmv = PolymorphicValue(LOCAL_CONST);
    ASSERT_NE(defPmv.expose(), nullptr);
    EXPECT_EQ(*defPmv.expose(), LOCAL_CONST);
#undef LOCAL_CONST

    int instanceCount = 0;
    {
        auto ndPmv = PolymorphicValue(InstanceCountTrackable(instanceCount));
        ASSERT_NE(ndPmv.expose(), nullptr);
        EXPECT_EQ(ndPmv.expose()->instanceCount, 1);
        EXPECT_EQ(instanceCount, 1);
    }
    ASSERT_EQ(instanceCount, 0);
}

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "nfse.h"

#define LOCAL_CONST 1337 /* an arbitrary rvalue */

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

struct CTorInvocationTrackable
{
    int& instanceCount;

    CTorInvocationTrackable(const CTorInvocationTrackable& other) :
        instanceCount(other.instanceCount)
    {
        ++instanceCount;
    }

    CTorInvocationTrackable(const CTorInvocationTrackable&& other) :
        instanceCount(other.instanceCount)
    {
        ++instanceCount;
    }

    CTorInvocationTrackable(int& refCount) : instanceCount(refCount)
    {
        ++refCount;
    }
};

TEST(PolymorphicValue_Suite, PMV_DefaultCTor_Test)
{
    PolymorphicValue<int> pmv;
    ASSERT_NE(pmv.expose(), nullptr);
    EXPECT_EQ(*pmv.expose(), 0);
}

TEST(PolymorphicValue_Suite, PMV_CopyValueCTor_Test)
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

TEST(PolymorphicValue_Suite, PMV_MoveValueCTor_Test)
{
    auto defPmv = PolymorphicValue(LOCAL_CONST);
    ASSERT_NE(defPmv.expose(), nullptr);
    EXPECT_EQ(*defPmv.expose(), LOCAL_CONST);

    int instanceCount = 0;
    {
        auto ndPmv = PolymorphicValue(InstanceCountTrackable(instanceCount));
        ASSERT_NE(ndPmv.expose(), nullptr);
        EXPECT_EQ(ndPmv.expose()->instanceCount, 1);
        EXPECT_EQ(instanceCount, 1);
    }
    ASSERT_EQ(instanceCount, 0);
}

TEST(PolymorphicValue_Suite, PMV_EmplaceCTor_Test)
{
    int instanceCount = 0;
    auto pmv = PolymorphicValue<CTorInvocationTrackable>::makeFrom(instanceCount);
    ASSERT_NE(pmv.expose(), nullptr);
    EXPECT_EQ(pmv.expose()->instanceCount, 1);

    instanceCount = 0;
    auto copyPmv = PolymorphicValue(CTorInvocationTrackable(instanceCount));
    ASSERT_NE(copyPmv.expose(), nullptr);
    EXPECT_EQ(copyPmv.expose()->instanceCount, 2);
}

TEST(PolymorphicValue_Suite, PMV_CopyPmvCTor_NonClassType_Test)
{
    auto pmvA = PolymorphicValue(LOCAL_CONST);
    auto pmvB = PolymorphicValue(pmvA);

    ASSERT_NE(pmvA.expose(), nullptr);
    ASSERT_NE(pmvB.expose(), nullptr);
    EXPECT_NE(pmvA.expose(), pmvB.expose());
    EXPECT_EQ(*pmvA.expose(), *pmvB.expose());
    EXPECT_EQ(*pmvA.expose(), LOCAL_CONST);
}

TEST(PolymorphicValue_Suite, PMV_CopyPmvCTor_ClassType_Test)
{
    int instanceCount = 0;
    auto pmvA = PolymorphicValue(InstanceCountTrackable(instanceCount));
    auto pmvB = PolymorphicValue(pmvA);

    ASSERT_NE(pmvA.expose(), nullptr);
    ASSERT_NE(pmvB.expose(), nullptr);
    EXPECT_NE(pmvA.expose(), pmvB.expose());
    EXPECT_EQ(pmvA.expose()->instanceCount, pmvB.expose()->instanceCount);
    EXPECT_EQ(instanceCount, 2);
}

TEST(PolymorphicValue_Suite, PMV_MovePmvCTor_NonClassType_Test)
{
    std::cout << "### pre A" << std::endl;
    auto pmvA = PolymorphicValue(LOCAL_CONST);
    std::cout << "### pre B" << std::endl;
    auto pmvB = PolymorphicValue(std::move(pmvA));
    std::cout << "### post" << std::endl;

    ASSERT_EQ(pmvA.expose(), nullptr);
    ASSERT_NE(pmvB.expose(), nullptr);
    EXPECT_EQ(*pmvB.expose(), LOCAL_CONST);
}

struct Base
{
    virtual ~Base() {}
};

struct Derived : public Base
{
    Derived& operator=(const Base&)
    {
        return *this;
    };
};

TEST(PolymorphicValue_Suite, PMV_CopyPmvCTor_UnrelatedType_Test)
{
    auto pmvBase = PolymorphicValue(Base());
    auto pmvDerived = PolymorphicValue<Base>(Derived());

    ASSERT_EQ(typeid(pmvBase), typeid(pmvDerived));
    EXPECT_NE(pmvDerived.expose(), nullptr);
    //EXPECT_NE(dynamic_cast<Derived*>(pmvDerived.expose()), nullptr);

    auto pmvInt = PolymorphicValue(LOCAL_CONST);

    // TODO: some of this SHOULD work
    // todo: make this a compile-success test
    auto pmvTrueDerived = PolymorphicValue(Derived());
    auto pmvInitWithBase = PolymorphicValue<Base>();
    //    pmvInitWithBase = pmvTrueDerived;
    //    pmvTrueDerived = pmvInitWithBase;
    //    EXPECT_NE(pmvInitWithBase.expose(), nullptr);
    //    EXPECT_NE(dynamic_cast<Derived*>(pmvInitWithBase.expose()), nullptr);

    // todo: make this a compile-fail test
    // auto pmvInvalidCopy = PolymorphicValue<Base>(pmvInt);

    // pmvBase = Derived();
}

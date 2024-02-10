#include "polymorphicvalue.h"

namespace NFSE
{

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::PolymorphicValue(T* init) :
        value(init)
    {}

    template<typename T>
    PolymorphicValue<T, true>::PolymorphicValue() :
        PolymorphicValue<T, false>(new T())
    {}

    template<typename T>
    PolymorphicValue<T, true>::PolymorphicValue(const T& init) :
        PolymorphicValue<T, false>(init)
    {}

    template<typename T>
    PolymorphicValue<T, true>::PolymorphicValue(const T&& init) :
        PolymorphicValue<T, false>(std::move(init))
    {}

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::PolymorphicValue(const T& init) :
        PolymorphicValue<T, false>(new T(init))
    {}

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::PolymorphicValue(const T&& init) :
        PolymorphicValue<T, false>(new T(std::move(init)))
    {}

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::~PolymorphicValue()
    {
        delete value;
    }

    template<typename T, bool enableDefaultCTor>
    T* const PolymorphicValue<T, enableDefaultCTor>::expose() const
    {
        return this->value;
    }
}

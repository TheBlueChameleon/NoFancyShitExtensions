#ifndef POLYMORPHICVALUE_H
#define POLYMORPHICVALUE_H

#include <type_traits>
#include <utility>

namespace NFSE
{
    template<typename T, bool enableDefaultCTor = std::is_default_constructible<T>::value>
    class PolymorphicValue
    {
        private:
            T* value;

        protected:
            PolymorphicValue(T* init);

        public:
            ~PolymorphicValue();
            PolymorphicValue(const T& init);
            PolymorphicValue(const T&& init);

            T* const expose() const;
    };

    template<typename T>
    class PolymorphicValue<T,true> : public PolymorphicValue<T,false>
    {
        public:
            PolymorphicValue();
            PolymorphicValue(const T& init);
            PolymorphicValue(const T&& init);
    };
}

#include "polymorphicvalue.txx"

#endif // POLYMORPHICVALUE_H

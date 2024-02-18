#ifndef POLYMORPHICVALUE_H
#define POLYMORPHICVALUE_H

#include <type_traits>

#include "../base/nfseerror.h"

namespace NFSE
{
    template<typename T, bool enableDefaultCTor = std::is_default_constructible<T>::value>
    class PolymorphicValue
    {
        public:
            using value_type = std::remove_cvref<T>;

        protected:
            T* value = nullptr;

            PolymorphicValue(T* const init);

            T* emitCopy() const;

        public:
            // .............................................................. //
            // CTORs, DTOR

            PolymorphicValue(const PolymorphicValue& other);
            PolymorphicValue(PolymorphicValue&& other);

            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue(const PolymorphicValue<U>& other);
            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue(PolymorphicValue<U>&& other);

            PolymorphicValue(const T& init);
            PolymorphicValue(T&& init);

            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue(const U& init);
            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue(U&& init);

            virtual ~PolymorphicValue();

            // .............................................................. //
            // assignment operators

            PolymorphicValue<T>& operator=(const PolymorphicValue& other);
            PolymorphicValue<T>& operator=(PolymorphicValue&& other);

            // copy assignment
            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue<T>& operator=(const PolymorphicValue<U>& other);

            // move assignment
            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue<T>& operator=(const PolymorphicValue<U>&& other);

            // copy/construct assignment
            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue<T>& operator=(const U& other);

            // move/construct assignment
            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue<T>& operator=(const U&& other);

            // .............................................................. //
            // other methods

            template<class ... Args>
            static PolymorphicValue makeFrom(Args&& ... args);

            T* const expose() const;

            // get, getAs<? extends T>
            // cast to underlying
    };

    template<typename T>
    class PolymorphicValue<T,true> : public PolymorphicValue<T,false>
    {
        public:
            PolymorphicValue();

            PolymorphicValue(const PolymorphicValue& other);
            PolymorphicValue(PolymorphicValue&& other);

            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue(const PolymorphicValue<U>& other);
            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue(PolymorphicValue<U>&& other);

            PolymorphicValue(const T& init);
            PolymorphicValue(T&& init);

            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue(const U& init);
            template <typename U> requires std::derived_from<U, T>
            PolymorphicValue(U&& init);

            // .............................................................. //
            // assignment operators

            PolymorphicValue<T>& operator=(const PolymorphicValue& other);
            PolymorphicValue<T>& operator=(PolymorphicValue&& other);
    };

}

#include "polymorphicvalue.txx"

#endif // POLYMORPHICVALUE_H

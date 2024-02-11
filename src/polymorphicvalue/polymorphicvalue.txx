#include "polymorphicvalue.h"

#include <iostream>

namespace NFSE
{
    template<typename U>
    void checkAssignmentAllowedOrThrow(const U* const src)
    {
        if (src == nullptr)
        {
            throw NfseError("Attempting to copy from null source");
        }
    }

    template<typename T, bool enableDefaultCTor>
    T* PolymorphicValue<T, enableDefaultCTor>::emitCopy() const
    {
        return new T(*value);
    }

    // ====================================================================== //
    // CTors, DTor

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::PolymorphicValue(T* const init) :
        value(init)
    {
        std::cout << "inner CTor" << std::endl;
    }

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::PolymorphicValue(const PolymorphicValue& other)
    {
        std::cout << "Copy CTor" << std::endl;
        checkAssignmentAllowedOrThrow(other.value);
        this->value = other.emitCopy();
    }

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::PolymorphicValue(PolymorphicValue&& other)
    {
        std::cout << "Move CTor" << std::endl;
        this->value = other.value;
        other.value = nullptr;
    }

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::PolymorphicValue(const T& init) :
        PolymorphicValue<T, false>(new T(init))
    {
        std::cout << "copy value CTor" << std::endl;
    }

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::PolymorphicValue(T&& init) :
        PolymorphicValue<T, false>(new T(std::move(init)))
    {
        std::cout << "move value CTor" << std::endl;
    }

    // ---------------------------------------------------------------------- //
    // DTor

    template<typename T, bool enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::~PolymorphicValue()
    {
        std::cout << "DTor" << std::endl;
        if (value)
        {
            delete value;
        }
    }

    // ---------------------------------------------------------------------- //
    // enableDefaultCTor=true delegators

    template<typename T>
    PolymorphicValue<T, true>::PolymorphicValue() :
        PolymorphicValue<T, false>(new T())
    {
        std::cout << "default CTor" << std::endl;
    }

    template<typename T>
    PolymorphicValue<T, true>::PolymorphicValue(const T& init) :
        PolymorphicValue<T, false>(init)
    {
        std::cout << "copy value CTor T" << std::endl;
    }

    template<typename T>
    PolymorphicValue<T, true>::PolymorphicValue(const T&& init) :
        PolymorphicValue<T, false>(std::move(init))
    {
        std::cout << "move value CTor T" << std::endl;
    }

    template<typename T>
    PolymorphicValue<T, true>::PolymorphicValue(const PolymorphicValue& other) :
        PolymorphicValue<T, false>(other)
    {
        std::cout << "Copy CTor T" << std::endl;
    }

    template<typename T>
    PolymorphicValue<T, true>::PolymorphicValue(PolymorphicValue&& other) :
        PolymorphicValue<T, false>(std::move(other))
    {
        std::cout << "Move CTor T" << std::endl;
    }

    // ====================================================================== //
    // Methods

    template<typename T, bool enableDefaultCTor>
    template<class ... Args>
    PolymorphicValue<T, enableDefaultCTor>
    PolymorphicValue<T, enableDefaultCTor>::makeFrom(Args&& ... args)
    {
        std::cout << "emplace CTor" << std::endl;
        return PolymorphicValue<T, enableDefaultCTor>(new T(args ...));
    }

    template<typename T, bool enableDefaultCTor>
    T* const PolymorphicValue<T, enableDefaultCTor>::expose() const
    {
        return this->value;
    }

    template<typename T, bool enableDefaultCTor>
    template<typename U> requires std::derived_from<U, T>
    PolymorphicValue<T>&
    PolymorphicValue<T, enableDefaultCTor>::operator=(const PolymorphicValue<U>& other)
    {
        std::cout << "copy assignment" << std::endl;

        checkAssignmentAllowedOrThrow(other.value);
        auto& u = *(other.value);
        return (*this = u);
    }

    template<typename T, bool enableDefaultCTor>
    template<typename U> requires std::derived_from<U, T>
    PolymorphicValue<T>&
    PolymorphicValue<T, enableDefaultCTor>::operator=(const PolymorphicValue<U>&& other)
    {
        std::cout << "move assignment" << std::endl;

        checkAssignmentAllowedOrThrow(other.value);
        auto& u = *(other.value);

        if (value)
        {
            if (typeid(*value) == typeid(u))
            {
                delete value;
                this->value = other.value;
                other.value = nullptr;
            }
            else
            {
                return (*this = std::move(u));
            }
        }
        else
        {
            this->value = other.value;
            other.value = nullptr;
        }

        return *this;
    }

    template<typename T, bool enableDefaultCTor>
    template<typename U> requires std::derived_from<U, T>
    PolymorphicValue<T>&
    PolymorphicValue<T, enableDefaultCTor>::operator=(const U& other)
    {
        std::cout << "copy value assignment" << std::endl;

        if (value)
        {
            auto& currentValue = *(this->value);

            if (typeid(currentValue) == typeid(other))
            {
                currentValue = other;
            }
            else
            {
                delete this->value;
                this->value = new T(other);
            }
        }
        else
        {
            this->value = new T(other);
        }

        return *this;
    }

    template<typename T, bool enableDefaultCTor>
    template<typename U> requires std::derived_from<U, T>
    PolymorphicValue<T>&
    PolymorphicValue<T, enableDefaultCTor>::operator=(const U&& other)
    {
        std::cout << "move value assignment" << std::endl;

        if (value)
        {
            auto& currentValue = *(this->value);

            if (typeid(currentValue) == typeid(other))
            {
                currentValue = std::move(other);
            }
            else
            {
                delete this->value;
                this->value = new T(std::move(other));
            }
        }
        else
        {
            this->value = new T(std::move(other));
        }

        return *this;
    }
}

#pragma once

#include "bmcl/Config.h"
#include "bmcl/AlignedUnion.h"

namespace bmcl {

template <typename K, typename T, K kind>
struct VariantElementDesc {
    static constexpr K kindValue = kind;
    using KindType = K;
    using Type = T;
};

template <typename K, K emptyValue, typename... Ds>
class Variant {
public:
    template <typename P, typename... Ps>
    struct UnionFromDesc
        : public AlignedUnion<typename P::Type, UnionFromDesc<Ps...>> {};

    template <typename P>
    struct UnionFromDesc<P> {
    private:
        using Type = typename P::Type;
        Type _data;
    };

    struct Union : public UnionFromDesc<Ds...> {
        template <typename C>
        C* as()
        {
            return reinterpret_cast<C*>(this);
        }

        template <typename C>
        const C* as() const
        {
            return reinterpret_cast<const C*>(this);
        }
    };

    template <typename T, typename... Ps>
    struct ContainsElementType
        : std::true_type {};

    template <typename T>
    struct ContainsElementType<T>
        : std::false_type {};

    template <typename T, typename P, typename... Ps>
    struct ContainsElementType<T, P, Ps...>
        : std::conditional<std::is_same<T, typename P::Type>::value, std::true_type, ContainsElementType<T, Ps...>>::type {};

    template <typename T, typename P, typename... Ps>
    struct InferKindFromType
        : std::conditional<std::is_same<T, typename P::Type>::value, P, InferKindFromType<T, Ps...>>::type {};

    template <typename T, typename P>
    struct InferKindFromType<T, P>
        : std::conditional<std::is_same<T, typename P::Type>::value, P, void>::type {};

    template <template <typename> class A, typename P, typename... Ps>
    struct GenericHelper {
        template <typename... As>
        void operator()(const K& kind, As&&... args)
        {
            if (kind == P::kindValue) {
                using C = typename P::Type;
                A<C>()(std::forward<As>(args)...);
                return;
            }
            GenericHelper<A, Ps...>()(kind, std::forward<As>(args)...);
        }
    };

    template <template <typename> class A, typename P>
    struct GenericHelper<A, P> {
        template <typename... As>
        void operator()(const K&, As&&... args)
        {
            using C = typename P::Type;
            A<C>().operator()(std::forward<As>(args)...);
        }
    };

    template <typename C>
    struct Destructor {
        void operator()(Union* data)
        {
            reinterpret_cast<C*>(data)->~C();
        }
    };

    template <typename C>
    struct Constructor {
        void operator()(const Union* from, Union* to)
        {
            new (reinterpret_cast<C*>(to)) C(*reinterpret_cast<const C*>(from));
        }
    };

    template <typename C>
    struct MoveConstructor {
        void operator()(Union* from, Union* to)
        {
            new (reinterpret_cast<C*>(to)) C(std::move(*reinterpret_cast<C*>(from)));
        }
    };

    template <typename C>
    struct Comparator {
        void operator()(const Union* left, const Union* right, bool* dest)
        {
            *dest = (*reinterpret_cast<const C*>(left) == *reinterpret_cast<const C*>(right));
        }
    };

    using DestructHelper = GenericHelper<Destructor, Ds...>;
    using ConstuctHelper = GenericHelper<Constructor, Ds...>;
    using MoveConstructHelper = GenericHelper<MoveConstructor, Ds...>;
    using CompareHelper = GenericHelper<Comparator, Ds...>;

//     template <typename T, typename P, typename... Ps>
//     struct DestructHelper {
//         void operator()(const K& kind, T* data)
//         {
//             if (kind == P::kindValue) {
//                 using C = typename P::Type;
//                 reinterpret_cast<C*>(data)->~C();
//                 return;
//             }
//             DestructHelper<T, Ps...>()(kind, data);
//         }
//     };
//
//     template <typename T, typename P>
//     struct DestructHelper<T, P> {
//         void operator()(const K&, T* data)
//         {
//             using C = typename P::Type;
//             reinterpret_cast<C*>(data)->~C();
//         }
//     };

    template<typename R, typename S = R>
    using enableIfAllowed = typename std::enable_if<ContainsElementType<R, Ds...>::value, S>::type;

    Variant()
        : _kind(emptyValue)
    {
    }

    template <typename T>
    Variant(const T& value, const enableIfAllowed<T>* helper = nullptr)
        : _kind(InferKindFromType<T, Ds...>::kindValue)
    {
        (void)helper;
        new (asPtr<T>()) T(value);
    }

    template <typename T>
    Variant(T&& value, const enableIfAllowed<T>* helper = nullptr)
        : _kind(InferKindFromType<T, Ds...>::kindValue)
    {
        (void)helper;
        new (asPtr<T>()) T(std::move(value));
    }

    Variant(const Variant& other)
        : _kind(other._kind)
    {
        if (other._kind == emptyValue) {
            return;
        }
        ConstuctHelper()(other._kind, &other._data, &_data);
    }

    Variant(Variant&& other)
        : _kind(other._kind)
    {
        if (other._kind == emptyValue) {
            return;
        }
        MoveConstructHelper()(other._kind, &other._data, &_data);
        other.destruct();
        other._kind = emptyValue;
    }

    ~Variant()
    {
        destruct();
    }

    void reset()
    {
        destruct();
        _kind = emptyValue;
    }

    const K& kind() const
    {
        return _kind;
    }

    bool isA(const K& kind) const
    {
        return _kind == kind;
    }

    bool isEmpty() const
    {
        return _kind == emptyValue;
    }

    template <typename T>
    enableIfAllowed<T, const T&> as() const
    {
        return *asPtr<T>();
    }

    template <typename T>
    enableIfAllowed<T, T&> as()
    {
        return *asPtr<T>();
    }

    template <typename T>
    enableIfAllowed<T, Variant&> operator=(const T& value)
    {
        destruct();
        _kind = InferKindFromType<T, Ds...>::kindValue;
        new (asPtr<T>()) T(value);
        return *this;
    }

    template <typename T>
    enableIfAllowed<T, Variant&> operator=(T&& value)
    {
        destruct();
        _kind = InferKindFromType<T, Ds...>::kindValue;
        new (asPtr<T>()) T(std::move(value));
        return *this;
    }

    Variant& operator=(const Variant& other)
    {
        destruct();
        if (other._kind == emptyValue) {
            _kind = emptyValue;
            return *this;
        }

        _kind = other._kind;
        ConstuctHelper()(other._kind, &other._data, &_data);
        return *this;
    }

    Variant& operator=(Variant&& other)
    {
        destruct();
        if (other._kind == emptyValue) {
            _kind = emptyValue;
            return *this;
        }

        _kind = other._kind;
        MoveConstructHelper()(other._kind, &other._data, &_data);
        other.destruct();
        other._kind = emptyValue;
        return *this;
    }

    template <typename T>
    enableIfAllowed<T, bool> equals(const T& value) const
    {
        if (_kind == InferKindFromType<T, Ds...>::kindValue) {
            return *asPtr<T>() == value;
        }
        return false;
    }

    bool equals(const Variant& other) const
    {
        if (_kind == other._kind) {
            if (_kind == emptyValue) {
                return true;
            }
            bool result;
            CompareHelper()(_kind, &_data, &other._data, &result); //TODO: rewrite
            return result;
        }
        return false;
    }

    bool operator==(const Variant& other) const
    {
        return equals(other);
    }

    bool operator!=(const Variant& other) const
    {
        return !equals(other);
    }

private:
    void destruct()
    {
        if (_kind == emptyValue) {
            return;
        }
        DestructHelper()(_kind, &_data);
    }

    template <typename T>
    T* asPtr()
    {
        return reinterpret_cast<T*>(&_data);
    }

    template <typename T>
    const T* asPtr() const
    {
        return reinterpret_cast<const T*>(&_data);
    }

    Union _data;
    K _kind;
};

template <typename T, typename K, K emptyValue, typename... Ds>
bool operator==(const Variant<K, emptyValue, Ds...>& var, const T& value)
{
    return var.equals(value);
}

template <typename T, typename K, K emptyValue, typename... Ds>
bool operator==(const T& value, const Variant<K, emptyValue, Ds...>& var)
{
    return var.equals(value);
}

template <typename T, typename K, K emptyValue, typename... Ds>
bool operator!=(const Variant<K, emptyValue, Ds...>& var, const T& value)
{
    return !var.equals(value);
}

template <typename T, typename K, K emptyValue, typename... Ds>
bool operator!=(const T& value, const Variant<K, emptyValue, Ds...>& var)
{
    return !var.equals(value);
}
}

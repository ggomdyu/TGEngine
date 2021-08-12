#pragma once

#include <array>
#include <type_traits>

#include "TypeTraits.h"

namespace tg
{

template <typename>
class Delegate;

namespace detail
{

template <typename _Return, typename... _Types>
class Functor
{
/**@section Constructor */
public:
    Functor() = default;
    Functor(const Functor& rhs) = delete;
    Functor(Functor&& rhs) = delete;

/**@section Destructor */
public:
    virtual ~Functor() = default;

/**@section Operator */
public:
    Functor& operator=(const Functor& rhs) = delete;
    Functor& operator=(Functor&& rhs) = delete;

/**@section Method */
public:
    virtual _Return Invoke(_Types... args) const = 0;
    virtual void CopyTo(Functor* functor) const = 0;
    virtual void Destroy() = 0;
    [[nodiscard]] virtual size_t GetSize() const noexcept = 0;
};

template <typename _Function, typename _Return, typename... _Types>
class FunctorImpl final :
    public Functor<_Return, _Types...>
{
/**@section Type */
public:
    using Storage = std::conditional_t<FunctionTraits<_Function>::IsMemberFunction,
        std::tuple<_Function, void*>,
        std::tuple<_Function>
    >;

/**@section Constructor */
public:
    explicit FunctorImpl(Storage storage) noexcept;

/**@section Method */
public:
    _Return Invoke(_Types... args) const override;
    void CopyTo(Functor<_Return, _Types...>* functor) const override;
    void Destroy() override;
    [[nodiscard]] size_t GetSize() const noexcept override;

/**@section Variable */
private:
    Storage m_storage;
};

template <typename _Function, typename _Return, typename... _Types>
FunctorImpl<_Function, _Return, _Types...>::FunctorImpl(Storage storage) noexcept :
    m_storage(std::move(storage))
{
}

template <typename _Function, typename _Return, typename... _Types>
void FunctorImpl<_Function, _Return, _Types...>::CopyTo(Functor<_Return, _Types...>* functor) const
{
    new (functor) FunctorImpl<_Function, _Return, _Types...>(m_storage);
}

template <typename _Function, typename _Return, typename... _Types>
_Return FunctorImpl<_Function, _Return, _Types...>::Invoke(_Types... args) const
{
    if constexpr (FunctionTraits<_Function>::IsMemberFunction)
    {
        auto& [function, receiver] = m_storage;
        return (reinterpret_cast<typename FunctionTraits<_Function>::ClassType*>(receiver)->*(function))(std::move(args)...);
    }
    else
    {
        return std::get<0>(m_storage)(std::move(args)...);
    }
}

template <typename _Function, typename _Return, typename... _Types>
void FunctorImpl<_Function, _Return, _Types...>::Destroy()
{
    if constexpr (FunctionTraits<_Function>::IsFunctor)
    {
        std::get<0>(m_storage).~_Function();
    }
}

template <typename _Function, typename _Return, typename... _Types>
size_t FunctorImpl<_Function, _Return, _Types...>::GetSize() const noexcept
{
    return sizeof(*this);
}

}

template <typename>
constexpr bool IsDelegate = std::false_type::value;

template <typename _Type>
constexpr bool IsDelegate<Delegate<_Type>> = std::true_type::value;

template <typename _Return, typename... _Types>
class Delegate<_Return(_Types...)> final
{
/**@section Type */
public:
    using ReturnType = _Return;

private:
    template <typename _Function>
    using FunctorImpl = detail::FunctorImpl<_Function, _Return, _Types...>;
    using Functor = detail::Functor<_Return, _Types...>;

/**@section Constructor */
public:
    constexpr Delegate() noexcept = default;
    constexpr Delegate(std::nullptr_t) noexcept;
    Delegate(const Delegate& rhs);
    Delegate(Delegate&& rhs) noexcept;
    template <typename _Function>
    Delegate(_Function function);
    template <typename _Function>
    Delegate(_Function function, void* receiver);

/**@section Destructor */
public:
    ~Delegate();

/**@section Operator */
public:
    template <typename... _Args2>
    _Return operator()(_Args2&&... args) const;
    Delegate& operator=(const Delegate& rhs);
    Delegate& operator=(Delegate&& rhs) noexcept;
    constexpr bool operator==(std::nullptr_t rhs) const noexcept;
    constexpr bool operator!=(std::nullptr_t rhs) const noexcept;

/**@section Method */
private:
    template <typename _Function>
    [[nodiscard]] static constexpr bool IsLargeFunction() noexcept;
    [[nodiscard]] bool IsDynamicAllocated() const noexcept;
    void Copy(const Delegate& rhs);
    void Move(Delegate&& rhs) noexcept;
    void Destroy();

/**@section Variable */
private:
    static constexpr size_t StorageCapacity = 7;

    Functor* m_functor{};
    std::array<void*, StorageCapacity> m_storage{};
};

template <typename _Function>
Delegate(_Function function) -> Delegate<typename FunctionTraits<std::remove_cvref_t<_Function>>::FunctionType>;

template <typename _Function>
Delegate(_Function function, void* receiver) -> Delegate<typename FunctionTraits<std::remove_cvref_t<_Function>>::FunctionType>;

template <typename _Return, typename... _Types>
constexpr Delegate<_Return(_Types...)>::Delegate(std::nullptr_t) noexcept :
    Delegate()
{
}

template <typename _Return, typename... _Types>
Delegate<_Return(_Types...)>::Delegate(const Delegate& rhs) :
    Delegate()
{
    this->Copy(rhs);
}

template <typename _Return, typename... _Types>
Delegate<_Return(_Types...)>::Delegate(Delegate&& rhs) noexcept :
    Delegate()
{
    this->Move(std::move(rhs));
}

template <typename _Return, typename... _Types>
template <typename _Function>
Delegate<_Return(_Types...)>::Delegate(_Function function) :
    Delegate()
{
    using FunctionType = std::decay_t<_Function>;
    if constexpr (Delegate::template IsLargeFunction<FunctionType>())
    {
        m_functor = static_cast<Functor*>(operator new(sizeof(FunctorImpl<FunctionType>)));
    }
    else
    {
        m_functor = reinterpret_cast<Functor*>(&m_storage[0]);
    }

    new (m_functor) FunctorImpl<FunctionType>(typename FunctorImpl<FunctionType>::Storage{std::move(function)});
}

template <typename _Return, typename... _Types>
template <typename _Function>
Delegate<_Return(_Types...)>::Delegate(_Function function, void* receiver) :
    Delegate()
{
    using FunctionType = std::decay_t<_Function>;
    if constexpr (Delegate::template IsLargeFunction<FunctionType>())
    {
        m_functor = static_cast<Functor*>(operator new(sizeof(FunctorImpl<FunctionType>)));
    }
    else
    {
        m_functor = reinterpret_cast<Functor*>(&m_storage[0]);
    }

    new (m_functor) FunctorImpl<FunctionType>(typename FunctorImpl<FunctionType>::Storage{std::move(function), receiver});
}

template <typename _Return, typename... _Types>
Delegate<_Return(_Types...)>::~Delegate()
{
    this->Destroy();
}

template <typename _Return, typename... _Types>
template <typename... _Args2>
_Return Delegate<_Return(_Types...)>::operator()(_Args2&&... args) const
{
    if (m_functor == nullptr)
    {
        return _Return();
    }

    return m_functor->Invoke(std::forward<_Args2>(args)...);
}

template <typename _Return, typename... _Types>
template <typename _Function>
constexpr bool Delegate<_Return(_Types...)>::IsLargeFunction() noexcept
{
    return sizeof(FunctorImpl<_Function>) > (sizeof(m_storage) - sizeof(m_storage[0]));
}

template <typename _Return, typename... _Types>
Delegate<_Return(_Types...)>& Delegate<_Return(_Types...)>::operator=(const Delegate& rhs)
{
    if (this != &rhs)
    {
        this->Copy(rhs);
    }

    return *this;
}

template <typename _Return, typename... _Types>
Delegate<_Return(_Types...)>& Delegate<_Return(_Types...)>::operator=(Delegate&& rhs) noexcept
{
    if (this != &rhs)
    {
        this->Move(std::move(rhs));
    }

    return *this;
}

template <typename _Return, typename... _Types>
constexpr bool Delegate<_Return(_Types...)>::operator==(std::nullptr_t rhs) const noexcept
{
    return m_functor == rhs;
}

template <typename _Return, typename... _Types>
constexpr bool Delegate<_Return(_Types...)>::operator!=(std::nullptr_t rhs) const noexcept
{
    return m_functor != rhs;
}

template <typename _Return, typename... _Types>
bool Delegate<_Return(_Types...)>::IsDynamicAllocated() const noexcept
{
    return m_functor != reinterpret_cast<const Functor*>(&m_storage[0]);
}

template <typename _Return, typename... _Types>
void Delegate<_Return(_Types...)>::Copy(const Delegate& rhs)
{
    this->Destroy();

    if (rhs.m_functor == nullptr)
    {
        return;
    }

    m_functor = static_cast<Functor*>(rhs.IsDynamicAllocated() ? operator new(rhs.m_functor->GetSize()) : &m_storage[0]);

    rhs.m_functor->CopyTo(m_functor);
}

template <typename _Return, typename... _Types>
void Delegate<_Return(_Types...)>::Move(Delegate&& rhs) noexcept
{
    if (IsDynamicAllocated() && rhs.IsDynamicAllocated())
    {
        std::swap(m_functor, rhs.m_functor);
    }
    else if (rhs.IsDynamicAllocated())
    {
        m_functor = rhs.m_functor;
        rhs.m_functor = nullptr;
    }
    else
    {
        if (IsDynamicAllocated())
        {
            std::swap(m_functor, rhs.m_functor);
        }

        m_storage = rhs.m_storage;
        m_functor = reinterpret_cast<Functor*>(&m_storage[0]);
    }
}

template <typename _Return, typename... _Types>
void Delegate<_Return(_Types...)>::Destroy()
{
    if (m_functor == nullptr)
    {
        return;
    }

    m_functor->Destroy();

    if (this->IsDynamicAllocated())
    {
        operator delete(m_functor);
    }

    m_storage = {};
}

}
#pragma once

namespace tg
{

template <typename _Type>
class MakeSharedEnabler final :
    public _Type
{
/**@section Constructor */
public:
    template <typename... _Types>
    explicit MakeSharedEnabler(_Types&&... args) noexcept;
};

template <typename _Type>
template <typename... _Types>
MakeSharedEnabler<_Type>::MakeSharedEnabler(_Types&&... args) noexcept :
    _Type(std::forward<_Types>(args)...)
{
}

}

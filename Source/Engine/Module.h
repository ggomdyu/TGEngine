#pragma once

#include "Core/Concepts.h"
#include "Core/RuntimeObject.h"

namespace tg
{

enum class ModuleStage
{
    Idle,
    Update,
    Render,
};

class Module :
    public RuntimeObject
{
public:
    TGON_RTTI(Module)

/**@section Method */
public:
    /**
     * @brief   Initializes the object.
     */
    virtual void Initialize() {}

    /**
     * @brief   Updates the frame of the object.
     */
    virtual void Update() {}
};


template <typename _Type, typename = std::void_t<>>
constexpr bool IsModularizable = false;

template <typename _Type>
constexpr bool IsModularizable<_Type, std::void_t<decltype(_Type::ModuleStage)>> = true;

template <typename _Type>
concept Modularizable = IsModularizable<_Type>;

}

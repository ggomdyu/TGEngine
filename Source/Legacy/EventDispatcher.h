/**
 * @file    EventDispatcher.h
 * @author  ggomdyu
 * @since   05/19/2018
 */

#pragma once
#include <unordered_map>

#include "Core/Delegate.h"
#include "Text/StringHash.h"

namespace tgon
{

class EventDispatcher final
{
/**@section Type */
public:
    using EventHandler = Delegate<void()>;
    
/**@section Method */
public:
    void SubscribeEvent(const StringHash& eventName, const EventHandler& eventHandler);
    void SubscribeEvent(const StringHash& eventName, EventHandler&& eventHandler);
    bool UnsubscribeEvent(const StringHash& eventName, const EventHandler& eventHandler);
    void DispatchEvent(const StringHash& eventName);
    
/**@section Variable */
private:
    std::unordered_map<StringHash, std::unordered_map<uintptr_t, EventHandler>> m_eventHandlers;
};

} /* namespace tgon */

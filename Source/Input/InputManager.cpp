#include "PrecompiledHeader.h"

#include "Engine/Application.h"

#include "InputManager.h"

namespace tg
{

InputManager::InputManager()
{
    if (m_inputManager == nullptr)
    {
        m_inputManager = std::make_unique<gainput::InputManager>();
    }

#if TGON_PLATFORM_WINDOWS
    Application::GetInstance().GetPlatformDependency().SetCustomMessageCallback([](const MSG& msg)
    {
        m_inputManager->HandleMessage(msg);
    });
#endif
}

InputManager::~InputManager()
{
    m_inputManager.reset();
}

std::unique_ptr<Keyboard> InputManager::CreateKeyboard() const
{
    return std::make_unique<Keyboard>(m_inputManager->CreateAndGetDevice<gainput::InputDeviceKeyboard>());
}

std::unique_ptr<Mouse> InputManager::CreateMouse() const
{
    return std::make_unique<Mouse>(m_inputManager->CreateAndGetDevice<gainput::InputDeviceMouse>());
}

std::unique_ptr<Gamepad> InputManager::CreateGamePad() const
{
    return std::make_unique<Gamepad>(m_inputManager->CreateAndGetDevice<gainput::InputDevicePad>());
}

void InputManager::Update()
{
    m_inputManager->Update();
}

}

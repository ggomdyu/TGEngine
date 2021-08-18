#include "Engine/Application.h"

#include "InputManager.h"

TG_NAMESPACE_BEGIN

InputManager::InputManager()
{
    if (_inputManager == nullptr)
    {
        _inputManager = std::make_unique<gainput::InputManager>();
    }

#if TG_PLATFORM_WINDOWS
    Application::GetInstance().GetPlatformDependency().SetCustomMessageCallback([](const MSG& msg)
    {
        _inputManager->HandleMessage(msg);
    });
#endif
}

InputManager::~InputManager()
{
    _inputManager.reset();
}

std::unique_ptr<Keyboard> InputManager::CreateKeyboard() const
{
    return std::make_unique<Keyboard>(_inputManager->CreateAndGetDevice<gainput::InputDeviceKeyboard>());
}

std::unique_ptr<Mouse> InputManager::CreateMouse() const
{
    return std::make_unique<Mouse>(_inputManager->CreateAndGetDevice<gainput::InputDeviceMouse>());
}

std::unique_ptr<Gamepad> InputManager::CreateGamePad() const
{
    return std::make_unique<Gamepad>(_inputManager->CreateAndGetDevice<gainput::InputDevicePad>());
}

void InputManager::Update()
{
    _inputManager->Update();
}

}

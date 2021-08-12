#pragma once

#include <gainput/gainput.h>

#include "Core/NonCopyable.h"

namespace tg
{

enum class KeyCode
{
    Escape          = gainput::KeyEscape,
    F1              = gainput::KeyF1,
    F2              = gainput::KeyF2,
    F3              = gainput::KeyF3,
    F4              = gainput::KeyF4,
    F5              = gainput::KeyF5,
    F6              = gainput::KeyF6,
    F7              = gainput::KeyF7,
    F8              = gainput::KeyF8,
    F9              = gainput::KeyF9,
    F10             = gainput::KeyF10,
    F11             = gainput::KeyF11,
    F12             = gainput::KeyF12,
    F13             = gainput::KeyF13,
    F14             = gainput::KeyF14,
    F15             = gainput::KeyF15,
    ScrollLock      = gainput::KeyScrollLock,
    Break           = gainput::KeyBreak,
    Space           = gainput::KeySpace,
    Apostrophe      = gainput::KeyApostrophe,
    Comma           = gainput::KeyComma,
    Minus           = gainput::KeyMinus,
    Period          = gainput::KeyPeriod,
    Slash           = gainput::KeySlash,
    Alpha0          = gainput::Key0,
    Alpha1          = gainput::Key1,
    Alpha2          = gainput::Key2,
    Alpha3          = gainput::Key3,
    Alpha4          = gainput::Key4,
    Alpha5          = gainput::Key5,
    Alpha6          = gainput::Key6,
    Alpha7          = gainput::Key7,
    Alpha8          = gainput::Key8,
    Alpha9          = gainput::Key9,
    Semicolon       = gainput::KeySemicolon,
    Less            = gainput::KeyLess,
    A               = gainput::KeyA,
    B               = gainput::KeyB,
    C               = gainput::KeyC,
    D               = gainput::KeyD,
    E               = gainput::KeyE,
    F               = gainput::KeyF,
    G               = gainput::KeyG,
    H               = gainput::KeyH,
    I               = gainput::KeyI,
    J               = gainput::KeyJ,
    K               = gainput::KeyK,
    L               = gainput::KeyL,
    M               = gainput::KeyM,
    N               = gainput::KeyN,
    O               = gainput::KeyO,
    P               = gainput::KeyP,
    Q               = gainput::KeyQ,
    R               = gainput::KeyR,
    S               = gainput::KeyS,
    T               = gainput::KeyT,
    U               = gainput::KeyU,
    V               = gainput::KeyV,
    W               = gainput::KeyW,
    X               = gainput::KeyX,
    Y               = gainput::KeyY,
    Z               = gainput::KeyZ,
    LeftBracket     = gainput::KeyBracketLeft,
    Backslash       = gainput::KeyBackslash,
    RightBracket    = gainput::KeyBracketRight,
    Grave           = gainput::KeyGrave,
    LeftArrow       = gainput::KeyLeft,
    RightArrow      = gainput::KeyRight,
    UpArrow         = gainput::KeyUp,
    DownArrow       = gainput::KeyDown,
    Insert          = gainput::KeyInsert,
    Home            = gainput::KeyHome,
    Delete          = gainput::KeyDelete,
    End             = gainput::KeyEnd,
    PageUp          = gainput::KeyPageUp,
    PageDown        = gainput::KeyPageDown,
    NumLock         = gainput::KeyNumLock,
    KeypadEquals    = gainput::KeyKpEqual,
    KeypadDivide    = gainput::KeyKpDivide,
    KeypadMultiply  = gainput::KeyKpMultiply,
    KeypadSubtract  = gainput::KeyKpSubtract,
    KeypadAdd       = gainput::KeyKpAdd,
    KeypadEnter     = gainput::KeyKpEnter,
    KeypadInsert    = gainput::KeyKpInsert,
    KeypadEnd       = gainput::KeyKpEnd,
    KeypadDown      = gainput::KeyKpDown,
    KeypadPageDown  = gainput::KeyKpPageDown,
    KeypadLeft      = gainput::KeyKpLeft,
    KeypadRight     = gainput::KeyKpRight,
    KeypadHome      = gainput::KeyKpHome,
    KeypadUp        = gainput::KeyKpUp,
    KeypadPageUp    = gainput::KeyKpPageUp,
    KeypadDelete    = gainput::KeyKpDelete,
    BackSpace       = gainput::KeyBackSpace,
    Tab             = gainput::KeyTab,
    Return          = gainput::KeyReturn,
    CapsLock        = gainput::KeyCapsLock,
    LeftShift       = gainput::KeyShiftL,
    RightShift      = gainput::KeyShiftR,
    LeftControl     = gainput::KeyCtrlL,
    RightControl    = gainput::KeyCtrlR,
    LeftWindows     = gainput::KeySuperL,
    RightWindows    = gainput::KeySuperR,
    LeftAlt         = gainput::KeyAltL,
    RightAlt        = gainput::KeyAltR,
    Back            = gainput::KeyBack,
    VolumeUp        = gainput::KeyVolumeUp,
    VolumeDown      = gainput::KeyVolumeDown,
    Power           = gainput::KeyPower,
    Equals          = gainput::KeyEquals,
    At              = gainput::KeyAt,
    Plus            = gainput::KeyPlus,
    MediaPlayPause  = gainput::KeyMediaPlayPause,
    MediaStop       = gainput::KeyMediaStop,
    MediaNext       = gainput::KeyMediaNext,
    MediaPrevious   = gainput::KeyMediaPrevious,
    Mute            = gainput::KeyMute,
    Fn              = gainput::KeyFn,
    Colon           = gainput::KeyColon,
    SysRq           = gainput::KeySysRq,
};

class Keyboard final :
    private NonCopyable
{
/**@section Constructor */
public:
    explicit Keyboard(gainput::InputDeviceKeyboard* nativeKeyboard) noexcept;

/**@section Method */
public:
    void Update();
    [[nodiscard]] bool IsKeyDown(KeyCode keyCode) const;
    [[nodiscard]] bool IsKeyHold(KeyCode keyCode) const;
    [[nodiscard]] bool IsKeyUp(KeyCode keyCode) const;

private:
    gainput::InputDeviceKeyboard* m_nativeKeyboard;
};
    
}

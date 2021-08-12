#include "PrecompiledHeader.h"

#include <array>

#include "Core/Algorithm.h"

#include "../Application.h"
#include "../Engine.h"

#ifndef NDEBUG
#define _CRTDBG_MAP_ALLOC
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace tg
{

extern thread_local std::array<wchar_t, 32768> g_tempUtf16StrBuffer;

namespace
{

LRESULT CALLBACK OnHandleMessage(HWND wndHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto* window = reinterpret_cast<Window*>(GetWindowLongPtrW(wndHandle, GWLP_USERDATA));
    if (window == nullptr)
    {
        return DefWindowProcW(wndHandle, msg, wParam, lParam);
    }

    switch (msg)
    {
    case WM_CREATE:
        break;

    case WM_SYSCOMMAND:
        {
            switch (wParam)
            {
            case SC_MINIMIZE:
                {
                    if (window->OnMinimize != nullptr)
                    {
                        window->OnMinimize();
                    }
                }
                break;

            case SC_MAXIMIZE:
                {
                    if (window->OnMaximize != nullptr)
                    {
                        window->OnMaximize();
                    }
                }
                break;

            default:
                break;
            }
        }
        break;

    case WM_SETFOCUS:
        {
            if (window->OnGetFocus != nullptr)
            {
                window->OnGetFocus();
            }
        }
        break;

    case WM_KILLFOCUS:
        {
            if (window->OnLoseFocus != nullptr)
            {
                window->OnLoseFocus();
            }
        }
        break;

    case WM_MOVE:
        {
            if (window->OnMove != nullptr)
            {
                window->OnMove(static_cast<int32_t>(LOWORD(lParam)), static_cast<int32_t>(HIWORD(lParam)));
            }
        }
        break;

    case WM_SIZE:
        {
            if (window->OnResize != nullptr)
            {
                window->OnResize(static_cast<int32_t>(LOWORD(lParam)), static_cast<int32_t>(HIWORD(lParam)));
            }
        }
        break;

    case WM_CLOSE:
        {
            if (window->OnWillClose != nullptr)
            {
                window->OnWillClose();
            }
        }
        break;

    case WM_DESTROY:
        {
            if (window->OnDidClose != nullptr)
            {
                window->OnDidClose();
            }

            PostQuitMessage(0);
        }
        break;

    default:
        break;
    }

    return DefWindowProcW(wndHandle, msg, wParam, lParam);
}

bool RegisterWindowClass(WNDPROC messageCallback)
{
    const WNDCLASSEXW wndClass
    {
        .cbSize = sizeof(wndClass),
        .style = CS_DBLCLKS,
        .lpfnWndProc = messageCallback,
        .cbWndExtra = sizeof(void*),
        .hInstance = GetModuleHandleW(nullptr),
        .hIcon = LoadIconW(nullptr, IDI_APPLICATION),
        .hCursor = LoadCursorW(nullptr, IDC_ARROW),
        .hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH)),
        .lpszClassName = L"TGON",
    };

    return RegisterClassExW(&wndClass) != 0;
}

constexpr UINT ConvertMessageBoxIconToNative(MessageBoxIcon messageBoxIcon) noexcept
{
    constexpr UINT nativeMessageBoxIcons[] = {
        0,
        MB_ICONQUESTION,
        MB_ICONEXCLAMATION,
    };
    return nativeMessageBoxIcons[UnderlyingCast(messageBoxIcon)];
}

}

WindowsApplication::WindowsApplication()
{
    RegisterWindowClass(OnHandleMessage);

#ifndef NDEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void WindowsApplication::SetCustomMessageCallback(MessageCallback callback)
{
    m_messageCallback = callback;
}

void Application::MessageLoop()
{
    MSG msg {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            DispatchMessageW(&msg);

            if (m_messageCallback != nullptr)
            {
                m_messageCallback(msg);
            }
        }
        else
        {
            m_engine->Update();
        }
    }
}

void Application::ShowMessageBox(const char8_t* title, const char8_t* message, MessageBoxIcon messageBoxIcon)
{
    const auto utf16Title = std::span(g_tempUtf16StrBuffer).subspan(0, g_tempUtf16StrBuffer.size() / 2);
    const auto utf16TitleLen = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(title), -1, &utf16Title[0], utf16Title.size());
    if (utf16TitleLen == 0)
    {
        return;
    }

    const auto utf16Message = std::span(g_tempUtf16StrBuffer).subspan(g_tempUtf16StrBuffer.size() / 2, g_tempUtf16StrBuffer.size() / 2);
    const auto utf16MessageLen = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(message), -1, &utf16Message[0], static_cast<int>(utf16Message.size()));
    if (utf16MessageLen == 0)
    {
        return;
    }

    MessageBoxW(nullptr, utf16Message.data(), utf16Title.data(), ConvertMessageBoxIconToNative(messageBoxIcon) | MB_OK);
}

Engine& Application::GetEngine() noexcept
{
    return *m_engine;
}

const Engine& Application::GetEngine() const noexcept
{
    return *m_engine;
}

}

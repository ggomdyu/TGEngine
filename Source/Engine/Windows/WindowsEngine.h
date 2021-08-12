#pragma once

#include <memory>
#ifndef NDEBUG
#include <crtdbg.h>
#endif

#include "../Application.h"

#define TGON_ENGINE(className)                                                                                                                                                         \
    int WINAPI WinMain([[maybe_unused]] HINSTANCE instanceHandle, [[maybe_unused]] HINSTANCE prevInstanceHandle, [[maybe_unused]] LPSTR commandLine, [[maybe_unused]] int commandShow) \
    {                                                                                                                                                                                  \
        static_assert(std::is_base_of_v<tg::Engine, className>, "TGON_ENGINE accepts only class that inherited from Engine.");                                                         \
                                                                                                                                                                                       \
        decltype(auto) application = tg::Application::GetInstance();                                                                                                                   \
        application.Initialize(std::make_unique<className>());                                                                                                                         \
        application.MessageLoop();                                                                                                                                                     \
                                                                                                                                                                                       \
        return 0;                                                                                                                                                                      \
    }
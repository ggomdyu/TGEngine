#pragma once

#ifdef _WINDLL
#ifdef TGON_API_EXPORT
#define TGON_API __declspec(dllexport)
#else
#define TGON_API __declspec(dllimport)
#endif
#else
#define TGON_API
#endif
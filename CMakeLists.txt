cmake_minimum_required(VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE external/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(TGEngine)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MACOSX_RPATH ON)

include(cmake/options.cmake)

function(init_subdirectories)
    add_subdirectory(src)
endfunction()

function(init_properties)
    set_target_properties(TGEngine PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        FOLDER TGEngine
    )
endfunction()

function(init_compile_definitions)
    target_compile_definitions(TGEngine PUBLIC
        # Namespaces
        "TG_NAMESPACE_NAME=${TG_NAMESPACE_NAME}"
        "TG_NAMESPACE_BEGIN=$<$<BOOL:${TG_ENABLE_NAMESPACE}>:namespace\t${TG_NAMESPACE_NAME}{>"
        "TG_NAMESPACE_END=$<$<BOOL:${TG_ENABLE_NAMESPACE}>:}>"

        # Platform settings
        "TG_PLATFORM_$<UPPER_CASE:${TG_PLATFORM}>=1"
        "TG_GRAPHICS_$<UPPER_CASE:${TG_GRAPHICS}>=1"
        "TG_SUPPORT_POSIX=$<NOT:$<PLATFORM_ID:Windows>>"

        # Compiler attributes
        "TG_NOINLINE=$<IF:$<CXX_COMPILER_ID:MSVC>,__declspec(noinline),__attribute__((noinline))>"
        "TG_NOVTABLE=$<$<CXX_COMPILER_ID:MSVC>:__declspec(novtable)>"

        # Etc
        $<$<PLATFORM_ID:Windows>:UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX>
    )
endfunction()

function(init_link_libraries)
    target_link_libraries(TGEngine PUBLIC 
        fmt::fmt-header-only
        freetype
        unofficial::gainput::gainput
        GLEW::glew
        #OpenAL
    )
endfunction()

function(init_tg)
    init_subdirectories()
    init_properties()
    init_compile_definitions()
    init_link_libraries()
endfunction()

init_tg()
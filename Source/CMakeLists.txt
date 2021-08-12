cmake_minimum_required(VERSION 3.8)

include(CheckIncludeFile)
include(CheckIncludeFiles)

set(TGON_PATH ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "")

CHECK_INCLUDE_FILE(emmintrin.h TGON_SUPPORT_SSE2)
CHECK_INCLUDE_FILE(arm_neon.h TGON_SUPPORT_NEON)
set(TGON_SUPPORT_SIMD FALSE)
if(TGON_SUPPORT_SSE2 OR TGON_SUPPORT_NEON)
    set(TGON_SUPPORT_SIMD TRUE)
endif()
option(TGON_USING_SIMD "" ${TGON_SUPPORT_SIMD})

set(TGON_SUPPORT_POSIX FALSE)
if(NOT TGON_PLATFORM STREQUAL "Windows")
    set(TGON_SUPPORT_POSIX TRUE)
endif()

if(WIN32)
    set(TGON_PLATFORM "Windows" CACHE STRING "")
    set_property(CACHE TGON_PLATFORM PROPERTY STRINGS "Windows" "macOS" "Android" "iOS")
    set(TGON_GRAPHICS "OpenGL" CACHE STRING "")
    set_property(CACHE TGON_GRAPHICS PROPERTY STRINGS "OpenGL" "Direct3D11" "Direct3D12" "Vulkan")
elseif(APPLE)
    set(TGON_PLATFORM "macOS" CACHE STRING "")
    set_property(CACHE TGON_PLATFORM PROPERTY STRINGS "macOS" "iOS")
    set(TGON_GRAPHICS "OpenGL" CACHE STRING "")
    set_property(CACHE TGON_GRAPHICS PROPERTY STRINGS "OpenGL" "Metal")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")  
else()
    message(FATAL_ERROR "Your platform is not supported.")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

function(init_tgon)
    init_tgon_subdirectories()
    
    init_tgon_properties()
    init_tgon_include_directories()
    init_tgon_compile_definitions()
    init_tgon_compile_options()
    init_tgon_link_libraries()
endfunction()

function(init_tgon_subdirectories)
    set(GLOB_AGRUMENTS *.cpp *c *.h *.mm *.inl)
    file(GLOB_RECURSE ALL_SRC_PATHS ${GLOB_AGRUMENTS})

    set(SRC_PATHS)
    string(TOUPPER ${TGON_PLATFORM} UPPERCASED_PLATFORM)
    string(TOUPPER ${TGON_GRAPHICS} UPPERCASED_GRAPHICS)

    foreach(SRC_PATH ${ALL_SRC_PATHS})
        # Ignore special directories
        string(TOUPPER ${SRC_PATH} UPPERCASED_SRC_PATH)
        if(${UPPERCASED_SRC_PATH} MATCHES "/THIRDPARTY/|/LEGACY/|/TEST/")
            continue()
        endif()

        # Ignore other platform sources
        if(${UPPERCASED_SRC_PATH} MATCHES "/WINDOWS|/MACOS|/ANDROID|/IOS|/LINUX|/POSIX")
            if(NOT ${UPPERCASED_SRC_PATH} MATCHES ${UPPERCASED_PLATFORM})
                if(NOT(${TGON_SUPPORT_POSIX} AND ${UPPERCASED_SRC_PATH} MATCHES "/POSIX"))
                    continue()
                endif()
            endif()
        endif()

        # Ignore unsupported graphics API sources
        if(${UPPERCASED_SRC_PATH} MATCHES "/GRAPHICS/(OPENGL|VULKAN|DIRECT3D11|DIRECT3D12|METAL)")
            if(NOT ${UPPERCASED_SRC_PATH} MATCHES ${UPPERCASED_GRAPHICS})
                continue()
            endif()
        endif()

        list(APPEND SRC_PATHS ${SRC_PATH})
    endforeach()

    add_library(TGON ${SRC_PATHS})
    source_group(TREE ${TGON_PATH} FILES ${SRC_PATHS})
endfunction()

function(init_tgon_properties)
    set_target_properties(TGON PROPERTIES
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        FOLDER TGON
    )
endfunction()

function(init_tgon_include_directories)
    set(TGON_INCLUDE_PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}
        ThirdParty/assimp-4.1.0/include
        ThirdParty/fmt-7.0.1/include
        ThirdParty/gainput-1.0.0/lib/include
        ThirdParty/GSL-2.0.0/include
        ThirdParty/icu4c-64_2/icu/source/common
        ThirdParty/icu4c-64_2/icu/source/i18n
        ThirdParty/libiconv-1.9.2/include
        ThirdParty/openal-soft-1.20.1
        ThirdParty/openal-soft-1.20.1/alc
        ThirdParty/openal-soft-1.20.1/common
        ThirdParty/openal-soft-1.20.1/include
        ThirdParty/stb
        ThirdParty/zlib-1.2.11
    )
    if(WIN32)
        list(APPEND TGON_INCLUDE_PATHS $ENV{DXSDK_DIR}/Include)
    endif()
    target_include_directories(TGON PUBLIC ${TGON_INCLUDE_PATHS})
endfunction()

function(init_tgon_compile_definitions)
    target_compile_definitions(TGON PUBLIC
        $<$<STREQUAL:$<UPPER_CASE:"${TGON_PLATFORM}">,"WINDOWS">:
            -DUNICODE
            -D_UNICODE
        >
        $<UPPER_CASE:TGON_PLATFORM_${TGON_PLATFORM}>=1
        $<UPPER_CASE:TGON_GRAPHICS_${TGON_GRAPHICS}>=1
        TGON_USING_SIMD=$<BOOL:${TGON_USING_SIMD}>
        TGON_SUPPORT_SIMD=$<BOOL:${TGON_SUPPORT_SIMD}>
        TGON_SUPPORT_POSIX=$<BOOL:${TGON_SUPPORT_POSIX}>
        $<IF:$<BOOL:${TGON_SUPPORT_SSE2}>,TGON_SIMD_SSE2=1,
        $<IF:$<BOOL:${TGON_SUPPORT_NEON}>,TGON_SIMD_NEON=1,>>
    )
endfunction()

function(init_tgon_compile_options)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(TGON PRIVATE
            /YcPrecompiledHeader.h
            /Oi
            $<$<CONFIG:Debug>:/Zi>
        )
        target_compile_options(TGON PUBLIC /std:c++latest)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        target_compile_options(TGON PUBLIC -std=c++2a)
    endif()
endfunction()

function(init_tgon_link_libraries)
    target_link_libraries(TGON PUBLIC 
        # common 
        fmt 
        freetype 
        gainputstatic 
        glew 
        icu 
        # OpenAL
    )
endfunction()

function(init_thirdparty)
    init_thirdparty_subdirectories()
    init_thirdparty_compile_definitions()
    init_thirdparty_target_properties()
    init_thirdparty_unset()
endfunction()

function(init_thirdparty_subdirectories)
    # option(ALSOFT_TESTS "" FALSE)
    # option(ALSOFT_UTILS "" FALSE)
    # option(ALSOFT_EXAMPLES "" FALSE)
    # set(LIBTYPE "STATIC")
    # add_subdirectory(ThirdParty/openal-soft-1.20.1)
 
    option(GAINPUT_TESTS "" FALSE)
    option(GAINPUT_BUILD_SHARED "" FALSE)
    option(GAINPUT_SAMPLES "" FALSE)
    add_subdirectory(ThirdParty/gainput-1.0.0)
    
    add_subdirectory(ThirdParty/fmt-7.0.1)
    add_subdirectory(ThirdParty/freetype-2.10.1)
    add_subdirectory(ThirdParty/glew-2.1.0)
    add_subdirectory(ThirdParty/icu4c-64_2/icu)
    add_subdirectory(ThirdParty/zlib-1.2.11)
endfunction()

function(init_thirdparty_compile_definitions)
    target_compile_definitions(freetype PUBLIC 
        FT_DEBUG_LEVEL_ERROR 
        FT_DEBUG_LEVEL_TRACE 
        FT2_BUILD_LIBRARY
    )
    target_compile_definitions(glew PUBLIC GLEW_STATIC)
    target_compile_definitions(icu PUBLIC 
        U_STATIC_IMPLEMENTATION 
        U_COMMON_IMPLEMENTATION 
        U_I18N_IMPLEMENTATION
    )
    # target_compile_definitions(OpenAL PUBLIC AL_LIBTYPE_STATIC)
    # target_compile_definitions(common PUBLIC AL_LIBTYPE_STATIC)
endfunction()

function(init_thirdparty_target_properties)
    # set_target_properties(common PROPERTIES FOLDER ThirdParty)
    # set_target_properties(ex-common PROPERTIES FOLDER ThirdParty)
    set_target_properties(fmt PROPERTIES FOLDER ThirdParty)
    set_target_properties(freetype PROPERTIES FOLDER ThirdParty)
    set_target_properties(gainputstatic PROPERTIES FOLDER ThirdParty)
    set_target_properties(glew PROPERTIES FOLDER ThirdParty)
    set_target_properties(icu PROPERTIES FOLDER ThirdParty)
    # set_target_properties(native-tools PROPERTIES FOLDER ThirdParty)
    # set_target_properties(OpenAL PROPERTIES FOLDER ThirdParty)
    set_target_properties(zlib PROPERTIES FOLDER ThirdParty)
endfunction()

function(init_thirdparty_unset)
    # unset(ALSOFT_AMBDEC_PRESETS CACHE)
    # unset(ALSOFT_BACKEND_DSOUND CACHE)
    # unset(ALSOFT_BACKEND_WASAPI CACHE)
    # unset(ALSOFT_BACKEND_WAVE CACHE)
    # unset(ALSOFT_BACKEND_WINMM CACHE)
    # unset(ALSOFT_BUILD_ROUTER CACHE)
    # unset(ALSOFT_CONFIG CACHE)
    # unset(ALSOFT_CPUEXT_SSE CACHE)
    # unset(ALSOFT_CPUEXT_SSE2 CACHE)
    # unset(ALSOFT_CPUEXT_SSE3 CACHE)
    # unset(ALSOFT_CPUEXT_SSE4_1 CACHE)
    # unset(ALSOFT_DLOPEN CACHE)
    # unset(ALSOFT_EMBED_HRTF_DATA CACHE)
    # unset(ALSOFT_HRTF_DEFS CACHE)
    # unset(ALSOFT_INSTALL CACHE)
    # unset(ALSOFT_NATIVE_TOOLS_PATH CACHE)
    # unset(ALSOFT_NO_CONFIG_UTIL CACHE)
    # unset(ALSOFT_NO_UID_DEFS CACHE)
    # unset(ALSOFT_REQUIRE_ALSA CACHE)
    # unset(ALSOFT_REQUIRE_COREAUDIO CACHE)
    # unset(ALSOFT_REQUIRE_DSOUND CACHE)
    # unset(ALSOFT_REQUIRE_JACK CACHE)
    # unset(ALSOFT_REQUIRE_NEON CACHE)
    # unset(ALSOFT_EXAMPLES CACHE)
    # unset(ALSOFT_REQUIRE_OPENSL CACHE)
    # unset(ALSOFT_REQUIRE_OSS CACHE)
    # unset(ALSOFT_REQUIRE_PORTAUDIO CACHE)
    # unset(ALSOFT_REQUIRE_PULSEAUDIO CACHE)
    # unset(ALSOFT_REQUIRE_QSA CACHE)
    # unset(ALSOFT_REQUIRE_SDL2 CACHE)
    # unset(ALSOFT_REQUIRE_SNDIO CACHE)
    # unset(ALSOFT_REQUIRE_SOLARIS CACHE)
    # unset(ALSOFT_REQUIRE_SSE CACHE)
    # unset(ALSOFT_REQUIRE_SSE2 CACHE)
    # unset(ALSOFT_REQUIRE_SSE3 CACHE)
    # unset(ALSOFT_REQUIRE_SSE4_1 CACHE)
    # unset(ALSOFT_REQUIRE_WASAPI CACHE)
    # unset(ALSOFT_REQUIRE_WINMM CACHE)
    # unset(ALSOFT_TESTS CACHE)
    # unset(ALSOFT_UTILS CACHE)
    # unset(ALSOFT_WERROR CACHE)
    # unset(ALSOFT_BACKEND_COREAUDIO CACHE)
    # unset(ALSOFT_STATIC_LIBGCC CACHE)
    # unset(ALSOFT_STATIC_STDCXX CACHE)
    # unset(FORCE_STATIC_VCRT CACHE)
    # unset(SDL2MAIN_LIBRARY CACHE)
    # unset(SDL2_CORE_LIBRARY CACHE)
    # unset(SDL2_INCLUDE_DIR CACHE)
    # unset(COREAUDIO_FRAMEWORK CACHE)
    # unset(WINMM_LIBRARY CACHE)
    # unset(AUDIOUNIT_INCLUDE_DIR CACHE)
    # unset(CMAKE_DEBUG_POSTFIX CACHE)
    # unset(CMAKE_BACKWARDS_COMPATIBILITY CACHE)
    # unset(CMAKE_BUILD_TYPE CACHE)

    # unset(GAINPUT_BUILD_SHARED CACHE)
    # unset(GAINPUT_BUILD_STATIC CACHE)
    # unset(GAINPUT_SAMPLES CACHE)
    # unset(GAINPUT_TESTS CACHE)

    # unset(FMT_CUDA_TEST CACHE)
    # unset(FMT_DOC CACHE)
    # unset(FMT_FUZZ CACHE)
    # unset(FMT_INSTALL CACHE)
    # unset(FMT_PEDANTIC CACHE)
    # unset(FMT_TEST CACHE)
    # unset(FMT_WERROR CACHE)
    # unset(FMT_DEBUG_POSTFIX CACHE)
    # unset(FMT_INC_DIR CACHE)
    # unset(FMT_OS CACHE)

    # unset(FT_WITH_BZIP2 CACHE)
    # unset(FT_WITH_HARFBUZZ CACHE)
    # unset(FT_WITH_PNG CACHE)
    # unset(FT_WITH_ZLIB CACHE)

    # unset(AGL_LIB_PATH CACHE)
    # unset(APPKIT CACHE)
    # unset(AUDIOTOOLBOX_LIBRARY CACHE)
    # unset(FOUNDATION CACHE)
    # unset(GAME_CONTROLLER CACHE)
    # unset(GL_LIB_PATH CACHE)
    # unset(IOKIT CACHE)
    # unset(EXECUTABLE_OUTPUT_PATH CACHE)
    # unset(EXECUTABLE_OUTPUT_PATH CACHE)
    # unset(LIBRARY_OUTPUT_PATH CACHE)
endfunction()

init_tgon()
init_thirdparty()

set(GLOB_AGRUMENTS *.cpp *.c *.mm)
file(GLOB_RECURSE SRC_PATH_LIST ${GLOB_AGRUMENTS})

string(TOUPPER ${TG_PLATFORM} UPPERCASE_PLATFORM)

foreach(SRC_PATH ${SRC_PATH_LIST})
    string(TOUPPER ${SRC_PATH} UPPERCASE_SRC_PATH)

    # Ignore the source which is not specific to current platform.
    if(${UPPERCASE_SRC_PATH} MATCHES "((\\.WINDOWS)|(\\.DARWIN)|(\\.ANDROID))\\.")
        # If the platform prefix doesn't matched from current platform
        if(NOT ${UPPERCASE_SRC_PATH} MATCHES ${UPPERCASE_PLATFORM})
            continue()
        endif()
        # Ignore the source if the current platform doesn't support POSIX.
    elseif(${UPPERCASE_SRC_PATH} MATCHES "\\.POSIX\\.")
        if(NOT ${TG_SUPPORT_POSIX})
            continue()
        endif()
    endif()

    list(APPEND TG_SOURCES ${SRC_PATH})
endforeach()

target_sources(TGEngine PUBLIC ${TG_SOURCES})
source_group(TREE ${TG_PATH} FILES ${SRC_PATHS})
cmake_minimum_required (VERSION 3.8)

project(glew)

function (init_project)
    file(GLOB_RECURSE SRC_PATH_LIST *.c *.cpp *.h)
    add_library(glew ${SRC_PATH_LIST})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_PATH_LIST})

	init_project_properties()
endfunction ()

function (init_project_properties)
    target_include_directories(glew PUBLIC "include")
	
	if (WIN32)
	elseif (APPLE)
		find_library(GL_LIB_PATH OpenGL)
		find_library(AGL_LIB_PATH AGL)
		SET(GLEW_LIB_PATH ${GL_LIB_PATH} ${AGL_LIB_PATH})
	else ()
		SET(GLEW_LIB_PATH GL X11 dl Xext)
	endif ()

    target_link_libraries(glew ${GLEW_LIB_PATH})
endfunction ()

init_project()
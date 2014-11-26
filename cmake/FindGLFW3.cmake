#
# Try to find GLFW3 library and include path.
# Once done this will define
#
# GLFW3_FOUND
# GLFW3_INCLUDE_PATH
# GLFW3_LIBRARY
# 

IF (MINGW)
    FIND_PATH( GLFW3_INCLUDE_PATH GLFW/glfw3.h
        ${DEPENDENCIES_PATH}/glfw-3.0.4.bin.WIN32/include
    )

    FIND_LIBRARY( GLFW3_LIBRARY
        NAMES glfw3dll
        PATHS
        ${DEPENDENCIES_PATH}/glfw-3.0.4.bin.WIN32/lib-mingw
    )

    execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
        ${DEPENDENCIES_PATH}/glfw-3.0.4.bin.WIN32/lib-mingw/glfw3.dll
        ${PROJECT_BINARY_DIR}/bin/
    )

ELSEIF (MSVC)
    FIND_PATH( GLFW3_INCLUDE_PATH GLFW/glfw3.h
        ${DEPENDENCIES_PATH}/glfw-3.0.4.bin.WIN32/include
    )

    FIND_LIBRARY( GLFW3_LIBRARY
        NAMES glfw3dll
        PATHS
        ${DEPENDENCIES_PATH}/glfw-3.0.4.bin.WIN32/lib-msvc110
    )

    foreach (CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
            ${DEPENDENCIES_PATH}/glfw-3.0.4.bin.WIN32/lib-msvc110/glfw3.dll
            ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION_TYPE}/
        )
    endforeach()

ELSEIF(APPLE)

    FIND_PATH(GLFW3_INCLUDE_PATH GLFW/glfw3.h
    ${DEPENDENCIES_PATH}/glfw-3.0.3_OSX/include)
    
    FIND_LIBRARY( GLFW3_LIBRARY
        NAMES libglfw3.a
        PATHS ${DEPENDENCIES_PATH}/glfw-3.0.3_OSX/build/src)

ELSE()
	FIND_PATH(GLFW3_INCLUDE_PATH GLFW/glfw3.h)
	FIND_LIBRARY(GLFW3_LIBRARY
        NAMES glfw3 glfw
	PATH_SUFFIXES dynamic) 
ENDIF ()



SET(GLFW3_FOUND "NO")
IF (GLFW3_INCLUDE_PATH AND GLFW3_LIBRARY)
	SET(GLFW3_LIBRARIES ${GLFW3_LIBRARY})
	SET(GLFW3_FOUND "YES")
    message("EXTERNAL LIBRARY 'GLFW3' FOUND")
    message("GLFW3_LIBRARY: " ${GLFW3_LIBRARY})
    message("GLFW3_INCLUDE_PATH: " ${GLFW3_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'GLFW3' NOT FOUND")
ENDIF (GLFW3_INCLUDE_PATH AND GLFW3_LIBRARY)
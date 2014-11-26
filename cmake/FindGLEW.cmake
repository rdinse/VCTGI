#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
# 

IF (MINGW)
	FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
		${DEPENDENCIES_PATH}/glew.MinGW/include/
	)

    FIND_LIBRARY( GLEW_LIBRARY
        NAMES glew32.dll
        PATHS
        ${DEPENDENCIES_PATH}/glew.MinGW/lib
    )

    execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
        ${DEPENDENCIES_PATH}/glew.MinGW/bin/glew32.dll
        ${PROJECT_BINARY_DIR}/bin/glew32.dll
    )

ELSEIF (MSVC)
    FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
        ${DEPENDENCIES_PATH}/glew-1.10.0/include/
    )

    FIND_LIBRARY( GLEW_LIBRARY
        NAMES glew32
        PATHS
        ${DEPENDENCIES_PATH}/glew-1.10.0/lib/Release/Win32/
    )

    foreach (CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
        execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
            ${DEPENDENCIES_PATH}/glew-1.10.0/bin/Release/Win32/glew32.dll
            ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION_TYPE}/glew32.dll
        )
    endforeach()
    

ELSEIF(APPLE)
	
	SET(GLEW_INCLUDE_PATH 
	 ${DEPENDENCIES_PATH}/glew-1.10.0_OSX/include)
	 
	SET(GLEW_LIBRARY
	${DEPENDENCIES_PATH}/glew-1.10.0_OSX/lib/libGLEW.a)
ELSE()
	FIND_PATH(GLEW_INCLUDE_PATH GL/glew.h)
	FIND_LIBRARY(GLEW_LIBRARY
	NAMES GLEW glew32 glew glew32s PATH_SUFFIXES lib64)
ENDIF()
    

SET(GLEW_FOUND "NO")
IF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)
	SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
	SET(GLEW_FOUND "YES")
    message("EXTERNAL LIBRARY 'GLEW' FOUND")
    message("GLEW_LIBRARY: " ${GLEW_LIBRARY})
    message("GLEW_INCLUDE_PATH: " ${GLEW_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'GLEW' NOT FOUND")
ENDIF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)
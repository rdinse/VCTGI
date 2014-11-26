#
# Try to find AntTweakBar library and include path.
# Once done this will define
#
# ANT_TWEAK_BAR_FOUND
# ANT_TWEAK_BAR_INCLUDE_PATH
# ANT_TWEAK_BAR_LIBRARY
# 

IF (MINGW)
    FIND_PATH( ANT_TWEAK_BAR_INCLUDE_PATH AntTweakBar.h
        PATHS ${DEPENDENCIES_PATH}/AntTweakBar/include
    )

    FIND_LIBRARY( ANT_TWEAK_BAR_LIBRARY AntTweakBar.lib
		PATHS ${DEPENDENCIES_PATH}/AntTweakBar/lib
    )

    execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
        ${DEPENDENCIES_PATH}/AntTweakBar/lib/AntTweakBar.dll
        ${PROJECT_BINARY_DIR}/bin/
    )
ELSE()
    FIND_PATH(ANT_TWEAK_BAR_INCLUDE_PATH AntTweakBar.h
		PATHS ${DEPENDENCIES_PATH}/AntTweakBar/include
	)
    FIND_LIBRARY(ANT_TWEAK_BAR_LIBRARY AntTweakBar.lib
        PATHS ${DEPENDENCIES_PATH}/AntTweakBar/lib
    )
ENDIF ()

#ELSEIF (MSVC)
#    FIND_PATH( ANT_TWEAK_BAR_INCLUDE_PATH assimp/defs.h
#        ${DEPENDENCIES_PATH}/assimp/include
#    )
#
#    FIND_LIBRARY( ANT_TWEAK_BAR_LIBRARY
#        NAMES assimp
#        PATHS
#        ${DEPENDENCIES_PATH}/assimp/lib/assimp_release-dll_win32
#    )
#
#    foreach (CONFIGURATION_TYPE ${CMAKE_CONFIGURATION_TYPES})
#        execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
#            ${DEPENDENCIES_PATH}/assimp/lib/assimp_release-dll_win32/Assimp32.dll
#            ${PROJECT_BINARY_DIR}/bin/${CONFIGURATION_TYPE}/
#        )
#    endforeach()
#
#ELSEIF(APPLE)
#
#    FIND_PATH(ANT_TWEAK_BAR_INCLUDE_PATH assimp/defs.h
#    ${DEPENDENCIES_PATH}/assimp-3.0.12_OSX/include)
#    
#    FIND_LIBRARY( ANT_TWEAK_BAR_LIBRARY
#        NAMES libassimp.a
#        PATHS ${DEPENDENCIES_PATH}/assimp-3.0.12_OSX/lib)
#    SET(ANT_TWEAK_BAR_LIBRARY z ${ANT_TWEAK_BAR_LIBRARY})
#
#


SET(ANT_TWEAK_BAR_FOUND "NO")
IF (ANT_TWEAK_BAR_INCLUDE_PATH AND ANT_TWEAK_BAR_LIBRARY)
	SET(ANT_TWEAK_BAR_LIBRARIES ${ANT_TWEAK_BAR_LIBRARY})
	SET(ANT_TWEAK_BAR_FOUND "YES")
    message("EXTERNAL LIBRARY 'ANTTWEAKBAR' FOUND")
    message("ANT_TWEAK_BAR_LIBRARY: " ${ANT_TWEAK_BAR_LIBRARY})
    message("ANT_TWEAK_BAR_INCLUDE_PATH: " ${ANT_TWEAK_BAR_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'ANTTWEAKBAR' NOT FOUND")
ENDIF (ANT_TWEAK_BAR_INCLUDE_PATH AND ANT_TWEAK_BAR_LIBRARY)

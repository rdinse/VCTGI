#
# Try to find OpenGL and include path.
# Once done this will define
#
# OpenGL3_FOUND
# OpenGL3_INCLUDE_PATH
# OpenGL3_LIBRARY
# 

IF (WIN32)

	FIND_PATH( OpenGL3_INCLUDE_PATH GL/glcorearb.h
		${DEPENDENCIES_PATH}/OpenGL/
	)
	SET(OpenGL3_LIBRARY OpenGL32)

ELSEIF (APPLE)
 FIND_PATH(OpenGL3_INCLUDE_PATH OpenGL/gl3.h 
	${DEPENDENCIES_PATH}/OpenGL/)
 SET(OpenGL3_LIBRARY "-framework Cocoa -framework OpenGL -framework IOKit" CACHE STRING "OpenGL lib for OSX")
 
ELSE()
 SET(OpenGL3_LIBRARY "GL" CACHE STRING "OpenGL lib for Linux")
    FIND_PATH(OpenGL3_INCLUDE_PATH GL/gl.h
      /usr/share/doc/NVIDIA_GLX-1.0/include
      /usr/openwin/share/include
      /opt/graphics/OpenGL/include /usr/X11R6/include
    )
ENDIF ()


SET(OpenGL3_FOUND "NO")
IF (OpenGL3_INCLUDE_PATH)
	SET(OpenGL3_LIBRARIES ${OpenGL3_LIBRARY})
	SET(OpenGL3_FOUND "YES")
    message("EXTERNAL LIBRARY 'OpenGL3' FOUND")
    message("OpenGL3_LIBRARY: " ${OpenGL3_LIBRARY})
    message("OpenGL3_INCLUDE_PATH: " ${OpenGL3_INCLUDE_PATH})
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'OpenGL3' NOT FOUND")
ENDIF (OpenGL3_INCLUDE_PATH)
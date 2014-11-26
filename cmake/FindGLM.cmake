#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLM_FOUND
# GLM_INCLUDE_PATH
# 

FIND_PATH( GLM_INCLUDE_PATH glm/glm.hpp
	${DEPENDENCIES_PATH}/glm
)


SET(GLM_FOUND "NO")
IF (GLM_INCLUDE_PATH)
	SET(GLM_FOUND "YES")
    message("EXTERNAL LIBRARY 'GLM' FOUND")
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'GLM' NOT FOUND")
ENDIF (GLM_INCLUDE_PATH)
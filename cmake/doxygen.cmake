#-- Add an Option to toggle the generation of the API documentation
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message( 
      "Doxygen is needed to build the documentation. Please install it correctly")
  else()

    #-- Configure the Template Doxyfile for our specific project
    configure_file(${CMAKE_MODULE_PATH}/Doxyfile.in 
      ${PROJECT_BINARY_DIR}/Doxyfile  @ONLY IMMEDIATE)

    #-- Add a custom target to run Doxygen when ever the project is built
    add_custom_target (Docs ALL 
    	COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
    	SOURCES ${PROJECT_BINARY_DIR}/Doxyfile)
    
    # IF you do NOT want the documentation to be generated EVERY time you build the project
    # then leave out the 'ALL' keyword from the above command.
  endif()
endif()
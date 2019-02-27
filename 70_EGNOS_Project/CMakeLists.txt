
file(GLOB_RECURSE EGNOS_SRC "src/*.cpp")
file(GLOB_RECURSE EGNOS_INCL "include/*.h" "include/*.hpp")



  # Define ext library include directories
  set( EGNOS_INC_DIRS "" )

  foreach( _headerFile ${EGNOS_INCL} )
    get_filename_component( _dir ${_headerFile} PATH )
    get_filename_component( _name ${_headerFile} NAME )
    list( APPEND EGNOS_INC_DIRS ${_dir} )
  endforeach()

  list( REMOVE_DUPLICATES EGNOS_INC_DIRS )

  # Add every directory containing a header file
  # to the project(gpstk) include_directories
  

  # append ext include files needed to build library target


include_directories (include)
add_executable(EGNOS_Project ${EGNOS_SRC} ${EGNOS_INCL} )
target_link_libraries (EGNOS_Project	PUBLIC ${GPSTK_LIBS}
										PUBLIC gpstk 
										PUBLIC GINAParser)
target_include_directories(EGNOS_Project PUBLIC ${EGNOS_INC_DIRS} ${GPSTK_CORE_INC_DIRS} ${GPSTK_EXT_INC_DIRS} ${GINAPARSER_CORE_INC_DIRS} ${EIGEN_CORE_INC_DIRS})


# Create VS Filters	
set( EGNOS_All_FILES "" )	
list( APPEND EGNOS_All_FILES ${EGNOS_SRC})
list( APPEND EGNOS_All_FILES ${EGNOS_INCL})					
assign_source_group(${EGNOS_All_FILES})

##################
# Set Properties #
##################
#####################################################################################################################################
set( EGNOS_INCLUDE_DIRS "" )
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY EGNOS_INCLUDE_DIRS ${EGNOS_INC_DIRS})
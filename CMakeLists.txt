cmake_minimum_required (VERSION 2.91.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/99_Cmake/definition.cmake) 

project (GINA)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Set directory definitions in the GINAConfig.h
set (ROOT_SRC_DIR "${CMAKE_SOURCE_DIR}")
change_slash2double_backslash(ROOT_SRC_DIR)





set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/GINAConfig.h.in"
  "${PROJECT_SOURCE_DIR}/99_Cmake/GINAConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find GINAConfig.h
include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/99_Cmake")

#################
# Math Packages #
#################
#################################################################
add_subdirectory (10_MathPckgs)

	############
	#Eigen Lib #
	############
	#################################################################
	get_property(EIGEN_CORE_INC_DIRS DIRECTORY "${PROJECT_SOURCE_DIR}/10_MathPckgs/Eigen" PROPERTY EIGEN_CORE_INC_DIRS)
	#message("Property for root directory root read from EIGEN_CORE_INC_DIRS: " ${EIGEN_CORE_INC_DIRS})

#########
# GPSTK #
#########
#################################################################

option( BUILD_EXT_EXAMPLE "HELP: BUILD_EXT_EXAMPLE: SWITCH, Default = OFF, GPSTK/ext/apps folder will be added - 1st exercise" OFF )

#add_subdirectory (20_GPSTk-master)
file( GLOB_RECURSE GPSTK_CORE_INC_FILES "20_GPSTk-master/core/lib/*.hpp" )
file( GLOB_RECURSE GPSTK_EXT_INC_FILES "20_GPSTk-master/ext/lib/*.h" "20_GPSTk-master/ext/lib/*.hpp" )
  
#----------------------------------------
# Define /core library include directories
#----------------------------------------

# initialize list of include directories
set( GPSTK_CORE_INC_DIRS "" )

foreach( _headerFile ${GPSTK_CORE_INC_FILES} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND GPSTK_CORE_INC_DIRS ${_dir} )
endforeach()


list( REMOVE_DUPLICATES GPSTK_CORE_INC_DIRS )

set( GPSTK_EXT_INC_DIRS "" )

 foreach( _headerFile ${GPSTK_EXT_INC_FILES} )
   get_filename_component( _dir ${_headerFile} PATH )
   get_filename_component( _name ${_headerFile} NAME )
   list( APPEND GPSTK_EXT_INC_DIRS ${_dir} )
 endforeach()

  list( REMOVE_DUPLICATES GPSTK_EXT_INC_DIRS )

add_subdirectory (20_GPSTk-master)
set (EXTRA_LIBS ${EXTRA_LIBS} gpstk_extra)
set (GPSTK_LIBS ${GPSTK_LIBS} gpstk)

#GINAPArser
##############################################################
option( test_GINAParser "HELP: Build test of GINA Parser: SWITCH, Default = ON, Build the GINAPArser" OFF )
add_subdirectory (68_Parsers) 
get_property(GINAPARSER_CORE_INC_DIRS DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/68_Parsers" PROPERTY GINAPARSER_CORE_INC_DIRS)
# message("Property for root directory root read from GNSSSIM_INCLUDE_DIRS: " ${GNSSSIM_INCLUDE_DIRS})

#GNSSsimulator
##############################################################
add_subdirectory (50_GNSSSimulator)
get_property(GNSSSIM_INCLUDE_DIRS DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/50_GNSSSimulator" PROPERTY GNSSSIM_INCLUDE_DIRS)
# message("Property for root directory root read from GNSSSIM_INCLUDE_DIRS: " ${GNSSSIM_INCLUDE_DIRS})
 
#IMUSimulator & IMUSimulatorLibrary
##############################################################
option( IMUSimulator_Tests "HELP: IMUSimulatorTest: Default = OFF, Create tests projects of the IMUSimulator." OFF )
add_subdirectory (60_IMUSimulator)
set (EXTRA_LIBS ${EXTRA_LIBS} IMUSimulatorLibrary)

# GNSS Competence Team's Projects
##############################################################
option( GNSS_Competence_Team_Project_1 "HELP: BUILD_EXC1: SWITCH, Default = OFF, Build the GNSS Competence Team - 1st exercise" OFF )
option( GNSS_Competence_Team_Project_2 "HELP: BUILD_EXC1: SWITCH, Default = OFF, Build the GNSS Competence Team - 2nd exercise" OFF )
option( GNSS_Competence_Team_Project_3 "HELP: BUILD_EXC1: SWITCH, Default = OFF, Build the GNSS Competence Team - 3rd exercise" OFF )
add_subdirectory (40_GNSS_Competence_Team) 

add_subdirectory (69_GINA)

option( EGNOS_RUNNABLES_EMS2IONEX "HELP: BUILD_EGNOS_RUNNABLES EMS2IONEX converter: SWITCH, Default = ON, Build the EMS 2 IONEX converter" ON )
option( EGNOS_RUNNABLES_COMPARE_IONEX_FILES "HELP: EGNOS_RUNNABLES_COMPARE_IONEX_FILES compare two IONEX files: SWITCH, Default = ON, Build the compare two IONEX files" ON )
option( EGNOS_RUNNABLES_PL_ENGINE "HELP: EGNOS Protection level engine: SWITCH, Default = ON, calculate PL from EMS file" ON )
add_subdirectory (70_EGNOS_Project)




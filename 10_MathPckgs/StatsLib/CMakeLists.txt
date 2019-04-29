
file(GLOB_RECURSE STATS_INCL "${CMAKE_CURRENT_SOURCE_DIR}/StatsLib-2.2.0/include/*.hpp")

# initialize list of include directories
set( STATSLIB_CORE_INC_DIRS_LOCAL "" )

foreach( _headerFile ${STATS_INCL} )
    get_filename_component( _dir ${_headerFile} PATH )
    list( APPEND STATSLIB_CORE_INC_DIRS_LOCAL ${_dir} )
endforeach()

##################
# Set Properties #
##################
#####################################################################################################################################
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY STATSLIB_CORE_INC_DIRS ${STATSLIB_CORE_INC_DIRS_LOCAL})

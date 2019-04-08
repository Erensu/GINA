

add_executable(EMS2IONEX EMS2IONEX.cpp)

target_link_libraries (EMS2IONEX 			PUBLIC EGNOS
											PUBLIC gpstk 
											PUBLIC GINAParser)


											
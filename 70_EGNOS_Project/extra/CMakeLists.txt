

add_executable(EGNOS_Runnable_1 main.cpp)

target_link_libraries (EGNOS_Runnable_1 	PUBLIC EGNOS
											PUBLIC gpstk 
											PUBLIC GINAParser)


											
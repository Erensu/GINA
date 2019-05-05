

if( EGNOS_RUNNABLES_EMS2IONEX )
	add_executable(EMS2IONEX EMS2IONEX.cpp)
	target_link_libraries (EMS2IONEX 			PUBLIC EGNOS
												PUBLIC gpstk 
												PUBLIC GINAParser)
endif()
		
if( EGNOS_RUNNABLES_COMPARE_IONEX_FILES )		
	add_executable(CompareIONEX CompareIONEX.cpp)
	target_link_libraries (CompareIONEX 	PUBLIC EGNOS
											PUBLIC gpstk 
											PUBLIC GINAParser)
endif()

		
if( EGNOS_RUNNABLES_PL_ENGINE )		
	add_executable(EGNOS_PL_Engine EGNOS_PL_Engine.cpp)
	target_link_libraries (EGNOS_PL_Engine 	PUBLIC EGNOS
											PUBLIC gpstk 
											PUBLIC GINAParser)
endif()											

if( PEGASUS_IONO_UTIL )		
	add_executable(PegasusIONEX PegasusIonoUtility.cpp)
	target_link_libraries (PegasusIONEX	PUBLIC EGNOS
										PUBLIC gpstk 
										PUBLIC GINAParser)
endif()	
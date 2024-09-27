set(INCLUDES
  ${ROOT_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/build/include
  ${CMAKE_SOURCE_DIR}/hipo/hipo4)
include_directories(${INCLUDES})
link_directories(${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_VERBOSE_MAKEFILE ON)


################################  CalculatePedestals ##########################################
add_executable(CalculatePedestals.exe CalculatePedestals.cc)
target_link_libraries(CalculatePedestals.exe Hipo4 urwellLDRDlib ${ROOT_LIBRARIES} )
#################################################################################################

################################  SkimZeroSuppression ##########################################
add_executable(SkimZeroSuppression.exe SkimZeroSuppression.cc)
target_link_libraries(SkimZeroSuppression.exe Hipo4 urwellLDRDlib ${ROOT_LIBRARIES} )
#################################################################################################

################################  AnaData ##########################################
add_executable(AnaData.exe AnaData.cc)
target_link_libraries(AnaData.exe Hipo4 urwellLDRDlib ${ROOT_LIBRARIES} )
#################################################################################################

################################ DrawBasicPlots ##########################################
add_executable(DrawBasicPlots.exe DrawBasicPlots.cc)
target_link_libraries(DrawBasicPlots.exe Hipo4 urwellLDRDlib ${ROOT_LIBRARIES} )
#################################################################################################


################################ Installing Executable targets #############################
install( TARGETS CalculatePedestals.exe SkimZeroSuppression.exe AnaData.exe DrawBasicPlots.exe
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

################################ Installing Non target files ###############################

################################ Installing needed directores ##############################
install( DIRECTORY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/PedFiles)
install( DIRECTORY
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/Figs )



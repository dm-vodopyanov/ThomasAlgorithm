include_directories ("${PROJECT_SOURCE_DIR}/src/Version_TBB")

add_executable (Version_TBB main_tbb.cpp)

install (TARGETS Version_TBB
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
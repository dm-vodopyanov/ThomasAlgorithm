include_directories ("${PROJECT_SOURCE_DIR}/src/Version_Cilk")

add_executable (Version_Cilk main_cilk.cpp)

install (TARGETS Version_Cilk
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
include_directories ("${PROJECT_SOURCE_DIR}/src/MatrixGeneration")

add_executable (MatrixGeneration main_gen_matrix.cpp)

install (TARGETS MatrixGeneration
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
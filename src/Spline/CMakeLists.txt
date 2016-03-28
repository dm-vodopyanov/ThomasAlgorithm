include_directories ("${PROJECT_SOURCE_DIR}/src/Spline")

add_executable (Spline main_spline.cpp)

install (TARGETS Spline
		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)
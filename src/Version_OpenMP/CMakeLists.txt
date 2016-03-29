include_directories ("${PROJECT_SOURCE_DIR}/src/Version_OpenMP")

add_executable (Version_OpenMP main_openmp.cpp)

install (TARGETS Version_OpenMP
         RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)

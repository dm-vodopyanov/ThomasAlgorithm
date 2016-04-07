include_directories ("${PROJECT_SOURCE_DIR}/src")

add_executable (MatrixGeneration main_gen_matrix.cpp)

add_executable (Version_OpenMP main_openmp.cpp)

add_executable (Version_Seq main_seq.cpp)

add_executable (Version_TBB main_tbb.cpp)

add_executable (Version_Cilk main_cilk.cpp)

add_executable (Spline main_spline.cpp)

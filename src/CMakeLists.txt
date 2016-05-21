include_directories ("${PROJECT_SOURCE_DIR}/src")

add_executable (MatrixGeneration main_gen_matrix.cpp)

add_executable (Version_OpenMP openmp_version.cpp main.cpp)

add_executable (Version_Seq seq_version.cpp main.cpp)

add_executable (Version_TBB tbb_version.cpp main.cpp)

add_executable (Version_Cilk cilk_version.cpp main.cpp)

add_executable (Spline main_spline.cpp)

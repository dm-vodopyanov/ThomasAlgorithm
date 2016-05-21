## Thomas Algorithm Demo application

**Dmitry Vodopyanov, Vasily Kumaev**  
*Lobachevsky University, Nizhny Novgorod, Russia*


### Overview

This application provides four implementations of [Thomas algorithm](https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm) using the technologies below:

- Sequintal (common C++ libraries)
- OpenMP
- Intel(R) Threading Building Blocks
- Intel(R) Cilk(TM) Plus

This algorithm solves the problem of finding the interpolating square spline.


### Project structure and launch diagram

```
----------------------------------------------------------------------------
|                             MatrixGeneration                             |
----------------------------------------------------------------------------
      ||                   ||                  ||                  ||
      \/                   \/                  \/                  \/
---------------    ------------------    ---------------    ----------------
| Version_Seq |    | Version_OpenMP |    | Version_TBB |    | Version_Cilk |
---------------    ------------------    ---------------    ----------------
      ||                   ||                  ||                  ||
      \/                   \/                  \/                  \/
----------------------------------------------------------------------------
|                                  Spline                                  |
----------------------------------------------------------------------------
```

***MatrixGeneration*** project generates tridiagonal matrices and writes them to file.  
***Version_Seq***, ***Version_OpenMP***, ***Version_TBB***, ***Version_Cilk*** projects get the MatrixGeneration's file, make calculations using one of the technologies describing above, and create two files: the file with result vector and the file with execution time of the implementation.  
***Spline*** project implements the logic of finding the interpolating square spline and executes each of Version_### project.  


### BKMs

#### Solution configuration
1. Configure the solution using CMake for Visual Studio
2. Open Visual Studio and make sure that all your projects compile with Intel(R) C++ Compiler
3. ***For all projects:*** Go to Properties -> Debugging -> Working Directory and add set the value ```$(ProjectDir)/../../../bin```
4. ***For all projects:*** Go to Properties -> C/C++ -> Language [Intel C++] -> OpenMP Support -> Generate Parallel Code (/Qopenmp)
5. ***For MatrixGeneration project:*** Go to Properties -> Debugging -> Command Arguments and set value (example) ```5 0 1 inputfile```, where ```5``` is dimension of the matrix, ```0``` and ```1``` are minimum and maximum elements of the matrix, ```inputfile``` is name of the file that will be created
6. ***For all Version_### projects:*** Go to Properties -> Debugging -> Command Arguments and set value ```inputfile outputfile timefile```  
  where ```###``` is ```Seq```, ```OpenMP```, ```TBB``` or ```Cilk```

#### Compile and run using CLI
##### Automation
1. Execute test/run.py with the following command (example): ```python run.py 1000 10 15 seq openmp```
2. Pros: 
  1. it is easy to run,
  2. you can see comparison between versions (execution time, unconsistency in the results)
3. Cons:
  1. if your Intel(R) Parallel Studio XE or Intel(R) Sustem Studio installed in non-default folder, you should change the path in run.py. Default paths: Windows - ```C:\Program Files (x86)\IntelSWTools```, Linux - ```/opt/intel```,
  2. it is very "hardcoded" script, something can goes wrong

##### Manual
###### Windows
1. Set the environment of Intel(R) Parallel Studio XE  
  ```C:\Program Files (x86)\IntelSWTools\compilers_and_libraries\windows\bin\compilervars.bat intel64```
2. Go to ThomasAlgorithm\src folder
3. Create file with matrix:  
  ```icl.exe main_gen_matrix.cpp /O2 /D NDEBUG```  
  ```main_gen_matrix.exe <dimension> <minimum> <maximum> <filename>```
4. Compile and run Sequintal, OpenMP, Intel(R) TBB or Intel(R) Cilk(TM) Plus version:  
  ```icl.exe ###_version.cpp main.cpp /O2 /D NDEBUG /Qopenmp /link tbb.lib```  
  ```###_version.exe inputfile outputfile timefile```,  
  where ```###``` is ```seq```, ```openmp```, ```tbb``` or ```cilk```

###### Linux and OS X
1. Set the environment of Intel(R) Parallel Studio XE or Intel(R) System Studio (Linux only)  
  ```source /opt/intel/compilers_and_libraries/***/bin/compilervars.sh intel64```,  
  where ```***``` is ```linux``` or ```mac```
2. Go to ThomasAlgorithm/src folder
3. Create file with matrix:
  ```icc -O2 -DNDEBUG -m64 -qopenmp -o main_gen_matrix ../src/main_gen_matrix.cpp```  
  ```./main_gen_matrix <dimension> <minimum> <maximum> <filename>```
4. Compile and run Sequintal, OpenMP, Intel(R) TBB or Intel(R) Cilk(TM) Plus version:  
  ```icc ###_version.cpp main.cpp -O2 -DNDEBUG -m64 -qopenmp -ltbb -o ###_version```  
  ```./###_version inputfile outputfile timefile```,  
  where ```###``` is ```seq```, ```openmp```, ```tbb``` or ```cilk```

#### Development process
1. **AR Vasily**: Please make sure that the version of the file you contribute is the latest: ***as frequently as you can execute ```git pull```***

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


### Project structure and hierarchy

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
5. ***For MatrixGeneration project:*** Go to Properties -> Debugging -> Command Arguments and set value (example) ```5 0 1 inputfile```, where ```5``` is dimension, ```0``` and ```1``` are minimum and maximum elements of the matrix, ```inputfile``` is name of the file that will be created
6. ***For all Version_### projects:*** Go to Properties -> Debugging -> Command Arguments and set value ```inputfile outputfile timefile```   

#### Development process
1. Please make sure that the version of the file you contribute is the latest: ***as frequently as you can execute ```git pull```***
2. To run the script in test folder you should execute the following command (example): ```python run.py 1000 10 15 seq openmp```
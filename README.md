

SimpleTaskManager

Implemented following features:
1. Non disposal multytasking
2. Task prioritization

Implemented 2 tasks with following functionality:
1. First
   1.1 Get string from console (no more than 100 symbols)
   1.2 Put the string on console in reverse order
2. Second 
   2.1 put overall task execute time on console every 5 seconds

Both output of tasks and input string don't overlap on console


BULDING

make - build and link TaskManagerTest software

make clean - remove all *.o files

make doc - building doxygen documentation

make run - call TaskManagerTest software with some tested parameters


Files:

TaskManager.c - low level reazation of task manager
TaskManagerImpl.h - header file of low-level realisation. In case C++ builder all function plasing in Lowlevel namespace

TaskManager.cpp - C++ wrapper of low level functions
TaskManager.hpp - heager file with declaration of C++ wrapper

main.cpp - simple test software.

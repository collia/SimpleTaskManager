
/**
 * @file TaskManager.hpp
 * File contain declaration of class wrapper of low lewel task manager functions  
 *
 * @brief SimpleTask class is hi level wrapper of task manaher functionality
 *
 * @author Nikolay Klimchuk
 * @version 0.01
 
 */


#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP


#include "TaskManagerImpl.h"

/**
 *  Class - error
 */
class ThreadError
{
};



class SimpleTask
{
private:
	LowLevel::Task* task;
	
public:

	SimpleTask(int priority, LowLevel::TaskCallback) throw(ThreadError);
	void Sleep(int time);
	void Run();
	virtual ~SimpleTask();
	
};

#endif //TASKMANAGER_HPP

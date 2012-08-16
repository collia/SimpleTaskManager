
/**
 * @file TaskManager.cpp
 * Realisation of TaskManager task
 *
 * @brief In file realised mhetods of SimpleTaskManager class
 *
 * @author Nikolay Klimchuk
 * @version 0.01
 
 */

#include "TaskManager.hpp"

/**
 * Constructor 
 *    @param[in] priority - Task priority
 *    @param[in] callback - pointer to callback function
 */

SimpleTask::SimpleTask(int priority, LowLevel::TaskCallback callback) throw(ThreadError)
{
	task = LowLevel::CreateTask(priority, callback);
	if (task == 0)
		throw ThreadError();
}

/**
 * Function which stops current task
 * @param[in] time- number of millisseconds to sleep
 */

void SimpleTask::Sleep(int time)
{
	LowLevel::Sleep(task, time);
}

/**
 *  Function which starts thread
 */

void SimpleTask::Run()
{
	LowLevel::Run(task);
}

/**
 *   Destructor
 */

SimpleTask::~SimpleTask()
{
	int res = LowLevel::DestroyTask(task);
	if (res != 0)
		throw ThreadError();
}

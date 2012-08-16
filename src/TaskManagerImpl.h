
/**
 * @file TaskManagerImpl.h
 * Header file with declare low level task meneger funcrions
 *
 * @brief In case c++ all function plasing in "LowLevel" namespace
 *
 * @author Nikolay Klimchuk
 * @version 0.01
 
 */


#ifndef TASKMANAGERIMPL_H
#define TASKMANAGERIMPL_H


#ifdef __cplusplus
namespace LowLevel
{
#endif



typedef void (*TaskCallback)();


/
#define MAX_PRIORITY 10	
#define STACK_SIZE 10000

/**
 *  Task structure
 */
typedef struct tTask
{
	
	TaskCallback callback; ///< pointer to callback function
	int priority;
	unsigned long long wakeupTime;  ///< time when task have to be wake up

	void* stackArea;  ///< pointer on memory for stack
	void* stackPointerESP;  ///< saved value of ESP register while task sleeping
	void* stackPointerEBP;  ///< saved value of EBP register while task sleeping
	struct tTask *next;     ///< Pointer on next task in queue
	char isRunned; ///< Field which contains state of task
} Task;


#ifdef __cplusplus
extern "C" 
{
#endif 
	Task* CreateTask (int priority, TaskCallback callback);
	void TaskManager();
	int DestroyTask(Task *task);
	
	void Run(Task* task);
	void Sleep (Task* task, int ticks);
	unsigned long long getTime();
#ifdef __cplusplus
}
#endif 

#ifdef __cplusplus
} //namespace
#endif


#endif //TASKMANAGERIMPL_H

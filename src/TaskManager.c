
/**
 * @file TaskManager.c
 * File contains low lewel realization of task meneger
 *
 * @brief Dile contains set of C functions which realisating low level functionality.
 *
 * @author Nikolay Klimchuk
 * @version 0.01
 
 */


#include "TaskManagerImpl.h"

#include <stdio.h>
#include <time.h>
#include <stdint.h>


static Task* iHead;

void* malloc(int);
void free(void*);
int clock_gettime(clockid_t clock_id, struct timespec *tp);

/**
      Function allocating memory for task and input task in tasks queue.
   In error case returns 0. 
   Priority is place in queue. 
      @param[in]  priority - task priority. Possible values from 0 to (MAX_PRIORITY-1)  
      @param[in]  callback - pointer to function-callback. When time is come this function will be called
      
      @return pointer to new task, or 0 in error case
 */

Task* CreateTask (int priority, TaskCallback callback)
{
	
	Task* newTask  = (Task*)  malloc (sizeof(Task));
	if( newTask == 0 ) return 0;
	if ((newTask->stackArea = malloc(STACK_SIZE)) == 0)
	{ 
		free(newTask);
		return 0;
	}
	newTask->stackPointerESP = newTask->stackPointerESP  = newTask->stackArea + STACK_SIZE; 
	
	newTask->callback = callback;
	newTask->wakeupTime = 0;
	if (priority < MAX_PRIORITY)
	{
		newTask->priority = priority;
	}
	else
	{
		free(newTask->stackArea);
		free(newTask);
		return 0;
	}

	newTask->isRunned = 0;
		
	Task *temp = iHead;
		
	if(iHead != 0)
	{
		while ((temp->next != iHead) && 
		       !((temp->priority <= priority) &&
			 (temp->next->priority >= priority)))
			temp = temp->next;
		
		newTask->next = temp->next;
		temp->next = newTask;
		if (iHead->priority >= priority)
			iHead = newTask;
	} 
	else 
	{
		iHead = newTask;
		iHead->next = iHead;
	}
	return newTask;
}

/**
   Function removes task from queue, and frees memory.
   @param[in] task - task to destroy
   @return error state 0 - normal state, all other values is error
 */
int DestroyTask(Task *task)
{
	Task *temp = iHead;
		
	if(iHead != 0)
	{
		while(temp->next != task && temp->next != iHead) 
			temp = temp->next;
		if(temp->next == task)
		{
			temp->next = task->next;
			task->isRunned = 0;
			task->next = 0;
			free(task->stackArea);
			free(task);
			if(iHead == task)
				iHead = temp;
			if(temp == task)
				iHead = 0;
			
		} else return 2; //error
	} 
	else return 1; // error
	return 0; // OK
}

/**
       Main function of task manager. Function save stack pointer and calls callback of supervisor task. When supervisor task is finish, function restore stack
 */

void TaskManager()
{
	// save stack
	asm volatile (
		"xor %%esp, %0\n"
		"xor %0, %%esp\n"
		"xor %%esp, %0\n"
		
		"xor %%ebp, %1 \n"
		"xor %1, %%ebp \n"
		"xor %%ebp, %1 \n"
		
		"pushl %%eax \n"
		"pushl %%ebx \n" ::
		"a" (iHead->stackPointerESP),
		"b" (iHead->stackPointerEBP) :
		 "memory");
	 
	
	iHead->callback();
	
	// restore stack
	asm volatile (
		"popl %%ebp \n"
		"popl %%esp \n" :::
		);
}

/**
       Function returns system time in ms. Time is system uptime.
       @return system time in ms
 */
unsigned long long getTime()
{
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (((long long)((long long)time.tv_sec * 1000000000L) + time.tv_nsec) / 1000000L) & 0x7FFFFFFFFFFFFFFFL;

} 

/**
   Function saves current stack and changes current stack to newTask pointer 
   @param[in] currTask - current task, in which have to be saved stack
   @param[in[ newTask - new task
 */

void swapTask(Task* currTask, Task* newTask)
{

	register void *newESP=newTask->stackPointerESP,
		*newEBP=newTask->stackPointerEBP;
	register void *oldESP, *oldEBP;
	// save stack
	asm volatile (
		"mov %%esp, %0 \n"
		"mov %%ebp, %1 \n"
		:
		"=a" (oldESP), 
		"=b" (oldEBP)
		::
		"memory");

	currTask->stackPointerESP = oldESP;
	currTask->stackPointerEBP = oldEBP;
	
	if ((newEBP - newESP) != (oldEBP - oldESP))
	{
		newESP = newEBP - (oldEBP - oldESP);
	}
	//load stack
	asm volatile (
		"mov %0, %%esp \n"
		"mov %1, %%ebp \n"
		: :
		  "a" (newESP),
		  "b" (newEBP) :
		"memory");
		  


}

/**
   Function stop current task and search in queue next task.
   @param[in] task - current Task
   @param[in] tics - number of tics to task sleep
 */

void Sleep (Task* task, int ticks)
{
	unsigned long long currTime = getTime();
	task->wakeupTime = ticks + currTime;
	Task* temp = iHead;
	
	while (temp->wakeupTime > currTime || !temp->isRunned)
	{
		temp = temp->next;
		currTime = getTime();
	}
	if(task != temp)
	{
		swapTask(task, temp);
	}
}

/**
   Function forms stack and starts task
   @param[in] task - task to start
 */

void Run(Task* task)
{
	volatile void* newStackpointer = task->stackPointerESP;
	volatile TaskCallback callback = task->callback;

	 void *esp, *ebp;
	 // forming stack
	asm volatile (
		"movl %%esp, %%ecx \n"
		"movl %%ebp, %%edx \n" 
		
		"movl %0, %%esp \n"
	 	"pushl %1 \n"
		"movl %%esp, %%ebp \n"
		"pushl %%ebp \n"
		"movl %%esp, %%ebp \n" 

		"xor %%ecx, %%esp \n"
		"xor %%esp, %%ecx \n"
		"xor %%ecx, %%esp \n"
		
		"xor %%edx, %%ebp \n"
		"xor %%ebp, %%edx \n"
		"xor %%edx, %%ebp \n"
		
		"mov %%ecx, %0 \n"
		"mov %%edx, %1 \n" :
		"=a" (esp), 
		"=b" (ebp)
		:
		
		"a" (newStackpointer),
		"b" (callback) :
		"memory");

	
	task->stackPointerESP = esp;
	task->stackPointerEBP = ebp;

	task->isRunned = 1;
	
}

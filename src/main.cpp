
/**
 * @file main.cpp
 * Main file which show usage of TaskManager library.
 *
 * @brief In main function creating two tasks (depends on input parameters). One task gets string from stdin and prints reversed. Second task prints system uptime every 5 sec
 *
 * @author Nikolay Klimchuk
 * @version 0.01
 
 */

#include <iostream>
#include <sstream>
#include "TaskManager.hpp"

/// Structure for supervisor task
struct SupervisorTask
{
	static SimpleTask *task; ///< pointer to task
	static void callback();  ///< static callback function
};

/// Structure for IO task
struct Task1
{
	static int stopTask;     ///< Flag of stopping all tasks
	static SimpleTask *task; ///< pointer to task
	static void callback();  ///< static callback function  
};

/// Structure for timer task
struct Task2
{
	static SimpleTask *task; ///< pointer to task
	static void callback();  ///< static callback function
};

// initialising of static fields
SimpleTask* SupervisorTask::task = 0;
SimpleTask* Task1::task = 0;
SimpleTask* Task2::task = 0;
int Task1::stopTask = 0;


/**
 * Realisation of callback function of supervisor task
 */
void SupervisorTask::callback()
{
	std::cout << "SupervisorTask" << std::endl;

	if(Task1::task)
		Task1::task->Run();
	if(Task2::task)
		Task2::task->Run();
	
	while(!Task1::stopTask)
	{
		task->Sleep(500);
	}

	
}

/**
 * Function for reverce string
 *  @param[in,out] val - for reverse
 */
void  ReverseString(std::string& val)
{
	char temp;
	int length = val.size()-1;
	for(int i = 0; i < length / 2 + length % 2; i++)
	{
		temp = val[i];
		val[i] = val[length - i];
		val[length - i] = temp;
	}
}


/**
 * Callback for input-output task
 */
void Task1::callback()
{
	int i = 0;
	for(;;)
	{

		task->Sleep(500);
		std::string val;
		std::cin >> val;
		ReverseString(val);
		std::cout << val << "\n";
		if (i++ > 5)
			stopTask = 1;
	}
}

/**
 * callback for timer task
 */

void Task2::callback()
{
	for(;;)
	{
		std::cout << "Time: " << LowLevel::getTime() <<  std::endl;
		task->Sleep(5000);
	}
}

/**
 * Function prints help message  
 */

void PrintHelp ()
{
	std::cout << "Usage: \n \n"; 
	std::cout << "\t TaskManagerTest [-tio priority] [-tt priority] [-h]\n";
	std::cout << "\t -tio priority - start input - output task with priority \n";
	std::cout << "\t -tt priority - start timer task with priority \n";
	std::cout << "\t -h - show this message \n";
}


/**
 * Main function 
 */

int main(int args, const char* argv[])
{
	try
	{
		SupervisorTask::task = new SimpleTask(0, SupervisorTask::callback);
		
	}
	catch(ThreadError)
	{
		std::cout << "Internal error while supervisor task creating" << std::endl;
		return 3;
	}


	for(int i = 1; i < args; i++)
	{
		
		if(std::string(argv[i]) == "-tio")
		{
			if(i+1 < args)
			{
				int val;
				std::istringstream ( std::string( argv[i+1]) ) >> val;
				
				try
				{
					Task1::task = new SimpleTask(val, Task1::callback);
				}
				catch(ThreadError)
				{
					std::cout << "Internal error while task creating" << std::endl;
					return 2;
				}

			} 
			else
			{
				PrintHelp();
				return 1;
			}

		}
		if(std::string(argv[i]) == "-tt")
		{
			if(i+1 < args)
			{
				int val;// = itoa(argv[i+1]);
				std::istringstream ( std::string( argv[i+1]) ) >> val;
				try
				{

					Task2::task = new SimpleTask(val, Task2::callback);
				}
				catch(ThreadError)
				{
					std::cout << "Internal error while task creating" << std::endl;
					return 2;
				}
			}
			else
			{
				PrintHelp();
				return 1;
			}
		}
		if (std::string(argv[i]) == "-h")
		{
			PrintHelp();
			return 0;
		}
			
	}

	// start supervisor task
	SupervisorTask::task->Run();


        // Start main task of Task manager
	LowLevel::TaskManager();

}

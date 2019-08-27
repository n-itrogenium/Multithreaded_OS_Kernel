#include "thread.h"
#include "semaphor.h"
#include <stdlib.h>
#include <dos.h>
#include <stdio.h>
#include <stdarg.h>

/*
 	 Test: Semafori bez realizacije spavanja
*/

/*
int syncPrintf(const char *format, ...) {
	int res;
	va_list args;
    lock
    va_start(args, format);
    res = vprintf(format, args);
    va_end(args);
    unlock
    return res;
}


const int n = 5;
int count = 10;

Semaphore s(2);

class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	s.wait(0);
	syncPrintf("Thread %d in critical section.\n", getId());
	for(unsigned int i=0;i<64000;i++)
		for(unsigned int j=0;j<64000;j++);
	s.signal();
}

void tick(){ }

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i].start();
	}
	for(i=0;i<n;i++)
	{
		t[i].waitToComplete();
	}
	syncPrintf("Test ends.\n");
	return 0;
}

*/

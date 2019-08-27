#include "thread.h"
#include "semaphor.h"
#include <stdlib.h>
#include <dos.h>
#include <stdio.h>
#include <stdarg.h>

/*
 	 Test: Semafori sa spavanjem
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

const int n = 1;
int t=-1;

Semaphore s(0);

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
	syncPrintf("Thread waits for 10 units of time...\n");
	t=0;
	s.wait(10);
	syncPrintf("Thread finished.\n");
	s.signal();
}

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

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

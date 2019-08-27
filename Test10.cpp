#include "thread.h"
#include "semaphor.h"
#include <stdlib.h>
#include <dos.h>
#include <stdio.h>
#include <stdarg.h>

/*
 	 Test: Semafori sa spavanjem 2
*/

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

int t=-1;

Semaphore s(0);

class TestThread : public Thread
{
private:
	Time waitTime;

public:

	TestThread(Time WT): Thread(), waitTime(WT){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d waits for %d units of time.\n",getId(),waitTime);
	int r = s.wait(waitTime);
	s.signal();
	syncPrintf("Thread %d finished: r = %d\n", getId(),r);
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
	TestThread t1(15),t2(10),t3(30);
	t1.start();
	t2.start();
	t3.start();
	s.wait(5);
	s.signal();
	s.wait(0);
	s.signal();
	syncPrintf("Test ends.\n");
	return 0;
}



#include "thread.h"
#include <stdio.h>


/*
 	 Test: idleThread
*/


void tick(){ }

class TestThread : public Thread
{
private:
	TestThread *t;

public:

	TestThread(TestThread *thread): Thread(), t(thread){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	t->waitToComplete();
}


int userMain(int argc, char** argv)
{
	lock
	printf("Test starts.\n");
	unlock

	TestThread *t1,*t2;
	t1 = new TestThread(t2);
	t2 = new TestThread(t1);
	t1->start();
	t2->start();
	delete t1;
	delete t2;

	lock
	printf("Test ends.\n");
	unlock

	return 0;
}



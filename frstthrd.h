#ifndef _frstthrd_h_
#define _frstthrd_h_

#include "thread.h"

class FirstThread : public Thread {
public:
	FirstThread() : Thread() {}
	~FirstThread();
	void inic();
};

#endif

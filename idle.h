#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"
#include "pcb.h"

class Idle : public Thread {
public:
	Idle(): Thread(128, 1) {}
	~Idle();
	virtual void run();
	void start();
};

#endif

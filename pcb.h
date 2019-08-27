/*
 * pcb.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "semaphor.h"
#include <dos.h>
#include "listpcb.h"

typedef unsigned long StackSize;
typedef unsigned int Time;
typedef unsigned Address;
typedef unsigned int TimeFlag;
enum Flag { START, READY, FINISHED, BLOCKED };

class PCB {

public:

	StackSize stackSize;
	Time timeSlice, waitTime;
	Address *stack, ss, sp, bp;
	TimeFlag timeFlag;
	Flag state;

	Semaphore* semWaitingOn;
	Thread* myThread;
	static PCB* running;
	List *waitingToComplete;

	PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
	virtual ~PCB();

	static void wrapper();
	void waitToComplete(Thread *);

};

#endif /* PCB_H_ */

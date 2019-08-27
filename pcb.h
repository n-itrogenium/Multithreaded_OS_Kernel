/*
 * pcb.h
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "list.h"
#include <dos.h>

typedef unsigned long Address;
enum Flag { READY, FINISHED, BLOCKED };

class PCB {

public:

	StackSize stackSize; // ako ne bude potrebno, izbaci
	Time timeSlice; // ako ne bude potrebno, izbaci
	Address *stack, ss, sp, bp;

	Flag state;

	Thread* myThread;
	static PCB* running;
	List *waitingToComplete;

	PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
	virtual ~PCB();

	static void wrapper();

};

#endif /* PCB_H_ */

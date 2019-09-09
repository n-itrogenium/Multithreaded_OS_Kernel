/*
 * pcb.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "pcb.h"
#include <stdio.h>
#include <dos.h>

volatile PCB* PCB::running = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread) {

	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->myThread = myThread;
	semWaitingOn = 0;
	state = START;
	timeExceeded = 1;
	waitTime = 0;

	stackSize /= sizeof(unsigned);
	stack = new Address[stackSize];

	stack[stackSize - 3] = 0x200;

#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 4] = FP_SEG(PCB::wrapper);
	stack[stackSize - 5] = FP_OFF(PCB::wrapper);

	ss = FP_SEG(stack + stackSize - 14);
	sp = FP_OFF(stack + stackSize - 14);
#endif
	bp = sp;

	stack[stackSize - 14] = bp;
}

PCB::~PCB() {
	delete stack;
}

void PCB::waitToComplete(Thread *thread) {
	lock
	if (state == FINISHED || state == START ||
			this == System::idleThread->getMyPCB() ||
			this == System::firstThread->getMyPCB() ||
			this == (PCB*) PCB::running) {
				unlock
				return;
	}
	PCB::running->state = BLOCKED;
	waitingToComplete.add((PCB*) PCB::running);
	dispatch();
	unlock
}

void PCB::wrapper() {
	if (PCB::running->myThread == System::idleThread)
		while (1) { }
	PCB::running->myThread->run();
	PCB::running->state = FINISHED;
	PCB* temp = 0;
	while (PCB::running->waitingToComplete.num_of_nodes != 0) {
		temp = ((PCB*)PCB::running)->waitingToComplete.getFirst();
		temp->state = READY;
		Scheduler::put(temp);
	}
	dispatch();
}



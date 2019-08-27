/*
 * pcb.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "pcb.h"
#include "idle.h"
#include "frstthrd.h"
#include <stdio.h>

PCB* PCB::running = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread) {

	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->myThread = myThread;
	semWaitingOn = 0;
	waitingToComplete = new List();
	state = BLOCKED;
	timeFlag = 1;
	waitTime = 0;

	stackSize /= sizeof(unsigned short); // Data = 2B, unsigned short = 2B
	stack = new Address[stackSize];

	// Na stack[stackSize - 1] i stack[stackSize - 2] su segment i offset f-je callback,
	// koja se poziva nakon metode wrapper (koja ipak nikad nece doci do kraja)

	stack[stackSize - 3] = 0x200; // Enable I flag

	// Podmecemo adresu javnog statickog wrapper-a,
	// koji ce posredno da poziva run() tekuce niti:
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 4] = FP_SEG(PCB::wrapper);
	stack[stackSize - 5] = FP_OFF(PCB::wrapper);
#endif

	// Od stack[stackSize - 6] do stack[stackSize - 13] su mesta za registre procesora 
#ifndef BCC_BLOCK_IGNORE
	ss = FP_SEG(stack + stackSize - 14); // Stek pocinje od ove lokacije
	sp = FP_OFF(stack + stackSize - 14);
#endif
	bp = sp;

	stack[stackSize - 14] = bp;

}

PCB::~PCB() {
	delete stack;
}

void PCB::waitToComplete(Thread *thread) {
	//printf("Thread %d pozvala waitToComplete za Thread %d \n",PCB::running->myThread->getId(),thread->getId());
	if (state == FINISHED || state == START ||
			this == System::idleThread->getMyPCB() ||
			this == System::firstThread->getMyPCB() ||
			this == PCB::running) {
				//printf("waitToComplete se ignorise! \n");
				unlock
				return;
	}
	waitingToComplete->add(PCB::running);
	PCB::running->state = BLOCKED;
	//printf("Thread %d se blokirala i vrsi se promena konteksta. \n",PCB::running->myThread->getId());
	dispatch();
	unlock
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	PCB::running->state = FINISHED;
	lock
	//printf("Thread %d je zavrsila! \n",PCB::running->myThread->getId());
	unlock
	PCB* temp = 0;
	while (!PCB::running->waitingToComplete->isEmpty()) {
		temp = PCB::running->waitingToComplete->getFirst();
		lock
		//printf("Iz liste uzeta thread %d \n",temp->myThread->getId());
		unlock
		temp->myThread->start();
	}
	dispatch();
}

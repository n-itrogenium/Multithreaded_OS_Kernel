/*
 * pcb.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "pcb.h"

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* myThread) {

	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->myThread = myThread;
	waitingToComplete = new List();
	state = BLOCKED;

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

}

PCB::~PCB() {
	// TODO Auto-generated destructor stub
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	List::Node *temp = running->waitingToComplete->head;
	while (temp) {
		temp->pcb->myThread->start();
		temp = temp->next;
	}
	running->state = FINISHED;
	dispatch();
}

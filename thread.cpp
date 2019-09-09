/*
 * thread.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "thread.h"
#include "system.h"
#include "pcb.h"
#include <stdio.h>
#include "listpcb.h"
#include "krnlsem.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	Id = ++staticID;
	myPCB = new PCB(stackSize, timeSlice, this);
	myPCB->state = START;
	System::threads->add(myPCB);
}

PCB * Thread::getMyPCB() {
	return myPCB;
}

void Thread::start() {
	if (myPCB->state != READY && myPCB->stack != 0) {
		myPCB->state = READY;
		Scheduler::put(myPCB);
	}
}

void Thread::waitToComplete() {
	if (this != System::idleThread)
		myPCB->waitToComplete(this);
}

Thread::~Thread() {
	myPCB->semWaitingOn->myImpl->total.remove(myPCB);
	myPCB->semWaitingOn->myImpl->limitedTime.remove(myPCB);
	System::threads->remove(myPCB);
	delete myPCB;
}

ID Thread::getId() { return Id; }

ID Thread::getRunningId() {
	return PCB::running->myThread->getId();
}

Thread * Thread::getThreadById(ID id) {
	return System::threads->get(id)->myThread;
}

ID Thread::staticID = 0;

void dispatch() { 
	asm cli;
	System::context_on_demand = 1;
	timer();
	asm sti;
}

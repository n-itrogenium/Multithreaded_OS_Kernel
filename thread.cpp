/*
 * thread.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "thread.h"
#include "list.h"
#include "system.h"
#include "pcb.h"
#include <stdio.h>

Thread::Thread(StackSize stackSize, Time timeSlice) {
	Id = ++staticID;
	myPCB = new PCB(stackSize, timeSlice, this);
	threads->add(myPCB);
}

PCB * Thread::getMyPCB() {
	return myPCB;
}

void Thread::start() {
	myPCB->state = READY;
	Scheduler::put(myPCB);
}

void Thread::runIdle() {
	while (1);
}

void Thread::waitToComplete() {
	myPCB->waitingToComplete->add(PCB::running);
	PCB::running->state = BLOCKED;
	dispatch();
	// SREDI OVO PICKO
}

Thread::~Thread() {
	waitToComplete();
	threads->remove(Id);
}

ID Thread::getId() { return Id; }

ID Thread::getRunningId() {
	return PCB::running->myThread->getId();
}

Thread * Thread::getThreadById(ID id) {
	return threads->get(id)->myThread;
}

ID Thread::staticID = 0;

void dispatch() { 
	asm cli;
	System::context_on_demand = 1;
	timer();
	asm sti;
}

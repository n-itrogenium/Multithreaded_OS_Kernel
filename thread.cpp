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
	myPCB->state = START;
	System::threads->add(myPCB);
}

PCB * Thread::getMyPCB() {
	return myPCB;
}

void Thread::start() {
	myPCB->state = READY;
	Scheduler::put(myPCB);
}

void Thread::waitToComplete() {
	myPCB->waitToComplete(this);
}

Thread::~Thread() {
	waitToComplete();
	System::threads->remove(Id);
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

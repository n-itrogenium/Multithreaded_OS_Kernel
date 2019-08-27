/*
 * thread.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "thread.h"

Thread::Thread(StackSize stackSize = defaultStackSize, Time timeSlice =
			defaultTimeSlice) {
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

void Thread::waitToComplete() {
	myPCB->waitingToComplete->add(PCB::running);
	PCB::running->state = BLOCKED;
	dispatch();
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
	context_on_demand = 1;
	timer();
	asm sti;
}
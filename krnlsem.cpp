/*
 * krnlsem.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "krnlsem.h"
#include "pcb.h"

int KernelSem::staticID = 0;

KernelSem::KernelSem(int init, Semaphore* mySem) {
	ID = ++staticID;
	value = init;
	this->mySem = mySem;
	waiting = new List();
}

KernelSem::~KernelSem() {
	delete waiting;
}

int KernelSem::val() const {
	return value;
}

void KernelSem::incVal() {
	value++;
}

int KernelSem::wait(Time maxTimeToWait) {
	lock
	if (--value < 0) {
		PCB::running->state = BLOCKED;
		PCB::running->semWaitingOn = mySem;
		waiting->add(PCB::running);
		if (maxTimeToWait > 0) {
			PCB::running->waitTime = maxTimeToWait;
			System::blockedThreads->add(PCB::running);
		}
		dispatch();
	}
	unlock
	return PCB::running->timeFlag;
}

int KernelSem::signal(int n) {
	lock
	if (n<0) {
		unlock
		return n;
	}
	PCB* temp;
	if (n==0) {
		if (!waiting->isEmpty()) {
			temp = waiting->getFirst();
			temp->waitTime = 0;
			temp->timeFlag = 1;
			temp->semWaitingOn = 0;
			System::blockedThreads->remove(temp);
			temp->myThread->start();
		}
		value++;
		unlock
		return 0;
	}
	if (n>0) {
		int ret = 0;

		for (int i = 0; i < n || waiting->isEmpty(); i++) {
			temp = waiting->getFirst();
			temp->waitTime = 0;
			temp->timeFlag = 1;
			temp->semWaitingOn = 0;
			System::blockedThreads->remove(temp);
			temp->myThread->start();
			ret++;
		}
		value += n;
		unlock
		return ret;
	}
}

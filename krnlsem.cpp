/*
 * krnlsem.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "krnlsem.h"
#include "pcb.h"

KernelSem::KernelSem(int init, Semaphore* mySem) {
	if (value >= 0) value = init;
	else value = 0;
	this->mySem = mySem;
	System::semaphores->add(this);
}

KernelSem::~KernelSem() {
	PCB* temp;
	while(!total.isEmpty()) {
		temp = total.getFirst();
		temp->timeExceeded = 1;
		temp->semWaitingOn = 0;
		temp->waitTime = 0;
		temp->state = READY;
		Scheduler::put(temp);
	}

	temp = unlimited.head->pcb;
	while(!unlimited.isEmpty())
		temp = unlimited.getFirst();

	temp = limitedTime.head->pcb;
	while(!limitedTime.isEmpty())
			temp = limitedTime.getFirst();

	System::semaphores->remove(this);
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
		total.add((PCB*) PCB::running);
		if (maxTimeToWait == 0)
			unlimited.add((PCB*) PCB::running);
		else {
			PCB::running->waitTime = maxTimeToWait;
			limitedTime.add((PCB*) PCB::running);
		}
		unlock
		dispatch();
		lock
		int ret = PCB::running->timeExceeded;
		PCB::running->timeExceeded = 1;
		unlock
		return ret;
	}
	PCB::running->timeExceeded = 1;
	unlock
	return 1;
}



int KernelSem::signal(int n) {
	lock

	if (n<0) {
		unlock
		return n;
	}

	int num_of_threads;
	if (n == 0) num_of_threads = 1;
	else num_of_threads = n;

	value += num_of_threads;

	PCB* temp = 0;

	int ret = 0;

	if (value <= 0) {
		for (int i = 0; i < num_of_threads; i++) {

			if (!total.isEmpty())
				temp = total.getFirst();
			if (!temp) break;
			if (temp->waitTime)
				limitedTime.remove(temp);
			else
				unlimited.remove(temp);
			temp->waitTime = 0;
			temp->timeExceeded = 1;
			temp->semWaitingOn = 0;
			temp->state = READY;
			Scheduler::put(temp);
			ret++;
		}
		unlock
		return ret;
	}
	unlock
	return 0;
}






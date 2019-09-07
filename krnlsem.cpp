/*
 * krnlsem.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "krnlsem.h"
#include "pcb.h"

int KernelSem::staticID = 0;

KernelSem::KernelSem(Semaphore* mySem, int init = 1) {
	ID = ++staticID;
	//if (value >= 0)
		value = init;
	//else value = 0;
	this->mySem = mySem;
	System::semaphores->add(this);
}

KernelSem::~KernelSem() {
	PCB* temp = 0;
	int counter = total.num_of_nodes;
	while(counter--) {
		temp = total.getFirst();
		if (!temp || !temp->stack) {
			continue;
		}
		temp->timeExceeded = 1;
		temp->semWaitingOn = 0;
		temp->waitTime = 0;
		temp->state = READY;
		Scheduler::put(temp);
	}

	counter = limitedTime.num_of_nodes;
	while(counter--) {
		temp = limitedTime.getFirst();
	}

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
		if (maxTimeToWait > 0){
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

	PCB* temp = 0;

	if (value <= 0) {
		value += num_of_threads;
		int counter = total.num_of_nodes, ret = 0;
		while (counter--) {
			temp = total.getFirst();
			if (!temp) break;
			if (!temp->stack) continue;
			if (temp->waitTime)
				limitedTime.remove(temp);
			temp->waitTime = 0;
			temp->timeExceeded = 1;
			temp->semWaitingOn = 0;
			temp->state = READY;
			//printf("signal KERNELSEM - U scheduler: %d\n",temp->myThread->getId());
			Scheduler::put(temp);
			ret++;
		}
		unlock
		return ret;
	}
	value += num_of_threads;
	unlock
	return 0;
}






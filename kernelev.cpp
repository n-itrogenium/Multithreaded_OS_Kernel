/*
 * kernelev.cpp
 *
 *  Created on: Aug 28, 2019
 *      Author: OS1
 */

#include "kernelev.h"
#include "ivtentry.h"

KernelEv::KernelEv(IVTNo ivtNo, Event* myEvent) {
	this->ivtNo = ivtNo;
	this->myEvent = myEvent;
	this->myCreator = (PCB*)PCB::running;
	value = 1;
	IVTEntry* entry = IVTEntry::IVT[ivtNo];
	entry->setMyEvent(this);

}

KernelEv::~KernelEv() {
	signal();
	IVTEntry::IVT[ivtNo]->setMyEvent(0);
}

void KernelEv::wait() {
	if (PCB::running == myCreator) {
		if (value == 1) {
			value = 0;
			PCB::running->state = BLOCKED;
			dispatch();
		}
		else value = 0;
	}
}

void KernelEv::signal() {
	if (myCreator->state == BLOCKED) {
		value = 1;
		myCreator->state = READY;
		Scheduler::put(myCreator);
		dispatch();
	} else value = 0;
}

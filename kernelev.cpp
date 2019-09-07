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
	blocked = 0;
	value = 1;
	IVTEntry* entry = IVTEntry::IVT[ivtNo];
	//if (entry && !entry->getMyEvent())
		entry->setMyEvent(this);

}

KernelEv::~KernelEv() {
	signal();
	//if (IVTEntry::IVT[ivtNo] && IVTEntry::IVT[ivtNo]->getMyEvent() == this)
		IVTEntry::IVT[ivtNo]->setMyEvent(0);
}

void KernelEv::wait() {
	if (PCB::running == myCreator) {
		if (value == 1) {
			value = 0;
			PCB::running->state = BLOCKED;
			blocked = (PCB*)PCB::running;
			dispatch();
		}
		else value = 0;
	}
}

void KernelEv::signal() {
	if (blocked != 0 && blocked->stack != 0) {
		if (blocked->state == BLOCKED) {
			value = 1;
			blocked->state = READY;
			Scheduler::put(blocked);
			blocked = 0;
			dispatch();
		} else value = 0;
	}
}

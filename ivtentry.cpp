/*
 * ivtentry.cpp
 *
 *  Created on: Aug 28, 2019
 *      Author: OS1
 */

#include "ivtentry.h"
#include <dos.h>

IVTEntry* IVTEntry::IVT[IVT_SIZE] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine) {
	this->ivtNo = ivtNo;
	myEvent = 0;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo,newRoutine);
#endif
	IVT[ivtNo] = this;
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo,oldRoutine);
#endif
	IVT[ivtNo] = 0;
	callOld();
}

void IVTEntry::callOld() {
	(*oldRoutine)();
}

void IVTEntry::signal() {
	if (myEvent) myEvent->signal();
}

void IVTEntry::setMyEvent(KernelEv* myEvent) {
	this->myEvent = myEvent;
}

KernelEv* IVTEntry::getMyEvent() {
	return myEvent;
}

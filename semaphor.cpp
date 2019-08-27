/*
 * semaphor.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */
#include "semaphor.h"
#include "krnlsem.h"
#include "system.h"
#include "listpcb.h"

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init,this);
	System::semaphores->add(myImpl);
}

Semaphore::~Semaphore() {
	delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
	return myImpl->signal(n);
}

int Semaphore::val() const {
	return myImpl->val();
}

void Semaphore::incVal() {
	myImpl->incVal();
}

List* Semaphore::waitingList() {
	return myImpl->waiting;
}

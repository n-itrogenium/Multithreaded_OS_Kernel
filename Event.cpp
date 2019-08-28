/*
 * event.cpp
 *
 *  Created on: Aug 28, 2019
 *      Author: OS1
 */

#include "Event.h"
#include "kernelev.h"
#include "pcb.h"

Event::Event(IVTNo ivtNo) {
#ifndef BCC_BLOCK_IGNORE
	intrLock
#endif
	myImpl = new KernelEv(ivtNo, this);
#ifndef BCC_BLOCK_IGNORE
	intrUnlock
#endif
}

Event::~Event() {
#ifndef BCC_BLOCK_IGNORE
	intrLock
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	intrUnlock
#endif
}

void Event::wait() {
#ifndef BCC_BLOCK_IGNORE
	intrLock
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	intrUnlock
#endif
}

void Event::signal() {
#ifndef BCC_BLOCK_IGNORE
	intrLock
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	intrUnlock
#endif
}

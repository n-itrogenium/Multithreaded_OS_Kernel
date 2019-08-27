#include "frstthrd.h"
#include "system.h"

FirstThread::~FirstThread() {
	delete getMyPCB();
}

void FirstThread::inic() {
	System::counter = firstThread->getMyPCB()->timeSlice;
	firstThread->getMyPCB()->state = READY;
	PCB::running = firstThread->getMyPCB();
}

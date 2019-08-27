#include "frstthrd.h"
#include "system.h"
#include "pcb.h"

FirstThread::~FirstThread() {
	delete getMyPCB();
}

void FirstThread::inic() {
	System::counter = System::firstThread->getMyPCB()->timeSlice;
	System::firstThread->getMyPCB()->state = READY;
	PCB::running = System::firstThread->getMyPCB();
}

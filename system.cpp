/*
 * system.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "system.h"
#include "thread.h"
#include "pcb.h"
#include <dos.h>
#include <stdio.h>
#include "idle.h"
#include "frstthrd.h"
#include "listsem.h"


List* System::threads = new List();
List* System::blockedThreads = new List();
SemList* System::semaphores = new SemList();

volatile unsigned System::lockFlag = 1;

volatile int System::counter = 20;
volatile int System::context_on_demand = 0;

FirstThread* System::firstThread = 0;
Idle* System::idleThread = 0;

pInterrupt System::oldRoutine = 0;

Address tsp = 0, tss = 0, tbp = 0;


void System::inic() {
#ifndef BCC_BLOCK_IGNORE
	System::oldRoutine = getvect(OLD_ENTRY);
	setvect(OLD_ENTRY, timer);
#endif

	System::firstThread = new FirstThread();
	System::firstThread->inic();

	System::idleThread = new Idle();
	System::idleThread->start();
}

void System::restore() {
#ifndef BCC_BLOCK_IGNORE
	setvect(OLD_ENTRY, System::oldRoutine);
#endif
	delete blockedThreads;
}


void interrupt timer(...) {

	if (!System::context_on_demand) {
		System::counter--;
		tick();
		(*System::oldRoutine)();
		System::blockedThreads->tickTime();
	}
	if (System::counter == 0 || System::context_on_demand) {
		if (System::lockFlag) { 	// If it's unlocked
			System::context_on_demand = 0;	// Reset request
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
	#endif
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if (PCB::running->myThread != System::idleThread
					&& PCB::running->state == READY)
				Scheduler::put((PCB *)PCB::running);

			PCB::running = Scheduler::get();
			if (!(PCB::running)) PCB::running = System::idleThread->getMyPCB();

			System::counter = PCB::running->timeSlice;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;


#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif
		}
		else {
			System::context_on_demand = 1;
		}
	}
}

int syncPrintf(const char *format, ...) {
	int res;
	va_list args;
#ifndef BCC_BLOCK_IGNORE
    asm { pushf; cli; }
    va_start(args, format);
    res = vprintf(format, args);
    va_end(args);
    asm { popf; }
#endif
    return res;
}

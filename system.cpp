/*
 * system.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "system.h"
#include "pcb.h"

volatile int System::counter = 0;
volatile int System::context_on_demand = 0;

void System::inic() {
#ifndef BCC_BLOCK_IGNORE
	pInterrupt oldRoutine = getvect(OLD_ENTRY);
	setvect(OLD_ENTRY, timer);
	setvect(NEW_ENTRY, oldRoutine);
#endif
	firstThread = new FirstThread();
	firstThread->inic();

	idleThread = new Idle();
	idleThread->start();
}

void System::restore() {
#ifndef BCC_BLOCK_IGNORE
	setvect(0x0008, oldRoutine);
#endif
	delete firstThread;
	delete idleThread;
	threads->~List();
	delete threads;
}


void interrupt timer() {

	if (!System::context_on_demand) System::counter--;
	if (System::counter == 0 || System::context_on_demand) {
		if (lockFlag) { 	// If it's unlocked
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

			if (!(PCB::running->state == FINISHED))
				Scheduler::put((PCB *)PCB::running);
			PCB::running = Scheduler::get();

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			System::counter = PCB::running->timeSlice;

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

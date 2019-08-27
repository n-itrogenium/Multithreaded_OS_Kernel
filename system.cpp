/*
 * system.cpp
 *
 *  Created on: Aug 23, 2019
 *      Author: OS1
 */

#include "system.h"
#include "pcb.h"
#include <dos.h>
#include <stdio.h>
#include "idle.h"
#include "frstthrd.h"

List* System::threads = new List();
volatile unsigned System::lockFlag = 1;
volatile int System::counter = 0;
volatile int System::context_on_demand = 0;
FirstThread* System::firstThread = 0;
Idle* System::idleThread = 0;
pInterrupt System::oldRoutine = 0;
Address tsp = 0, tss = 0, tbp = 0;

void System::inic() {
#ifndef BCC_BLOCK_IGNORE
	System::oldRoutine = getvect(OLD_ENTRY);
	setvect(OLD_ENTRY, timer);
	setvect(NEW_ENTRY, System::oldRoutine);
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
	delete firstThread;
	delete idleThread;
	threads->~List();
	delete threads;
}


void interrupt timer(...) {

	if (!System::context_on_demand) {
		System::counter--;
		tick();
		asm int 60h
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

/////////////////////////NE ZABORAVI DA OBRISES!!!!!//////////////////
			//printf("Kontekst niti je %d\n", PCB::running->myThread->getId());

			if (PCB::running->myThread != System::idleThread
					&& PCB::running->state == READY)
				Scheduler::put((PCB *)PCB::running);

			PCB::running = Scheduler::get();
			if (!(PCB::running)) PCB::running = System::idleThread->getMyPCB();

/////////////////////////NE ZABORAVI DA OBRISES!!!!!//////////////////
			//printf("Promenjen kontekst niti na %d\n", PCB::running->myThread->getId());

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

/*void syncPrintf(char *text, ID id) {
	lock
	printf(text,id);
	unlock
}*/

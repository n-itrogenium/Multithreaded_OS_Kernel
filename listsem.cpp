/*
 * listsem.cpp
 *
 *  Created on: Aug 27, 2019
 *      Author: OS1
 */

#include "listsem.h"
#include "krnlsem.h"
#include "listpcb.h"
#include "pcb.h"

SemList::SemList() {
	head = tail = 0;
	num_of_nodes = 0;
}

SemList::~SemList() {
	Node *oldNode;
	while (head) {
		oldNode = head;
		head = head->next;
		delete oldNode;
	}
}

void SemList::add(KernelSem* sem) {
	Node *newNode = new Node(sem);
	if (num_of_nodes == 0)
		head = tail = newNode;
	else
		tail = tail->next = newNode;
	num_of_nodes++;
}

void SemList::remove(KernelSem* sem) {
	Node *temp = head, *prev = 0, *oldNode;
	while (temp) {
		if (temp->sem != sem) {
			prev = temp;
			temp = temp->next;
		}
		else {
			oldNode = temp;
			temp = temp->next;
			if (!prev) head = temp;
			else prev->next = temp;
			delete oldNode;
		}
	}
}

int SemList::isEmpty() {
	if (num_of_nodes == 0) return 1;
	else return 0;
}

KernelSem* SemList::getFirst() {
	if (!head) return 0;
	Node *temp = head;
	head = head->next;
	if (!head) tail = 0;
	num_of_nodes--;
	return temp->sem;
}


void SemList::onTick() {
	Node* curr = head;
	while (curr) {
		List::Node *temp = curr->sem->limitedTime.head;
		int counter = 0;
		while (temp) {
			temp->pcb->waitTime--;
			if (temp->pcb->waitTime <= 0) {
				counter++;
				temp->pcb->timeExceeded = 0;
				temp->pcb->semWaitingOn = 0;
				temp->pcb->state = READY;
				Scheduler::put(temp->pcb);
				temp = temp->next;
				curr->sem->limitedTime.remove(temp->pcb);
				curr->sem->total.remove(temp->pcb);
			}
			else
				temp = temp->next;
		}
		curr->sem->value += counter;
		curr = curr->next;
	}
}



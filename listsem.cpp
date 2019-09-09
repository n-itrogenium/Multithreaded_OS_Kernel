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
		num_of_nodes--;
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
			num_of_nodes--;
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
	Node* temp = head;
	while (temp) {
		List::Node *curr = temp->sem->limitedTime.head, *prev = 0;
			int counter = 0;
			while (curr) {
				curr->pcb->waitTime--;
				if (curr->pcb->waitTime != 0) {
					prev = curr; curr = curr->next;
				}
				else {
					counter++;
					List::Node* old = curr;
					old->pcb->timeExceeded = 0;
					old->pcb->state = READY;
					Scheduler::put(old->pcb);
					curr = curr->next;
					if (!prev) temp->sem->limitedTime.head = curr;
					else prev->next = curr;
					delete old;
					temp->sem->limitedTime.num_of_nodes--;
				}
			}
		temp->sem->value += counter;
		temp = temp->next;
	}
}



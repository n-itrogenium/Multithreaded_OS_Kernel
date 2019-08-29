#include "listpcb.h"
#include "pcb.h"
#include "krnlsem.h"

List::List() {
	head = tail = 0;
	num_of_nodes = 0;
}

List::~List() {
	Node *oldNode;
	while (head) {
		oldNode = head;
		head = head->next;
		delete oldNode;
	}
}


void List::add(PCB * pcb) {
	Node *newNode = new Node(pcb);
	if (num_of_nodes == 0)
		head = tail = newNode;
	else
		tail = tail->next = newNode;
	num_of_nodes++;
}


void List::remove(PCB* pcb) {
	Node *temp = head, *prev = 0, *oldNode;
	while (temp) {
		if (temp->pcb != pcb) {
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


int List::isEmpty() {
	if (num_of_nodes == 0) return 1;
	else return 0;
}


PCB* List::getFirst() {
	if (!head) return 0;
	Node *temp = head;
	PCB* temp_pcb = temp->pcb;
	head = head->next;
	if (!head) tail = 0;
	num_of_nodes--;
	delete temp;
	return temp_pcb;
}


PCB* List::get(int ID) {
	Node *temp = head;
	while (temp) {
		if (temp->pcb->myThread->getId() == ID)
			return temp->pcb;
		temp = temp->next;
	}
	return 0;
}


int List::onTick() {
	if (!head) return 0;

	Node *temp = head;
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
			remove(temp->pcb);
		}
		else
			temp = temp->next;
	}
	return counter;
}
















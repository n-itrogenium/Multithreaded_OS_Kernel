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
	head = head->next;
	if (!head) tail = 0;
	num_of_nodes--;
	return temp->pcb;
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

void List::tickTime() {
	if (!head) {
		unlock
		return;
	}
	Node* temp = head;
	PCB* curr_pcb = 0;
	while (temp) {
		if (--(temp->pcb->waitTime) == 0)
			curr_pcb = temp->pcb;
			curr_pcb->timeFlag = 0;
			curr_pcb->semWaitingOn->incVal();
			curr_pcb->semWaitingOn->waitingList()->remove(curr_pcb);
			curr_pcb->semWaitingOn = 0;
			curr_pcb->myThread->start();
			remove(curr_pcb);
		temp = temp->next;
	}
}


















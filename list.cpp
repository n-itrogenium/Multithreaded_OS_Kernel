#include "list.h"


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

void List::remove(int ID) {
	Node *temp = head, *prev = 0, *oldNode;
	while (temp) {
		if (temp->pcb->myThread->getId() != ID) {
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

PCB* List::get(int ID) {
	Node *temp = head;
	while (temp) {
		if (temp->pcb->myThread->getId() == ID)
			return temp->pcb;
		temp = temp->next;
	}
	return 0;
}

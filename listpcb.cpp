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
	//syncPrintf("Nit %d stavljam u red.\n",pcb->myThread->getId());
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

void List::tickTime() {
	//printf("Usla sam u metodu tickTime.\n");
	if (!head) {
		//printf("LISTA USPAVANIH NITI JE PRAZNA\nKontekst niti: %d\n",PCB::running->myThread->getId());
		return;
	}
	Node* temp = head, *prev = 0;
	PCB* curr_pcb = 0;
	while (temp) {
		temp->pcb->waitTime--;

		if (temp->pcb->waitTime == 0) {
			printf("Nit %d ---------------- odblokirana!\n",temp->pcb->myThread->getId());
			curr_pcb = temp->pcb;
			curr_pcb->timeExceeded = 0;
			curr_pcb->semWaitingOn->incVal();
			curr_pcb->semWaitingOn->waitingList()->remove(curr_pcb);
			curr_pcb->semWaitingOn = 0;
			curr_pcb->state = READY;
			Scheduler::put(curr_pcb);
			//printf("THEN: Sa elementa %d prelazim na element %d\n",temp->pcb->myThread->getId(),temp->next->pcb->myThread->getId());
			//if (!temp->next) printf("Kraj liste.\n");
			temp = temp->next;
			remove(curr_pcb);
		}  else //printf("Nijedna se nije odblokirala.\n");
		 {
			//printf("ELSE: Sa elementa %d prelazim na element %d\n",temp->pcb->myThread->getId(),temp->next->pcb->myThread->getId());
			//if (!temp->next) printf("Kraj liste.\n");
			temp = temp->next;
		 }


	}
	//printf("Izlazim iz metode tickTime.\n");
}


















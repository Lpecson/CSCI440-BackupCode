#include "schedule.h"
#include <stdlib.h>

struct node
{
	PCB* pcbptr;
	struct node* next;
};
typedef struct node node;

struct Queue
{
	node* front;
	node* end;
	int size;
};
typedef struct Queue Queue;

/**
 * Function to initialize any global variables for the scheduler.
 */
Queue myQ;
void init(){
	myQ.front = NULL;
	myQ.end = NULL;
	myQ.size = 0;
}

/**
 * Function to add a process to the scheduler
 * @Param PCB * - pointer to the PCB for the process/thread to be added to the
 *      scheduler queue
 * @return true/false response for if the addition was successful
 */
int addProcess(PCB *process){
	node* tmp = (node*) malloc(sizeof(node));
	tmp->pcbptr = process;
	tmp->next = NULL;
	if(hasProcess())
	{
		myQ.end->next = tmp;
		myQ.end = tmp;
	}
	else
	{
		myQ.front = tmp;
		myQ.end = tmp;
	}
	myQ.size++;
	return 1;
}

/**
 * Function to get the next process from the scheduler
 *
 * @Return returns the Process Control Block of the next process that should be
 *      executed, returns NULL if there are no processes
 */
PCB* nextProcess(){
	PCB* process;
	if( !hasProcess() )
	{
		return NULL;
	}
	else
	{
		process = myQ.front->pcbptr;
		node *tmp;
		tmp = myQ.front;
		myQ.front = myQ.front->next;
		myQ.size--;
		free(tmp);
		return process;
	}
}

/**
 * Function that returns a boolean 1 True/0 False based on if there are any
 * processes still scheduled
 * @Return 1 if there are processes still scheduled 0 if there are no more
 *		scheduled processes
 */
int hasProcess(){
	if( myQ.front )
	{
		return 1;
	}
	return 0;
}

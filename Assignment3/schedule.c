#include "schedule.h"
#include <stdlib.h>
//MultilevelRR


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
Queue myQ[4];

void init(){
	for( int i=0; i<4; i++)
	{
		myQ[i].front = NULL;
		myQ[i].end = NULL;
		myQ[i].size = 0;
	}
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
	int prio = process->priority;
	if(hasProcess())
	{
		myQ[prio].end->next = tmp;
		myQ[prio].end = tmp;
	}
	else
	{
		myQ[prio].front = tmp;
		myQ[prio].end = tmp;
	}
	myQ[prio].size++;
	return 1;
}

/*
// Helper function that just returns which priority q is not empty
*/
int nextProcessPrio() {
	if(  myQ[0].front )
	{
		return 0;
	}	
	if(  myQ[1].front )
	{
		return 1;
	}	
	if(  myQ[2].front )
	{
		return 2;
	}	
	if(  myQ[3].front )
	{
		return 3;
	}
	return -1;
}

/**
 * Function to get the next process from the scheduler
 *
 * @Return returns the Process Control Block of the next process that should be
 *      executed, returns NULL if there are no processes
 */
PCB* nextProcess(int *time){
	PCB* process;
	if( !hasProcess() )
	{
		return NULL;
	}
	int Prio = nextProcessPrio();
	*time = 4 - Prio;
	process = myQ[Prio].front->pcbptr;
	node *tmp;
	tmp = myQ[Prio].front;
	myQ[Prio].front = myQ[Prio].front->next;
	myQ[Prio].size--;
	free(tmp);
	return process;
}


/**
 * Function that returns a boolean 1 True/0 False based on if there are any
 * processes still scheduled
 * @Return 1 if there are processes still scheduled 0 if there are no more
 *		scheduled processes
 */
int hasProcess(){
	for( int i=0; i<4; i++ )
	{
		if( myQ[i].front )
		{
			return 1;
		}
	}
	return 0;
}

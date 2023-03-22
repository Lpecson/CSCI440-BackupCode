#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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
	if(myQ[prio].front)
	{
		myQ[prio].end->next = tmp;
		myQ[prio].end = tmp;
	}
	else
	{
		myQ[prio].front = tmp;
		myQ[prio].end = tmp;
	}
	return 1;
}

/**
 * Function to get the next process from the scheduler
 *
 * @Return returns the Process Control Block of the next process that should be
 *      executed, returns NULL if there are no processes
 */
PCB* nextProcess(int *time){
	PCB* process;
	static int index = -1;
	if( !hasProcess() )
	{
		return NULL;
	}
	index = (index + 1) % 4;//mod operator on static index of size 4 to get range 0-3
	while(!myQ[index].front)//ensures we do not run myQ[index] if that index is complete
	{
		index = (index + 1) % 4;
	}
	process = myQ[index].front->pcbptr;
	*time = 4 - process->priority;
	node *tmp;
	tmp = myQ[index].front;
	myQ[index].front = myQ[index].front->next;
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

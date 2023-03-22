#include "schedule.h"
#include <stdlib.h>
//MultilevelFeedback


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
Queue myQ[3];

void init(){
	for( int i=0; i<3; i++)
	{
		myQ[i].front = NULL;
		myQ[i].end = NULL;
	}
}

/**
 * Function called every simulated time period to provide a mechanism
 * to age the scheduled processes and trigger feedback if needed.
 */
void age(){
	for( int i=1; i<=2; i++ )//loop for aging
	{
		if( myQ[i].front )
		{
			node* tmp = myQ[i].front;
			while( tmp )
			{
				tmp->pcbptr->age++;
				if( tmp->next )
				{
					tmp = tmp->next;
				}
				else
				{
					tmp = NULL;
				}
			}
		}//go through the bottom 2 RR queues and age each process
	}
	for( int i=1; i<=2; i++ )//loop for promotion
	{
		if( myQ[i].front )
		{
			node* tmp = myQ[i].front;
			while( tmp )
			{
				if( tmp->pcbptr->age >= 1000)//process needs to be aged up
				{
					PCB* add = tmp->pcbptr;//remove process from current prio
					add->priority--;
					myQ[i].front = tmp->next;
					addProcess(add);//add process to current
					free(tmp);
					tmp = NULL;
				}
				if( tmp )
				{
					tmp = tmp->next;
				}
				else
				{
					tmp = NULL;
				}
			}
		}//go through the bottom 2 RR queues and promote any processes that are age == 1000
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
	tmp->pcbptr->age = 0;
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
	if( !hasProcess() )
	{
		return NULL;
	}
	if(myQ[0].front)//Run through the FCFS Q first
	{
		process = myQ[0].front->pcbptr;
		*time = -1;
		node *tmp;
		tmp = myQ[0].front;
		myQ[0].front = myQ[0].front->next;
		free(tmp);
		return process;	
	}
	else if(myQ[1].front)//if FCFS is empty run highRR Q
	{
		process = myQ[1].front->pcbptr;
		*time = 4;
		node *tmp;
		tmp = myQ[1].front;
		myQ[1].front = myQ[1].front->next;
		free(tmp);
		return process;	
	}
	else//both other Queues are empty run lowRR Q
	{
		process = myQ[2].front->pcbptr;
		*time = 1;
		node *tmp;
		tmp = myQ[2].front;
		myQ[2].front = myQ[2].front->next;
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
	for( int i=0; i<3; i++ )
	{
		if( myQ[i].front )
		{
			return 1;
		}
	}
	return 0;
}
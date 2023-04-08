/*
 * File: pager-lru.c
 * Author:       Lukas Pecson
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

	/* This file contains the stub for an LRU pager */
	/* You may need to add/remove/modify any part of this file */

	/* Static vars */
	static int initialized = 0;
	static int tick = 1; // artificial time
	static int timestamps[MAXPROCESSES][MAXPROCPAGES];

	/* Local vars */
	int proctmp;
	int pagetmp;
	int proc;
	int pc;
	int page;
	int oldpage;
	int lr;//least recently used page tick val

	/* initialize static vars on first run */
	if (!initialized) 
	{
		for (proctmp = 0; proctmp < MAXPROCESSES; proctmp++)
		{
			for (pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
				timestamps[proctmp][pagetmp] = 0;
			}
		}
		initialized = 1;
	} //use tick for max 
	/* pentry struct
	/  long active - flag indicating whether process completee or not
	/  long pc - value of the program counter for the process current page = pc/PAGESIZE
	/  long npages - the number of pages in the processes mem space, if process is active  this will equal MAXPROCPAGES, if exited this will be 0
	/  long pages[MAXPROCPAGES] - a bitmap array representing the page map for a given process. if pages[X] = 0 X is swapped out, swapping out/in if pages[X] is 1 page X is currently swapped in.

	/  Select first active process */
	for (proc = 0; proc < MAXPROCESSES; proc++) 
	{
		/* Is process active? */
		if (q[proc].active) 
		{
			pc = q[proc].pc; 		        // program counter for process
			page = pc / PAGESIZE; 		// page the program counter needs
			timestamps[proc][page] = tick; //update time
			if(!q[proc].pages[page])//not swapped out
			{
				if(!pagein(proc, page))//try to pagein the page
				{
					lr = tick;// set lr to tick our highest possible value(tick is total time)
					for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++)
					{
						if(q[proc].pages[pagetmp] && timestamps[proc][pagetmp] < lr )
						{
							oldpage = pagetmp;
							lr = timestamps[proc][pagetmp];
						}
					}
					pageout(proc, oldpage);//dont worry if unsuccessful
				}
			}
		}
	}
	//exit(EXIT_FAILURE);
	/* advance time for next pageit iteration */
	tick++;
}
